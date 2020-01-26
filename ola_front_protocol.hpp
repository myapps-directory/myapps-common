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

    uint32_t    version_ = version;
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
        solid::serialization::addVersion<AuthRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.auth_, _rctx, "auth").add(_rthis.pass_, _rctx, "pass");
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
        const AuthRequest& _rreq, const uint32_t _version = version)
        : solid::frame::mprpc::Message(_rreq)
        , version_(_version)
        , error_(-1)
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

struct ListOSesRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<ListOSesRequest>(_s, _rthis.version_, "version");
    }
};

struct ListOSesResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                 version_ = version;
    uint32_t                 error_   = -1;
    std::string              message_;
    std::vector<std::string> osvec_;

    ListOSesResponse() {}

    ListOSesResponse(
        const ListOSesRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<ListOSesResponse>(_s, _rthis.version_, "version");
        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
            _s.add(_rthis.osvec_, _rctx, "osvec");
        },
            _rctx, _name);
    }
};

struct ListAppsRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t version_ = version;
    uint8_t  choice_; //o - owned applications
        //a - aquired applications
        //A - all applications

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<ListAppsRequest>(_s, _rthis.version_, "version");
        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.choice_, _rctx, "choice");
        },
            _rctx, _name);
    }
};

struct ListAppsResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    using AppVectorT = std::vector<utility::ListApplicationItem>;

    uint32_t    version_ = version;
    uint32_t    error_   = -1;
    std::string message_;
    AppVectorT  app_vec_;

    ListAppsResponse() {}

    ListAppsResponse(
        const ListAppsRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<ListAppsResponse>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
            _s.add(_rthis.app_vec_, _rctx, "app_vec");
        },
            _rctx, _name);
    }
};

struct FetchBuildUpdatesRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                 version_ = version;
    std::string              lang_;
    std::string              os_id_;
    std::vector<std::string> app_id_vec_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchBuildUpdatesRequest>(_s, _rthis.version_, "version");
        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.lang_, _rctx, "lang");
            _s.add(_rthis.os_id_, _rctx, "os_id");
            _s.add(_rthis.app_id_vec_, _rctx, "app_id_vec");
        },
            _rctx, _name);
    }
};

struct FetchBuildUpdatesResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                                         version_ = version;
    uint32_t                                         error_   = -1;
    std::string                                      message_;
    std::vector<std::pair<std::string, std::string>> app_vec_; //first means app_unique, second means build_unique

    FetchBuildUpdatesResponse() {}

    FetchBuildUpdatesResponse(
        const FetchBuildUpdatesRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchBuildUpdatesResponse>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
            _s.add(_rthis.app_vec_, _rctx, "app_vec");
        },
            _rctx, _name);
    }
};

struct ListStoreRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t    version_ = version;
    std::string storage_id_;
    std::string path_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<ListStoreRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.storage_id_, _rctx, "storage_id");
            _s.add(_rthis.path_, _rctx, "path");
        },
            _rctx, _name);
    }
};

struct ListStoreResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                                version_      = version;
    uint32_t                                node_version_ = ola::utility::ListStoreNode::version;
    uint32_t                                error_        = -1;
    std::string                             message_;
    std::deque<ola::utility::ListStoreNode> node_dq_;

    ListStoreResponse() {}

    ListStoreResponse(
        const ListStoreRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<ListStoreResponse>(_s, _rthis.version_, "version");
        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            solid::serialization::addVersion<ola::utility::ListStoreNode>(_s, _rthis.node_version_, "node_version");

            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
            _s.add(_rthis.node_dq_, _rctx, "node_dq");
        },
            _rctx, _name);
    }
};

struct FetchStoreRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t    version_ = version;
    std::string storage_id_;
    std::string path_;
    uint64_t    offset_ = 0;
    uint64_t    size_   = 0;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchStoreRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.storage_id_, _rctx, "storage_id");
            _s.add(_rthis.path_, _rctx, "path");
            _s.add(_rthis.offset_, _rctx, "offset");
            _s.add(_rthis.size_, _rctx, "size");
        },
            _rctx, _name);
    }
};

