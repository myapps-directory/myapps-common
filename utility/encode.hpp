#pragma once

#include <istream>
#include <string>

namespace myapps {

namespace utility {

std::string sha256(const std::string& str);
std::string sha256(std::istream& _ris);

std::string base64_encode(const std::string& _txt);
std::string base64_decode(const std::string& _txt);

std::string hex_encode(const std::string& _txt);
std::string hex_decode(const std::string& _txt);

} // namespace utility
} // namespace myapps
