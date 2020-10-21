#pragma once
#include "ola/common/ola_front_protocol_common.hpp"

namespace ola {
namespace front {

struct CaptchaRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<CaptchaRequest>(_s, _rthis.version_, "version");
    }
};

struct CaptchaResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    std::string          captcha_token_;
    std::vector<uint8_t> captcha_image_;
    std::vector<uint8_t> captcha_audio_;

    CaptchaResponse() {}

    CaptchaResponse(
        const CaptchaRequest& _rreq, const uint32_t _version = version)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<CaptchaResponse>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.captcha_token_, _rctx, "captcha_token").add(_rthis.captcha_image_, solid::serialization::limit(1024 * 1024), _rctx, "captcha_image");
            _s.add(_rthis.captcha_audio_, _rctx, "captcha_audio");
        },
            _rctx, _name);
    }
};

struct AuthCreateRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    std::string pass_;
    std::string user_;
    std::string email_;
    std::string captcha_text_;
    std::string captcha_token_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<AuthCreateRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.pass_, _rctx, "pass").add(_rthis.user_, _rctx, "user");
            _s.add(_rthis.email_, _rctx, "email");
            _s.add(_rthis.captcha_text_, _rctx, "captcha_text").add(_rthis.captcha_token_, _rctx, "captcha_token");
        },
            _rctx, _name);
    }
};

struct AuthFetchRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<AuthCreateRequest>(_s, _rthis.version_, "version");
    }
};

struct AuthFetchResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    std::string user_;
    std::string email_;

    AuthFetchResponse() {}

    AuthFetchResponse(const AuthFetchRequest& _req)
        : solid::frame::mprpc::Message(_req)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<AuthCreateRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.user_, _rctx, "user");
            _s.add(_rthis.email_, _rctx, "email");
        },
            _rctx, _name);
    }
};

struct AuthAmendRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    std::string ticket_;
    std::string new_pass_;
    std::string new_user_;
    std::string new_email_;
    std::string pass_;
    std::string captcha_text_;
    std::string captcha_token_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<AuthCreateRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.ticket_, _rctx, "ticket");
            _s.add(_rthis.new_pass_, _rctx, "new_pass").add(_rthis.new_user_, _rctx, "new_user");
            _s.add(_rthis.new_email_, _rctx, "new_email");
            _s.add(_rthis.pass_, _rctx, "pass");
            _s.add(_rthis.captcha_text_, _rctx, "captcha_text").add(_rthis.captcha_token_, _rctx, "captcha_token");
        },
            _rctx, _name);
    }
};

struct AuthValidateRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    std::string ticket_;
    std::string text_;
    std::string captcha_text_;
    std::string captcha_token_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<AuthValidateRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.ticket_, _rctx, "ticket");
            _s.add(_rthis.text_, _rctx, "text");
            _s.add(_rthis.captcha_text_, _rctx, "captcha_text").add(_rthis.captcha_token_, _rctx, "captcha_token");
        },
            _rctx, _name);
    }
};

struct AuthResetRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    std::string login_;
    std::string pass_;
    std::string captcha_text_;
    std::string captcha_token_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<AuthResetRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.login_, _rctx, "login");
            _s.add(_rthis.pass_, _rctx, "pass");
            _s.add(_rthis.captcha_text_, _rctx, "captcha_text").add(_rthis.captcha_token_, _rctx, "captcha_token");
        },
            _rctx, _name);
    }
};
template <class R>
inline void protocol_setup_auth(R _r, ProtocolT& _rproto)
{
    _r(_rproto, solid::TypeToType<AuthCreateRequest>(), {1, 1});
    _r(_rproto, solid::TypeToType<AuthValidateRequest>(), {1, 2});
    _r(_rproto, solid::TypeToType<AuthAmendRequest>(), {1, 3});
    _r(_rproto, solid::TypeToType<AuthFetchRequest>(), {1, 4});
    _r(_rproto, solid::TypeToType<AuthFetchResponse>(), {1, 5});
    _r(_rproto, solid::TypeToType<AuthResetRequest>(), {1, 6});
    _r(_rproto, solid::TypeToType<CaptchaRequest>(), {1, 7});
    _r(_rproto, solid::TypeToType<CaptchaResponse>(), {1, 8});
}

} //namespace front
} //namespace ola
