#pragma once
#include "ola/common/ola_front_protocol_common.hpp"

namespace ola {
namespace front {

struct InitRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<InitRequest>(_s, _rthis.version_, "version");
        _rctx.addVersion(_s);
    }
};

struct InitResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t    version_ = version;
    uint32_t    error_   = -1;
    std::string message_;

    InitResponse() {}

    InitResponse(
        const InitRequest& _rreq, const uint32_t _version = version)
        : solid::frame::mprpc::Message(_rreq)
        , version_(_version)
        , error_(-1)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<InitResponse>(_s, _rthis.version_, "version");
        _rctx.addVersion(_s);

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
        },
            _rctx, _name);
    }
};

struct AuthRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    std::string pass_;
    std::string user_;
    std::string captcha_text_;
    std::string captcha_token_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<AuthRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.pass_, _rctx, "pass").add(_rthis.user_, _rctx, "user");
            _s.add(_rthis.captcha_text_, _rctx, "captcha_text").add(_rthis.captcha_token_, _rctx, "captcha_token");
        },
            _rctx, _name);
    }
};

struct AuthResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t    version_ = version;
    uint32_t    error_   = -1;
    std::string message_;

    AuthResponse() {}

    AuthResponse(
        const solid::frame::mprpc::Message& _rreq, const uint32_t _version = version)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<AuthResponse>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
        },
            _rctx, _name);
    }
};

struct Response : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t    version_ = version;
    uint32_t    error_   = -1;
    std::string message_;

    Response() {}

    Response(
        const solid::frame::mprpc::Message& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<Response>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
        },
            _rctx, _name);
    }
};

template <class R>
inline void protocol_setup_init(R _r, ProtocolT& _rproto)
{
    protocol_setup(_rproto);

    _r(_rproto, solid::TypeToType<InitRequest>(), {0, 1});
    _r(_rproto, solid::TypeToType<InitResponse>(), {0, 2});

    _r(_rproto, solid::TypeToType<AuthRequest>(), {0, 4});
    _r(_rproto, solid::TypeToType<AuthResponse>(), {0, 5});
    _r(_rproto, solid::TypeToType<Response>(), {0, 6});
}

} //namespace front
} //namespace ola
