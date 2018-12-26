#pragma once

#include "solid/system/pimpl.hpp"
#include <string>

namespace ola {

namespace utility {

std::string sha256(const std::string& str);

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

} //namespace utility
} //namespace ola
