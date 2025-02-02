// myapps/common/utility/encode.hpp

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

#include <istream>
#include <string>

namespace myapps {

namespace utility {

std::string sha256(const std::string& str);
std::string sha256(std::istream& _ris);

std::string base64_encode(const std::string_view& _txt);
std::string base64_decode(const std::string_view& _txt);

std::string hex_encode(const std::string_view& _txt);
std::string hex_decode(const std::string_view& _txt);

} // namespace utility
} // namespace myapps
