#pragma once
#include <string>

namespace ola{
namespace utility{

bool archive_create(const std::string &_zip_path, std::string _root, uint64_t &_runcompressed_size);

bool archive_extract(const std::string& _zip_path, const std::string& _root, uint64_t& _runcompressed_size);

}//namespace utility
}//namespace ola
