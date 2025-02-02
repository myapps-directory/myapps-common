// myapps/common/utility/archive.hpp

// This file is part of MyApps.directory project
// Copyright (C) 2020, 2021, 2022, 2023, 2024, 2025 Valentin Palade (vipalade @ gmail . com)

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
