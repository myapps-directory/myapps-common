#pragma once

#include "solid/frame/mprpc/mprpccontext.hpp"
#include "solid/frame/mprpc/mprpcmessage.hpp"
#include "solid/frame/mprpc/mprpcprotocol_serialization_v2.hpp"

#include <string>

namespace ola {
namespace front {

inline const char* default_port()
{
    return "4443";
}

enum struct ErrorE : uint32_t {
    Generic = 1,
    Exist,
    Backend,
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

struct ListOSesRequest : solid::frame::mprpc::Message {
    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
    }
};

struct ListOSesResponse : solid::frame::mprpc::Message {
    std::vector<std::string> osvec_;

    ListOSesResponse() {}

    ListOSesResponse(
        const ListOSesRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.osvec_, _rctx, "osvec");
    }
};

struct GenericResponse : solid::frame::mprpc::Message {
    uint32_t    error_;
    std::string message_;

    GenericResponse() {}

    GenericResponse(
        const solid::frame::mprpc::Message& _rreq)
        : solid::frame::mprpc::Message(_rreq)
        , error_(-1)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
    }
};

struct AppConfig{
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
};

struct CreateAppRequest: solid::frame::mprpc::Message{
    AppConfig config_;
    
    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.config_, _rctx, "config");
    }
};

using ProtocolT = solid::frame::mprpc::serialization_v2::Protocol<uint8_t>;

template <class R>
inline void protocol_setup(R _r, ProtocolT& _rproto)
{
    _rproto.null(static_cast<ProtocolT::TypeIdT>(0));

    _r(_rproto, solid::TypeToType<AuthRequest>(), 1);
    _r(_rproto, solid::TypeToType<AuthResponse>(), 2);
    _r(_rproto, solid::TypeToType<GenericResponse>(), 3);

    _r(_rproto, solid::TypeToType<ListOSesRequest>(), 10);
    _r(_rproto, solid::TypeToType<ListOSesResponse>(), 11);
    
    _r(_rproto, solid::TypeToType<CreateAppRequest>(), 21);
}

} //namespace front
} //namespace ola
