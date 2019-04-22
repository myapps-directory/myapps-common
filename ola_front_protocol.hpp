#pragma once

#include "ola/common/utility/ola_error.hpp"
#include "ola/common/utility/ola_protocol.hpp"

#include <deque>
#include <fstream>
#include <sstream>
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
    uint8_t choice_; //o - owned applications
        //a - aquired applications
        //A - all applications

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.choice_, _rctx, "choice");
    }
};

struct ListAppsResponse : solid::frame::mprpc::Message {
    uint32_t                 error_ = -1;
    std::vector<std::string> app_id_vec_;

    ListAppsResponse() {}

    ListAppsResponse(
        const ListAppsRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.error_, _rctx, "error");
        _s.add(_rthis.app_id_vec_, _rctx, "app_id_vec");
    }
};

struct ListStoreRequest : solid::frame::mprpc::Message {
    std::string storage_id_;
    std::string path_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.storage_id_, _rctx, "storage_id");
        _s.add(_rthis.path_, _rctx, "path");
    }
};

struct ListStoreResponse : solid::frame::mprpc::Message {
    uint32_t                                error_ = -1;
    std::deque<ola::utility::ListStoreNode> node_dq_;

    ListStoreResponse() {}

    ListStoreResponse(
        const ListStoreRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.error_, _rctx, "error");
        _s.add(_rthis.node_dq_, _rctx, "node_dq");
    }
};

struct FetchAppRequest : solid::frame::mprpc::Message {
    std::string app_id_;
    std::string lang_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.app_id_, _rctx, "app_id");
        _s.add(_rthis.lang_, _rctx, "lang");
    }
};

struct FetchAppResponse : solid::frame::mprpc::Message {
    uint32_t                 error_ = -1;
    utility::Application     application_;
    std::vector<std::string> build_id_vec_;

    FetchAppResponse() {}

    FetchAppResponse(
        const FetchAppRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.error_, _rctx, "error");
        _s.add(_rthis.application_, _rctx, "application");
        _s.add(_rthis.build_id_vec_, _rctx, "build_id_vec");
    }
};

struct FetchBuildRequest : solid::frame::mprpc::Message {
    std::string app_id_;
    std::string build_id_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.app_id_, _rctx, "app_id");
        _s.add(_rthis.build_id_, _rctx, "build_id");
    }
};

struct FetchBuildResponse : solid::frame::mprpc::Message {
    uint32_t       error_ = -1;
    std::string    storage_id_;
    utility::Build build_;

    FetchBuildResponse() {}

    FetchBuildResponse(
        const FetchBuildRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.error_, _rctx, "error");
        _s.add(_rthis.storage_id_, _rctx, "storage_id");
        _s.add(_rthis.build_, _rctx, "build");
    }
};

struct FetchBuildConfigurationRequest : solid::frame::mprpc::Message {
    std::string              app_id_;
    std::string              lang_;
    std::string              os_id_;
    std::vector<std::string> property_vec_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.app_id_, _rctx, "app_id");
        _s.add(_rthis.lang_, _rctx, "lang");
        _s.add(_rthis.os_id_, _rctx, "os_id");
        _s.add(_rthis.property_vec_, _rctx, "property_vec");
    }
};

struct FetchBuildConfigurationResponse : solid::frame::mprpc::Message {
    uint32_t                           error_ = -1;
    std::string                        storage_id_;
    ola::utility::Build::Configuration build_configuration_;

    FetchBuildConfigurationResponse() {}

    FetchBuildConfigurationResponse(
        const FetchBuildConfigurationRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.error_, _rctx, "error");
        _s.add(_rthis.storage_id_, _rctx, "storage_id");
        _s.add(_rthis.build_configuration_, _rctx, "build_configuration");
    }
};

struct Response : solid::frame::mprpc::Message {
    uint32_t    error_ = -1;
    std::string message_;

    Response() {}

    Response(
        const solid::frame::mprpc::Message& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
    }
};

struct CreateAppRequest : solid::frame::mprpc::Message {
    utility::Application application_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.application_, _rctx, "application");
    }
};

struct AcquireAppRequest : solid::frame::mprpc::Message {
    std::string app_id_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.app_id_, _rctx, "app_id");
    }
};

struct CreateBuildRequest : solid::frame::mprpc::Message {
    std::string    app_id_;
    std::string    unique_; //there cannot be two builds with the same tag per application
    uint64_t       size_;
    std::string    sha_sum_;
    utility::Build build_;

    CreateBuildRequest()
        : size_(0)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.app_id_, _rctx, "app_id");
        _s.add(_rthis.unique_, _rctx, "unique");
        _s.add(_rthis.size_, _rctx, "size");
        _s.add(_rthis.sha_sum_, _rctx, "sha_sum");
        _s.add(_rthis.build_, _rctx, "build");
    }
};

struct UploadBuildRequest : solid::frame::mprpc::Message {
    mutable std::ifstream ifs_;
    std::ostringstream    oss_;

    template <class S>
    void solidSerializeV2(S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name) const
    {
        //on serializer side
        auto progress_lambda = [](std::istream& _ris, uint64_t _len, const bool _done, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name) {
        };
        _s.add(ifs_, 100 * 1024, progress_lambda, _rctx, "file");
    }

    template <class S>
    void solidSerializeV2(S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name)
    {
        auto progress_lambda = [](std::ostream& _ros, uint64_t _len, const bool _done, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name) {
        };
        _s.add(oss_, progress_lambda, _rctx, "file");
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

    _r(_rproto, solid::TypeToType<CreateAppRequest>(), 10);

    _r(_rproto, solid::TypeToType<ListOSesRequest>(), 20);
    _r(_rproto, solid::TypeToType<ListOSesResponse>(), 21);

    _r(_rproto, solid::TypeToType<ListAppsRequest>(), 22);
    _r(_rproto, solid::TypeToType<ListAppsResponse>(), 23);

    _r(_rproto, solid::TypeToType<ListStoreRequest>(), 24);
    _r(_rproto, solid::TypeToType<ListStoreResponse>(), 25);

    _r(_rproto, solid::TypeToType<CreateBuildRequest>(), 40);
    _r(_rproto, solid::TypeToType<UploadBuildRequest>(), 44);

    _r(_rproto, solid::TypeToType<FetchAppRequest>(), 50);
    _r(_rproto, solid::TypeToType<FetchAppResponse>(), 51);

    _r(_rproto, solid::TypeToType<FetchBuildRequest>(), 60);
    _r(_rproto, solid::TypeToType<FetchBuildResponse>(), 61);
    _r(_rproto, solid::TypeToType<FetchBuildConfigurationRequest>(), 62);
    _r(_rproto, solid::TypeToType<FetchBuildConfigurationResponse>(), 63);

    _r(_rproto, solid::TypeToType<AcquireAppRequest>(), 100);
}

} //namespace front
} //namespace ola
