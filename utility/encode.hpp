#pragma once

#include "solid/system/pimpl.hpp"
#include <istream>
#include <string>

namespace ola {

namespace utility {

std::string sha256hex(const std::string& str);
std::string sha256hex(std::istream& _ris);

std::string sha256(const std::string& str);
std::string sha256(std::istream& _ris);

class Enigma {
    struct Data;
    solid::PimplT<Data> pimpl_;

public:
    Enigma();
    ~Enigma();
    void        configure(const std::string& _s);
    std::string encode(const std::string& _txt);
    std::string decode(const std::string& _txt);
};

std::string base64_encode(const std::string& _txt);
std::string base64_decode(const std::string& _txt);

std::string hex_encode(const std::string& _txt);
std::string hex_decode(const std::string& _txt);

} //namespace utility
} //namespace ola