struct FetchStoreResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                   version_ = version;
    uint32_t                   error_   = -1;
    std::string                message_;
    int64_t                    size_ = 0;
    mutable std::istringstream iss_;
    std::stringstream          ioss_;

    FetchStoreResponse() {}

    FetchStoreResponse(
        const FetchStoreRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchStoreResponse>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name) {
            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
            _s.add(_rthis.size_, _rctx, "size");

            if constexpr (S::is_serializer) {
                auto progress_lambda = [](std::istream& _ris, uint64_t _len, const bool _done, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name) {

                };
                _s.add(_rthis.iss_, progress_lambda, _rctx, _name);
            } else {
                auto progress_lambda = [](std::ostream& _ros, uint64_t _len, const bool _done, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name) {

                };
                _s.add(_rthis.ioss_, progress_lambda, _rctx, _name);
            }
        },
            _rctx, _name);
    }
};

struct FetchAppRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t    version_ = version;
    std::string app_id_;
    std::string lang_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchAppRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.lang_, _rctx, "lang");
        },
            _rctx, _name);
    }
};

struct FetchAppResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                 version_             = version;
    uint32_t                 application_version_ = utility::Application::version;
    uint32_t                 error_               = -1;
    std::string              message_;
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
        solid::serialization::addVersion<FetchAppResponse>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            solid::serialization::addVersion<utility::Application>(_s, _rthis.application_version_, "application_version");

            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
            _s.add(_rthis.application_, _rctx, "application");
            _s.add(_rthis.build_id_vec_, _rctx, "build_id_vec");
        },
            _rctx, _name);
    }
};

struct FetchBuildRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t    version_ = version;
    std::string app_id_;
    std::string build_id_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchBuildRequest>(_s, _rthis.version_, "version");
        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.build_id_, _rctx, "build_id");
        },
            _rctx, _name);
    }
};

struct FetchBuildResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t          version_       = version;
    uint32_t          build_version_ = utility::Build::version;
    uint32_t          error_         = -1;
    std::string       message_;
    std::string       storage_id_;
    std::vector<char> image_blob_;
    utility::Build    build_;

    FetchBuildResponse() {}

    FetchBuildResponse(
        const FetchBuildRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchBuildResponse>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            solid::serialization::addVersion<utility::Build>(_s, _rthis.build_version_, "build_version");

            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
            _s.add(_rthis.storage_id_, _rctx, "storage_id");
            _s.add(_rthis.image_blob_, _rctx, "image_blob");
            _s.add(_rthis.build_, _rctx, "build");
        },
            _rctx, _name);
    }
};

struct FetchBuildConfigurationRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                                version_ = version;
    std::string                             app_id_;
    std::string                             lang_;
    std::string                             os_id_;
    ola::utility::Build::FetchOptionBitsetT fetch_options_;
    std::vector<std::string>                property_vec_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchBuildConfigurationRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.lang_, _rctx, "lang");
            _s.add(_rthis.os_id_, _rctx, "os_id");
            _s.add(_rthis.fetch_options_, _rctx, "fetch_options");
            _s.add(_rthis.property_vec_, _rctx, "property_vec");
        },
            _rctx, _name);
    }
};

struct FetchBuildConfigurationResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                           version_               = version;
    uint32_t                           configuration_version_ = ola::utility::Build::Configuration::version;
    uint32_t                           error_                 = -1;
    std::string                        message_;
    std::string                        app_unique_;
    std::string                        build_unique_;
    std::string                        storage_id_;
    ola::utility::Build::Configuration configuration_;
    std::vector<char>                  image_blob_;

    FetchBuildConfigurationResponse() {}

    FetchBuildConfigurationResponse(
        const FetchBuildConfigurationRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchBuildConfigurationResponse>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            solid::serialization::addVersion<ola::utility::Build::Configuration>(_s, _rthis.configuration_version_, "configuration_version");

            _s.add(_rthis.error_, _rctx, "error").add(_rthis.message_, _rctx, "message");
            _s.add(_rthis.app_unique_, _rctx, "app_unique");
            _s.add(_rthis.build_unique_, _rctx, "build_unique");
            _s.add(_rthis.storage_id_, _rctx, "storage_id");
            _s.add(_rthis.configuration_, _rctx, "configuration");
            _s.add(_rthis.image_blob_, solid::serialization::limit(1024 * 1024), _rctx, "image_blob");
        },
            _rctx, _name);
    }
};

