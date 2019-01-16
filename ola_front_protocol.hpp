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

struct ListAppsRequest : solid::frame::mprpc::Message {
    uint8_t choice_;
    std::string static_fields_;
    std::vector<std::string> field_vec_;
    
    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.choice_, _rctx, "choice");
        _s.add(_rthis.static_fields_, _rctx, "static_fields");
        _s.add(_rthis.field_vec_, _rctx, "field_vec");
    }
};

struct ListAppsResponse : solid::frame::mprpc::Message {
    std::vector<std::string> app_vec_;

    ListAppsResponse() {}

    ListAppsResponse(
        const ListAppsRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.app_vec_, _rctx, "app_vec");
    }
    
    template <class F>
    void visit(F _f, const std::string &_static_fields, const std::vector<std::string> &_field_vec){
        const size_t field_cnt = _static_fields.size() + _field_vec.size();
        std::string empty;
        for(size_t i = 0; i < app_vec_.size(); ++i){
            const std::string &v = app_vec_[i];
            const size_t field_idx = i % field_cnt;
            const bool is_first = field_idx == 0;
            const bool is_last = field_idx == (field_cnt - 1);
            const char static_field = field_idx < _static_fields.size() ? _static_fields[field_idx] : '\0';
            const std::string &field = field_idx < _static_fields.size() ? empty : _field_vec[field_idx];
            
            _f(static_field, field, v, is_first, is_last);
        }
    }
};

struct Response : solid::frame::mprpc::Message {
    uint32_t    error_;
    std::string message_;

    Response() {}

    Response(
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

struct CreateAppRequest : solid::frame::mprpc::Message {
    utility::AppConfig config_;

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
    _r(_rproto, solid::TypeToType<Response>(), 3);

    _r(_rproto, solid::TypeToType<ListOSesRequest>(), 10);
    _r(_rproto, solid::TypeToType<ListOSesResponse>(), 11);

    _r(_rproto, solid::TypeToType<CreateAppRequest>(), 21);
}

} //namespace front
} //namespace ola
