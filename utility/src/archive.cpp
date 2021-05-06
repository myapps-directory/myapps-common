#include <boost/filesystem.hpp>
#include "solid/system/log.hpp"
#include "ola/common/utility/archive.hpp"
#include "zip.h"

using namespace std;

namespace ola{
namespace utility{
namespace{
solid::LoggerT  logger("ola::utility::archive");
//-----------------------------------------------------------------------------

bool zip_add_file(zip_t *_pzip, const boost::filesystem::path &_path, size_t _base_path_len, uint64_t &_rsize){
	string path = _path.generic_string();
    zip_source_t *psrc = zip_source_file(_pzip, path.c_str(), 0, 0);
    if(psrc != nullptr){
        _rsize += file_size(_path);
        zip_int64_t err = zip_file_add(_pzip, (path.c_str() + _base_path_len), psrc, ZIP_FL_ENC_UTF_8);
        solid_log(logger, Info, ""<<(path.c_str() + _base_path_len)<<" rv = "<<err);
        if(err < 0){
            zip_source_free(psrc);
        }
    }
    return true;
}

//-----------------------------------------------------------------------------

bool zip_add_dir(zip_t *_pzip, const boost::filesystem::path &_path, size_t _base_path_len, uint64_t &_rsize){
    using namespace boost::filesystem;
    string path = _path.generic_string();
    zip_int64_t err = zip_dir_add(_pzip, (path.c_str() + _base_path_len), ZIP_FL_ENC_UTF_8);
    
    solid_log(logger, Info, ""<<(path.c_str() + _base_path_len)<<" rv = "<<err);
    
    for (directory_entry& x : directory_iterator(_path)){
        auto p = x.path();
        if(is_directory(p)){
            zip_add_dir(_pzip, p, _base_path_len, _rsize);
        }else{
            zip_add_file(_pzip, p, _base_path_len, _rsize);
        }        
    }
    return true;
}
}//namespace

bool archive_create(const std::string &_zip_path, std::string _root, uint64_t &_runcompressed_size){
   using namespace boost::filesystem;
    
    int err;
    zip_t *pzip = zip_open(_zip_path.c_str(), ZIP_CREATE| ZIP_EXCL, &err);
    
    if(pzip == nullptr){
        zip_error_t error;
        zip_error_init_with_code(&error, err);
        solid_log(logger, Error, "Creating archive: "<<zip_error_strerror(&error));
        zip_error_fini(&error);
        return false;
    }
    
    if(!_root.empty() && _root.back() != '/'){
        _root += '/';
    }
    
    solid_log(logger, Info, "Create archive: "<<_zip_path <<" from "<<_root);
    _runcompressed_size = 0;
    
    if(!is_directory(_root)){
        solid_log(logger, Error, "Path: "<<_root<<" not a directory");
        return false;
    }
    
    for (directory_entry& x : directory_iterator(_root)){
        auto p = x.path();
        if(is_directory(p)){
            zip_add_dir(pzip, p, _root.size(), _runcompressed_size);
        }else{
            zip_add_file(pzip, p, _root.size(), _runcompressed_size);
        }
        
    }
    zip_close(pzip);
    return true; 
}

bool archive_extract(const std::string& _zip_path, const std::string& _root, uint64_t& _runcompressed_size){
    using namespace boost::filesystem;

    int              err;
    zip_t*           pzip = zip_open(_zip_path.c_str(), ZIP_RDONLY, &err);
    zip_stat_t       stat;
    constexpr size_t bufcp = 1024 * 64;
    char             buf[bufcp];

    for (int64_t i = 0; i < zip_get_num_entries(pzip, 0); i++) {
        if (zip_stat_index(pzip, i, 0, &stat) == 0) {
            _runcompressed_size += stat.size;
            size_t name_len = strlen(stat.name);
            if (stat.name[name_len - 1] == '/') {
                //folder
                create_directory(_root + '/' + stat.name);
            } else {
                zip_file* pzf = zip_fopen_index(pzip, i, 0);

                if (pzf) {
                    std::ofstream ofs(_root + '/' + stat.name);
                    uint64_t      fsz = 0;
                    do {
                        auto v = zip_fread(pzf, buf, bufcp);
                        if (v > 0) {
                            ofs.write(buf, v);
                            fsz += v;
                        } else {
                            break;
                        }
                    } while (true);
                    if (fsz != stat.size) {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
    }
    return true;
}

}//namespace utility
}//namespace ola