struct FetchMediaConfigurationRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t    version_ = version;
    std::string app_id_;
    std::string lang_;
    std::string os_id_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchMediaConfigurationRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.lang_, _rctx, "lang");
            _s.add(_rthis.os_id_, _rctx, "os_id");
        },
            _rctx, _name);
    }
};

struct FetchMediaConfigurationResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                           version_               = version;
    uint32_t                           configuration_version_ = ola::utility::Build::Configuration::version;
    uint32_t                           error_                 = -1;
    std::string                        message_;
    std::string                        unique_;
    std::string                        storage_id_;
    ola::utility::Media::Configuration configuration_;

    FetchMediaConfigurationResponse() {}

    FetchMediaConfigurationResponse(
        const FetchMediaConfigurationRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchMediaConfigurationResponse>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            solid::serialization::addVersion<ola::utility::Media::Configuration>(_s, _rthis.configuration_version_, "configuration_version");

            _s.add(_rthis.error_, _rctx, "error");
            _s.add(_rthis.message_, _rctx, "message");
            _s.add(_rthis.unique_, _rctx, "unique_");
            _s.add(_rthis.storage_id_, _rctx, "storage_id");
            _s.add(_rthis.configuration_, _rctx, "configuration");
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

struct CreateAppRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version              = 1;
    uint32_t                  version_             = version;
    uint32_t                  application_version_ = utility::Application::version;
    utility::Application      application_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<CreateAppRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            solid::serialization::addVersion<utility::Application>(_s, _rthis.application_version_, "application_version");

            _s.add(_rthis.application_, _rctx, "application");
        },
            _rctx, _name);
    }
};

struct AcquireAppRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t    version_ = version;
    std::string app_id_;
    bool        acquire_ = true;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<AcquireAppRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.acquire_, _rctx, "acquire");
        },
            _rctx, _name);
    }
};

struct CreateBuildRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t          version_       = version;
    uint32_t          build_version_ = utility::Build::version;
    std::string       app_id_;
    std::string       unique_; //there cannot be two builds with the same tag per application
    uint64_t          size_;
    std::string       sha_sum_;
    std::vector<char> image_blob_;
    utility::Build    build_;

    CreateBuildRequest()
        : size_(0)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<CreateBuildRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            solid::serialization::addVersion<utility::Build>(_s, _rthis.build_version_, "build_version");

            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.unique_, _rctx, "unique");
            _s.add(_rthis.size_, _rctx, "size");
            _s.add(_rthis.sha_sum_, _rctx, "sha_sum");
            _s.add(_rthis.image_blob_, solid::serialization::limit(1024 * 1024), _rctx, "image_blob");
            _s.add(_rthis.build_, _rctx, "build");
        },
            _rctx, _name);
    }
};

struct CreateMediaRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t       version_       = version;
    uint32_t       media_version_ = utility::Media::version;
    std::string    app_id_;
    std::string    unique_; //there cannot be two builds with the same tag per application
    uint64_t       size_;
    std::string    sha_sum_;
    utility::Media media_;

    CreateMediaRequest()
        : size_(0)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<CreateMediaRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            solid::serialization::addVersion<utility::Media>(_s, _rthis.media_version_, "media_version");
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.unique_, _rctx, "unique");
            _s.add(_rthis.size_, _rctx, "size");
            _s.add(_rthis.sha_sum_, _rctx, "sha_sum");
            _s.add(_rthis.media_, _rctx, "media");
        },
            _rctx, _name);
    }
};

