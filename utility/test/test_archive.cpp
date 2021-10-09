#include "ola/common/utility/archive.hpp"
#include "solid/system/exception.hpp"
#include "solid/system/log.hpp"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

namespace {
const string archive_root    = "test_archive_root";
const string archive_path    = "test_archive.zip";
const string archive_extract = "test_archive_extract";
string       pattern;

void create_file(const string& _path, size_t _size)
{
    ofstream ofs(_path, std::ifstream::binary);

    while (_size) {
        size_t towrite = _size;
        if (towrite > pattern.size()) {
            towrite = pattern.size();
        }
        ofs.write(pattern.c_str(), towrite);
        _size -= towrite;
    }
}

void create_files(const string& _root, size_t _count, size_t _max_size)
{
    for (size_t i = 0; i < _count; ++i) {
        ostringstream oss;
        oss << _root << "/" << hex << setw(4) << setfill('0') << i;
        create_file(oss.str(), i % _max_size);
    }
}
} //namespace

int test_archive(int argc, char* argv[])
{
    solid::log_start(std::cerr, {".*:EW"});
    namespace fs = boost::filesystem;

    boost::system::error_code err;
    fs::remove_all(archive_root, err);
    fs::remove_all(archive_path, err);
    fs::remove_all(archive_extract, err);

    for (int j = 0; pattern.size() < 4 * 1024; ++j) {
        for (int i = 0; i < 127; ++i) {
            int c = (i + j) % 127;
            if (isprint(c) != 0 && isblank(c) == 0) {
                pattern += static_cast<char>(c);
            }
        }
    }

    fs::path archive_root_path(archive_root);

    solid_check(fs::create_directory(archive_root_path, err));
    create_files(archive_root, 100, 4 * 1024);
    solid_check(fs::create_directory(archive_root_path / "first", err));
    create_files((archive_root_path / "first").generic_string(), 100, 4 * 1024);
    solid_check(fs::create_directory(archive_root_path / "second", err));
    create_files((archive_root_path / "second").generic_string(), 100, 4 * 1024);
    solid_check(fs::create_directory(archive_root_path / "second" / "third", err));
    create_files((archive_root_path / "second" / "third").generic_string(), 100, 4 * 1024);

    uint64_t create_total_size = 0;
    solid_check(ola::utility::archive_create(archive_path, archive_root, create_total_size));

    uint64_t extract_total_size = 0;
    solid_check(fs::create_directory(archive_extract, err));
    solid_check(ola::utility::archive_extract(archive_path, archive_extract, extract_total_size));
    solid_check(create_total_size == extract_total_size && extract_total_size != 0);
    return 0;
}
