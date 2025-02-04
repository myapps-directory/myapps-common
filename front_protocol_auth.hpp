// myapps/common/front_protocol_auth.hpp

// This file is part of MyApps.directory project
// Copyright (C) 2020, 2021, 2022, 2023, 2024, 2025 Valentin Palade (vipalade @ gmail . com)

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once
#include <limits>

#include "myapps/common/front_protocol_core.hpp"

namespace myapps {
namespace front {
namespace auth {
constexpr uint8_t protocol_id = 1;

// the version is only transfered from client to server.
// the client will NOT know the server version
struct Version {
    static constexpr uint32_t version      = 1;
    static constexpr uint32_t init_request = 1;

    uint32_t version_      = version;
    uint32_t init_request_ = init_request;

    void clear() { init_request_ = std::numeric_limits<uint32_t>::max(); }

    bool operator<=(const Version& _rthat) const
    {
        return version_ <= _rthat.version_ && init_request_ <= _rthat.init_request_;
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.version_, _rctx, 1, "version");
        _r.add(
            [&_rthis](Reflector& _r, Context& _rctx) {
                if constexpr (!Reflector::is_const_reflector) {
                    if (_rthis.version > Version::version) {
                        _rthis.clear();
                        return;
                    }
                }
                if (_rthis.version_ == version) {
                    _r.add(_rthis.init_request_, _rctx, 3, "init_request");
                }
            },
            _rctx);
    }
};

constexpr Version version;

struct InitRequest : solid::frame::mprpc::Message {
    Version       auth_version_;
    core::Version core_version_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.auth_version_, _rctx, 1, "auth_version");
        _r.add(
            [&_rthis](Reflector& _r, Context& _rctx) {
                if (_rthis.auth_version_.init_request_ == Version::init_request) {
                    _r.add(_rthis.core_version_, _rctx, 3, "core_version");
                }
            },
            _rctx);
    }
};

struct CaptchaRequest : solid::frame::mprpc::Message {
    SOLID_REFLECT_V1(_r, _rthis, _rctx) {}
};

struct CaptchaResponse : solid::frame::mprpc::Message {
    std::string          captcha_token_;
    std::vector<uint8_t> captcha_image_;
    std::vector<uint8_t> captcha_audio_;

    CaptchaResponse() {}

    CaptchaResponse(const CaptchaRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.captcha_token_, _rctx, 1, "captcha_token");
        _r.add(_rthis.captcha_image_, _rctx, 2, "captcha_image",
            [](auto& _rmeta) { _rmeta.maxSize(1024 * 1024); });
        _r.add(_rthis.captcha_audio_, _rctx, 3, "captcha_audio");
    }
};

struct CreateRequest : solid::frame::mprpc::Message {
    std::string pass_;
    std::string user_;
    std::string email_;
    std::string captcha_text_;
    std::string captcha_token_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.pass_, _rctx, 1, "pass");
        _r.add(_rthis.user_, _rctx, 2, "user");
        _r.add(_rthis.email_, _rctx, 3, "email");
        _r.add(_rthis.captcha_text_, _rctx, 4, "captcha_text");
        _r.add(_rthis.captcha_token_, _rctx, 5, "captcha_token");
    }
};

struct FetchRequest : solid::frame::mprpc::Message {
    SOLID_REFLECT_V1(_r, _rthis, _rctx) {}
};

struct FetchResponse : solid::frame::mprpc::Message {
    std::string user_;
    std::string email_;

    FetchResponse() {}

    FetchResponse(const FetchRequest& _req)
        : solid::frame::mprpc::Message(_req)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.user_, _rctx, 1, "user");
        _r.add(_rthis.email_, _rctx, 2, "email");
    }
};

struct AmendRequest : solid::frame::mprpc::Message {
    std::string ticket_;
    std::string new_pass_;
    std::string new_user_;
    std::string new_email_;
    std::string pass_;
    std::string captcha_text_;
    std::string captcha_token_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.ticket_, _rctx, 1, "ticket");
        _r.add(_rthis.new_pass_, _rctx, 2, "new_pass");
        _r.add(_rthis.new_user_, _rctx, 3, "new_user");
        _r.add(_rthis.new_email_, _rctx, 4, "new_email");
        _r.add(_rthis.pass_, _rctx, 5, "pass");
        _r.add(_rthis.captcha_text_, _rctx, 6, "captcha_text");
        _r.add(_rthis.captcha_token_, _rctx, 7, "captcha_token");
    }
};

struct DeleteRequest : solid::frame::mprpc::Message {
    std::string ticket_;
    std::string pass_;
    std::string reason_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.ticket_, _rctx, 1, "ticket");
        _r.add(_rthis.pass_, _rctx, 2, "pass");
        _r.add(_rthis.reason_, _rctx, 3, "reason");
    }
};

struct ValidateRequest : solid::frame::mprpc::Message {
    std::string ticket_;
    std::string text_;
    std::string captcha_text_;
    std::string captcha_token_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.ticket_, _rctx, 1, "ticket");
        _r.add(_rthis.text_, _rctx, 2, "text");
        _r.add(_rthis.captcha_text_, _rctx, 3, "captcha_text");
        _r.add(_rthis.captcha_token_, _rctx, 4, "captcha_token");
    }
};

struct ResetRequest : solid::frame::mprpc::Message {
    std::string login_;
    std::string pass_;
    std::string captcha_text_;
    std::string captcha_token_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.login_, _rctx, 1, "login");
        _r.add(_rthis.pass_, _rctx, 2, "pass");
        _r.add(_rthis.captcha_text_, _rctx, 3, "captcha_text");
        _r.add(_rthis.captcha_token_, _rctx, 4, "captcha_token");
    }
};

template <class Reg>
inline void configure_protocol(Reg _rreg)
{
    _rreg({protocol_id, 1}, "InitRequest", solid::TypeToType<InitRequest>());

    _rreg({protocol_id, 2}, "CreateRequest", solid::TypeToType<CreateRequest>());
    _rreg({protocol_id, 3}, "ValidateRequest",
        solid::TypeToType<ValidateRequest>());
    _rreg({protocol_id, 4}, "AmendRequest", solid::TypeToType<AmendRequest>());
    _rreg({protocol_id, 5}, "FetchRequest", solid::TypeToType<FetchRequest>());
    _rreg({protocol_id, 6}, "FetchResponse", solid::TypeToType<FetchResponse>());
    _rreg({protocol_id, 7}, "ResetRequest", solid::TypeToType<ResetRequest>());
    _rreg({protocol_id, 8}, "CaptchaRequest",
        solid::TypeToType<CaptchaRequest>());
    _rreg({protocol_id, 9}, "CaptchaResponse",
        solid::TypeToType<CaptchaResponse>());
    _rreg({protocol_id, 10}, "DeleteRequest", solid::TypeToType<DeleteRequest>());
}

} // namespace auth
} // namespace front
} // namespace myapps