struct UploadRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t              version_ = version;
    mutable std::ifstream ifs_;
    std::ostringstream    oss_;

    template <class S>
    void solidSerializeV2(S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name) const
    {
        solid::serialization::addVersion<UploadRequest>(_s, version_, "version");

        _s.add([this](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            //on serializer side
            auto progress_lambda = [](std::istream& _ris, uint64_t _len, const bool _done, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name) {
            };
            _s.add(ifs_, 100 * 1024, solid::serialization::limit(100 * 1024), progress_lambda, _rctx, "file");
        },
            _rctx, _name);
    }

    template <class S>
    void solidSerializeV2(S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name)
    {
        solid::serialization::addVersion<UploadRequest>(_s, version_, "version");

        _s.add([this](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            auto progress_lambda = [](std::ostream& _ros, uint64_t _len, const bool _done, solid::frame::mprpc::ConnectionContext& _rctx, const char* _name) {
            };
            _s.add(oss_, solid::serialization::limit(100 * 1024), progress_lambda, _rctx, "file");
        },
            _rctx, _name);
    }
};

using ProtocolT = solid::frame::mprpc::serialization_v2::Protocol<uint8_t>;

template <class R>
inline void protocol_setup(R _r, ProtocolT& _rproto)
{
    _rproto.version(1, 0);

    _rproto.null(static_cast<ProtocolT::TypeIdT>(0));

    _r(_rproto, solid::TypeToType<InitRequest>(), 1);
    _r(_rproto, solid::TypeToType<InitResponse>(), 2);

    _r(_rproto, solid::TypeToType<AuthRequest>(), 4);
    _r(_rproto, solid::TypeToType<AuthResponse>(), 5);
    _r(_rproto, solid::TypeToType<Response>(), 6);

    _r(_rproto, solid::TypeToType<CreateAppRequest>(), 10);

    _r(_rproto, solid::TypeToType<ListOSesRequest>(), 20);
    _r(_rproto, solid::TypeToType<ListOSesResponse>(), 21);

    _r(_rproto, solid::TypeToType<ListAppsRequest>(), 22);
    _r(_rproto, solid::TypeToType<ListAppsResponse>(), 23);

    _r(_rproto, solid::TypeToType<ListStoreRequest>(), 24);
    _r(_rproto, solid::TypeToType<ListStoreResponse>(), 25);

    _r(_rproto, solid::TypeToType<FetchStoreRequest>(), 30);
    _r(_rproto, solid::TypeToType<FetchStoreResponse>(), 31);

    _r(_rproto, solid::TypeToType<CreateBuildRequest>(), 40);
    _r(_rproto, solid::TypeToType<CreateMediaRequest>(), 41);
    _r(_rproto, solid::TypeToType<UploadRequest>(), 44);

    _r(_rproto, solid::TypeToType<FetchAppRequest>(), 50);
    _r(_rproto, solid::TypeToType<FetchAppResponse>(), 51);

    _r(_rproto, solid::TypeToType<FetchBuildRequest>(), 60);
    _r(_rproto, solid::TypeToType<FetchBuildResponse>(), 61);
    _r(_rproto, solid::TypeToType<FetchBuildConfigurationRequest>(), 62);
    _r(_rproto, solid::TypeToType<FetchBuildConfigurationResponse>(), 63);
    _r(_rproto, solid::TypeToType<FetchMediaConfigurationRequest>(), 64);
    _r(_rproto, solid::TypeToType<FetchMediaConfigurationResponse>(), 65);

    _r(_rproto, solid::TypeToType<FetchBuildUpdatesRequest>(), 70);
    _r(_rproto, solid::TypeToType<FetchBuildUpdatesResponse>(), 71);

    _r(_rproto, solid::TypeToType<AcquireAppRequest>(), 100);
}

} //namespace front
} //namespace ola
