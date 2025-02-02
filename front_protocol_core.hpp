// myapps/common/front_protocol_core.hpp

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

#include "myapps/common/utility/error.hpp"
#include "myapps/common/utility/protocol.hpp"

#include <string>

namespace myapps {
namespace front {

inline constexpr const char* default_port() { return "4443"; }

using ProtocolTypeIdT = std::pair<uint8_t, uint16_t>;

namespace core {

constexpr uint8_t protocol_id = 0;

struct Version {
    static constexpr uint32_t version       = 1;
    static constexpr uint32_t auth_request  = 1;
    static constexpr uint32_t auth_response = 1;
    static constexpr uint32_t init_response = 1;
    static constexpr uint32_t response      = 1;

    uint32_t version_       = version;
    uint32_t auth_request_  = auth_request;
    uint32_t auth_response_ = auth_response;
    uint32_t init_response_ = init_response;
    uint32_t response_      = response;

    void clear() { auth_request_ = auth_response_ = response_ = -1; }

    bool operator<=(const Version& _rthat) const
    {
        return version_ <= _rthat.version_ && auth_request <= _rthat.auth_request_ && auth_response_ <= _rthat.auth_response_ && init_response_ <= _rthat.init_response_ && response_ <= _rthat.response_;
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
                    _r.add(_rthis.auth_request_, _rctx, 3, "auth_request");
                    _r.add(_rthis.auth_response_, _rctx, 4, "auth_response");
                    _r.add(_rthis.init_response_, _rctx, 5, "init_response");
                    _r.add(_rthis.response_, _rctx, 6, "response");
                }
            },
            _rctx);
    }
};

constexpr Version version;

struct AuthRequest : solid::frame::mprpc::Message {
    std::string pass_;
    std::string user_;
    std::string captcha_text_;
    std::string captcha_token_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.pass_, _rctx, 1, "pass");
        _r.add(_rthis.user_, _rctx, 2, "user");
        _r.add(_rthis.captcha_text_, _rctx, 3, "captcha_text");
        _r.add(_rthis.captcha_token_, _rctx, 4, "captcha_token");
    }
};

struct AuthResponse : solid::frame::mprpc::Message {
    uint32_t    error_ = -1;
    std::string message_;

    AuthResponse() {}

    AuthResponse(const solid::frame::mprpc::Message& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "error");
        _r.add(_rthis.message_, _rctx, 2, "message");
    }
};

struct InitResponse : solid::frame::mprpc::Message {
    uint32_t    error_ = -1;
    std::string message_;

    InitResponse() {}

    InitResponse(const solid::frame::mprpc::Message& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        if (_rctx.any().template get_if<Version>() && _rctx.any().template get_if<Version>()->init_response_ == Version::init_response) {
            _r.add(_rthis.error_, _rctx, 1, "error");
            _r.add(_rthis.message_, _rctx, 2, "message");
        }
    }
};

struct Response : solid::frame::mprpc::Message {
    uint32_t    error_ = -1;
    std::string message_;

    Response() {}

    Response(const solid::frame::mprpc::Message& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "error");
        _r.add(_rthis.message_, _rctx, 2, "message");
    }
};

template <class Reg>
inline void configure_protocol(Reg _rreg)
{
    _rreg({protocol_id, 1}, "InitResponse", solid::TypeToType<InitResponse>());
    _rreg({protocol_id, 2}, "AuthRequest", solid::TypeToType<AuthRequest>());
    _rreg({protocol_id, 3}, "AuthResponse", solid::TypeToType<AuthResponse>());
    _rreg({protocol_id, 4}, "Response", solid::TypeToType<Response>());
}

} // namespace core
} // namespace front
} // namespace myapps
