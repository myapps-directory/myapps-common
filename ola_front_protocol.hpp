#pragma once

#include "solid/frame/mprpc/mprpccontext.hpp"
#include "solid/frame/mprpc/mprpcmessage.hpp"
#include "solid/frame/mprpc/mprpcprotocol_serialization_v2.hpp"

#include <string>

namespace ola {
namespace front {

inline const char * default_port(){
    return "4443";
}

enum struct ErrorE : uint32_t {
    Generic = 1,
    Exist,
};

struct AuthRequest : solid::frame::mprpc::Message {
    std::string auth_;
    std::string pass_;

    AuthRequest() {}

    AuthRequest(
        const std::string& _auth,
        const std::string& _pass = std::string())
        : auth_(_auth)
        , pass_(_pass)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.auth_, _rctx, "auth").add(_rthis.pass_, _rctx, "pass");
    }
};

struct AuthResponse : solid::frame::mprpc::Message {
    uint32_t    error_;
    std::string message_;

    AuthResponse() {}

    AuthResponse(
        const AuthRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
        , error_(-1)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
    }
};

using ProtocolT = solid::frame::mprpc::serialization_v2::Protocol<uint8_t>;

template <class R>
inline void protocol_setup(R _r, ProtocolT& _rproto)
{
    _rproto.null(static_cast<ProtocolT::TypeIdT>(0));

    _r(_rproto, solid::TypeToType<AuthRequest>(), 1);
    _r(_rproto, solid::TypeToType<AuthResponse>(), 2);
}

} //namespace front
} //namespace ola

