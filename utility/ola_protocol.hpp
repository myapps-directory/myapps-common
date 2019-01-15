#pragma once

#include "solid/frame/mprpc/mprpcprotocol_serialization_v2.hpp"
#include <string>
#include <vector>

namespace ola{
namespace utility{

struct AppConfig {
    using KVVectorT = std::vector<std::pair<std::string, std::string>>;
    std::string name_;
    std::string short_description_;
    std::string description_;
    KVVectorT   name_vec_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.name_, _rctx, "name");
        _s.add(_rthis.short_description_, _rctx, "short_description");
        _s.add(_rthis.description_, _rctx, "description");
        _s.add(_rthis.name_vec_, _rctx, "name_vec");
    }
    
    template <class Archive>
    void serialize(Archive& _a)
    {
        _a(name_, short_description_, description_, name_vec_);
    }
};


}//namespace utility
}//namespace ola
