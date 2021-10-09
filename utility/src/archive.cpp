#include "ola/common/utility/archive.hpp"
#include "solid/system/log.hpp"
#include "zip.h"
#include <boost/filesystem.hpp>

using namespace std;

namespace ola {
namespace utility {
namespace {
constexpr uint16_t meta_extra_field_id = 0x3333;
solid::LoggerT     logger("ola::utility::archive");
//-----------------------------------------------------------------------------

bool zip_add_file(
    zip_t* _pzip, const boost::filesystem::path& _path, size_t _base_path_len, uint64_t& _rsize,
    const CreateFileMetaFunctionT& _rmeta_fnc, vector<uint8_t>& _rmeta_data)
{
    string        path = _path.generic_string();
    zip_source_t* psrc = zip_source_file(_pzip, path.c_str(), 0, 0);
    if (psrc != nullptr) {
        _rsize += file_size(_path);
        zip_int64_t index = zip_file_add(_pzip, (path.c_str() + _base_path_len), psrc, ZIP_FL_ENC_UTF_8);
        solid_log(logger, Info, "" << (path.c_str() + _base_path_len) << " rv = " << index);
        if (index < 0) {
            zip_source_free(psrc);
        } else {
            _rmeta_data.clear();
            _rmeta_fnc(path, _rmeta_data);
            if (!_rmeta_data.empty()) {
                zip_file_extra_field_set(_pzip, index, meta_extra_field_id, 0, _rmeta_data.data(), _rmeta_data.size(), ZIP_FL_LOCAL);
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------------

bool zip_add_dir(
    zip_t* _pzip, const boost::filesystem::path& _path, size_t _base_path_len, uint64_t& _rsize,
    const CreateFileMetaFunctionT& _rmeta_fnc, vector<uint8_t>& _rmeta_data)
{
    using namespace boost::filesystem;
    string      path = _path.generic_string();
    zip_int64_t err  = zip_dir_add(_pzip, (path.c_str() + _base_path_len), ZIP_FL_ENC_UTF_8);

    solid_log(logger, Info, "" << (path.c_str() + _base_path_len) << " rv = " << err);

    for (directory_entry& x : directory_iterator(_path)) {
        auto p = x.path();
        if (is_directory(p)) {
            zip_add_dir(_pzip, p, _base_path_len, _rsize, _rmeta_fnc, _rmeta_data);
        } else {
            zip_add_file(_pzip, p, _base_path_len, _rsize, _rmeta_fnc, _rmeta_data);
        }
    }
    return true;
}
} //namespace

bool archive_create(
    const std::string& _zip_path, std::string _root, uint64_t& _runcompressed_size,
    CreateFileMetaFunctionT _meta_fnc)
{
    using namespace boost::filesystem;

    int             err;
    zip_t*          pzip = zip_open(_zip_path.c_str(), ZIP_CREATE | ZIP_EXCL, &err);
    vector<uint8_t> meta_data;

    if (pzip == nullptr) {
        zip_error_t error;
        zip_error_init_with_code(&error, err);
        solid_log(logger, Error, "Creating archive: " << zip_error_strerror(&error));
        zip_error_fini(&error);
        return false;
    }

    if (!_root.empty() && _root.back() != '/') {
        _root += '/';
    }

    solid_log(logger, Info, "Create archive: " << _zip_path << " from " << _root);
    _runcompressed_size = 0;

    if (!is_directory(_root)) {
        solid_log(logger, Error, "Path: " << _root << " not a directory");
        return false;
    }

    for (directory_entry& x : directory_iterator(_root)) {
        auto p = x.path();
        if (is_directory(p)) {
            zip_add_dir(pzip, p, _root.size(), _runcompressed_size, _meta_fnc, meta_data);
        } else {
            zip_add_file(pzip, p, _root.size(), _runcompressed_size, _meta_fnc, meta_data);
        }
    }
    zip_close(pzip);
    return true;
}

bool do_archive_extract(
    const std::string& _zip_path, const std::string& _root, uint64_t& _runcompressed_size,
    OnCreateDirectoryFunctionT& _on_create_dir_function,
    CreateWriteFunctionT&       _create_file_writer_function)
{
    using namespace boost::filesystem;

    int                       err;
    zip_t*                    pzip = zip_open(_zip_path.c_str(), ZIP_RDONLY, &err);
    zip_stat_t                stat;
    constexpr size_t          bufcp = 1024 * 64;
    char                      buf[bufcp];
    boost::system::error_code error;
    const int64_t             num_entries = zip_get_num_entries(pzip, 0);
    for (int64_t i = 0; i < num_entries; ++i) {
        if (zip_stat_index(pzip, i, 0, &stat) == 0) {
            _runcompressed_size += stat.size;
            size_t name_len = strlen(stat.name);
            if (stat.name[name_len - 1] == '/') {

                //folder
                if (!create_directory(_root + '/' + stat.name, error) || !_on_create_dir_function(stat.name)) {
                    return false;
                }
                solid_log(logger, Info, "created directory: " << stat.name);
            } else {
                zip_file* pzf = zip_fopen_index(pzip, i, 0);

                if (pzf) {
                    //std::ofstream ofs(_root + '/' + stat.name);
                    uint16_t    meta_data_size = 0;
                    const auto* meta_data      = zip_file_extra_field_get_by_id(pzip, i, meta_extra_field_id, 0, &meta_data_size, ZIP_FL_LOCAL);

                    FileWriteFunctionT file_write_function = _create_file_writer_function(stat.name, stat.size, meta_data, meta_data_size);
                    if (!file_write_function) {
                        return false;
                    }
                    uint64_t fsz = 0;
                    do {
                        auto v = zip_fread(pzf, buf, bufcp);
                        if (v > 0) {
                            if (!file_write_function(buf, v)) {
                                return false;
                            }
                            fsz += v;
                        } else {
                            break;
                        }
                    } while (true);
                    if (fsz != stat.size) {
                        return false;
                    }
                    solid_log(logger, Info, "Created file: " << stat.name);
                } else {
                    return false;
                }
            }
        }
    }
    return true;
}

bool archive_extract(const std::string& _path, const std::string& _root, uint64_t& _runcompressed_size)
{
    auto create_dir_lambda   = [](const char*) { return true; };
    auto create_write_lambda = [&_root](const char* _file_name, uint64_t /*_size*/, const uint8_t*, uint16_t) {
        std::ofstream ofs(_root + '/' + _file_name, std::ofstream::binary);
        if (ofs) {
            auto lambda = [ofs = std::move(ofs)](const char* _buf, size_t _len) mutable {
                ofs.write(_buf, _len);
                return ofs.good();
            };
            return FileWriteFunctionT{std::move(lambda)};
        } else {
            return FileWriteFunctionT{};
        }
    };
    return archive_extract(_path, _root, _runcompressed_size, create_dir_lambda, create_write_lambda);
}

} //namespace utility
} //namespace ola
