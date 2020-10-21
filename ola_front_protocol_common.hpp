#pragma once

#include "ola/common/utility/ola_error.hpp"
#include "ola/common/utility/ola_protocol.hpp"

#include <string>

namespace ola {
namespace front {

inline const char* default_port()
{
    return "4443";
}

using TypeIdT = std::pair<uint8_t, uint16_t>;

struct TypeIdHash {
    using argument_type = TypeIdT;
    using result_type   = std::size_t;

    result_type operator()(argument_type const& s) const noexcept
    {
        result_type const h1(std::hash<uint8_t>{}(s.first));
        result_type const h2(std::hash<uint16_t>{}(s.second));
        return h1 ^ (h2 << 8);
    }
};

using ProtocolT = solid::frame::mprpc::serialization_v2::Protocol<TypeIdT, TypeIdHash>;

inline void protocol_setup(ProtocolT& _rproto)
{
    _rproto.version(2, 0);

    _rproto.null(TypeIdT(0, 0));
}

} //namespace front
} //namespace ola
