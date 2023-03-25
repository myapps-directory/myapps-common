#pragma once
#include "solid/utility/function.hpp"
#include <fstream>
#include <string>
#include <vector>

namespace myapps {
namespace utility {

constexpr const char* metadata_name = ".myapps_metadata";

using FileWriteFunctionT         = solid::Function<bool(const char*, size_t)>;
using OnCreateDirectoryFunctionT = solid::Function<bool(const char*)>;
using CreateWriteFunctionT       = solid::Function<FileWriteFunctionT(const char*, uint64_t, const uint8_t*, uint16_t)>;
using CreateFileMetaFunctionT    = solid::Function<void(const std::string&, std::vector<uint8_t>&)>;

bool archive_create(
    const std::string& _path, std::string _root, uint64_t& _runcompressed_size,
    CreateFileMetaFunctionT _meta_fnc = [](const std::string&, std::vector<uint8_t>&) {});

bool do_archive_extract(
    const std::string& _path, const std::string& _root, uint64_t& _runcompressed_size,
    OnCreateDirectoryFunctionT& _on_create_dir_function,
    CreateWriteFunctionT&       _create_file_writer_function);

template <class CreateDirFnc, class CreateWriteFnc>
bool archive_extract(const std::string& _path, const std::string& _root, uint64_t& _runcompressed_size, CreateDirFnc _create_dir_fnc, CreateWriteFnc _create_write_fnc)
{
    OnCreateDirectoryFunctionT create_dir_fnc(_create_dir_fnc);
    CreateWriteFunctionT       create_write_fnc(_create_write_fnc);
    return do_archive_extract(_path, _root, _runcompressed_size, create_dir_fnc, create_write_fnc);
}

bool archive_extract(const std::string& _path, const std::string& _root, uint64_t& _runcompressed_size);

} // namespace utility
} // namespace myapps
