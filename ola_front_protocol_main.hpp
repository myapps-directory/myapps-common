#pragma once

#include "ola/common/ola_front_protocol_core.hpp"

#include <deque>
#include <fstream>
#include <sstream>

namespace ola {
namespace front {
namespace main{

constexpr size_t protocol_id = 2;

//the version is only transfered from client to server.
//the client will NOT know the server version
struct Version{
    static constexpr uint32_t version = 1;
    static constexpr uint32_t init_request = 1;
        
    uint32_t version_ = version;
    uint32_t init_request_ = init_request;
    
    void clear(){
        init_request_ = std::numeric_limits<uint32_t>::max();
    }
    
    bool operator<=(const Version& _rthat)const{
        return version_ <= _rthat.version_ && init_request_ <= _rthat.init_request_;
    }
    
    SOLID_REFLECT_V1(_s, _rthis, _rctx){
        _s.add(_rthis.version_, _rctx, 0, "version");
        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx) {
            if constexpr (!S::is_const_reflector){
                if(_rthis.version > Version::version){
                    _rthis.clear();
                    return;
                }
            }
            if(_rthis.version_ == version){
                _s.add(_rthis.init_request_, _rctx, 2, "init_request");
            }
        },
            _rctx, 1, "lambda");
    }
    
};

constexpr Version version;

struct InitRequest : solid::frame::mprpc::Message {
    Version main_version_;
    core::Version core_version_;
    utility::Version utility_version_;

    SOLID_REFLECT_V1(_s, _rthis, _rctx)
    {
        _s.add(_rthis.auth_version_, _rctx, 0, "main_version");
        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx) {
            
            if(_rthis.version_.init_request_ == Version::init_request){
                _s.add(_rthis.core_version_, _rctx, 3, "core_version");
                _s.add(_rthis.utility_version_, _rctx, 4, "utility_version");
            }
        },
            _rctx, 1, "lambda");
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

    using AppVectorT = std::vector<utility::ApplicationListItem>;

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
    using StringPairT                 = std::pair<std::string, std::string>;

    uint32_t                 version_ = version;
    std::string              lang_;
    std::string              os_id_;
    std::vector<StringPairT> app_id_vec_;

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
    std::string os_id_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchAppRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.os_id_, _rctx, "os_id");
        },
            _rctx, _name);
    }
};

struct ChangeAppItemStateRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                   version_ = version;
    std::string                app_id_;
    std::string                os_id_;
    ola::utility::AppItemEntry item_;
    uint8_t                    new_state_ = 0;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<ChangeAppItemStateRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.os_id_, _rctx, "os_id");
            _s.add(_rthis.item_, _rctx, "item");
            _s.add(_rthis.new_state_, _rctx, "new_state");
        },
            _rctx, _name);
    }
};

struct FetchAppResponse : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;
    using ItemEntryVectorT            = std::vector<ola::utility::AppItemEntry>;

    uint32_t             version_             = version;
    uint32_t             application_version_ = utility::Application::version;
    uint32_t             error_               = -1;
    std::string          message_;
    utility::Application application_;
    ItemEntryVectorT     item_vec_;

    FetchAppResponse() {}

    FetchAppResponse(
        const FetchAppRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    FetchAppResponse(
        const ChangeAppItemStateRequest& _rreq)
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
            _s.add(_rthis.item_vec_, _rctx, "item_vec");
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
            _s.add(_rthis.image_blob_, solid::serialization::limit(1024 * 1024), _rctx, "image_blob");
            _s.add(_rthis.build_, _rctx, "build");
        },
            _rctx, _name);
    }
};

struct FetchBuildConfigurationRequest : solid::frame::mprpc::Message {
    static constexpr uint32_t version = 1;

    uint32_t                                version_ = version;
    std::string                             app_id_;
    std::string                             build_id_;
    std::string                             lang_;
    std::string                             os_id_;
    ola::utility::Build::FetchOptionBitsetT fetch_options_;
    std::vector<std::string>                property_vec_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<FetchBuildConfigurationRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.build_id_, _rctx, "build_id");
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
    std::string                        build_storage_id_;
    std::string                        media_storage_id_;
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
            _s.add(_rthis.build_storage_id_, _rctx, "build_storage_id");
            _s.add(_rthis.media_storage_id_, _rctx, "media_storage_id");
            _s.add(_rthis.configuration_, _rctx, "configuration");
            _s.add(_rthis.image_blob_, solid::serialization::limit(1024 * 1024), _rctx, "image_blob");
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

    uint32_t    version_ = version;
    std::string app_id_;
    std::string unique_; //there cannot be two media with the same tag per application
    uint64_t    size_;
    std::string sha_sum_;

    CreateMediaRequest()
        : size_(0)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        solid::serialization::addVersion<CreateMediaRequest>(_s, _rthis.version_, "version");

        _s.add([&_rthis](S& _s, solid::frame::mprpc::ConnectionContext& _rctx, const char* /*_name*/) {
            _s.add(_rthis.app_id_, _rctx, "app_id");
            _s.add(_rthis.unique_, _rctx, "unique");
            _s.add(_rthis.size_, _rctx, "size");
            _s.add(_rthis.sha_sum_, _rctx, "sha_sum");
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

template <class Reg>
inline void configure_protocol(Reg _rreg)
{
    _rreg(protocol_id, 1, "InitRequest", solid::TypeToType<InitRequest>());

    _rreg(protocol_id, 2, "ListOSesRequest", solid::TypeToType<ListOSesRequest>());
    _rreg(protocol_id, 3, "ListOSesResponse", solid::TypeToType<ListOSesResponse>());
    _rreg(protocol_id, 4, "ListAppsRequest", solid::TypeToType<ListAppsRequest>());
    _rreg(protocol_id, 5, "ListAppsResponse", solid::TypeToType<ListAppsResponse>());
    _rreg(protocol_id, 6, "ListStoreRequest", solid::TypeToType<ListStoreRequest>());
    _rreg(protocol_id, 7, "ListStoreResponse", solid::TypeToType<ListStoreResponse>());

    _rreg(protocol_id, 8, "FetchStoreRequest", solid::TypeToType<FetchStoreRequest>());
    _rreg(protocol_id, 9, "FetchStoreResponse", solid::TypeToType<FetchStoreResponse>());

    _rreg(protocol_id, 10, "CreateBuildRequest", solid::TypeToType<CreateBuildRequest>());
    _rreg(protocol_id, 11, "CreateMediaRequest", solid::TypeToType<CreateMediaRequest>());
    _rreg(protocol_id, 12, "UploadRequest", solid::TypeToType<UploadRequest>());

    _rreg(protocol_id, 13, "FetchAppRequest", solid::TypeToType<FetchAppRequest>());
    _rreg(protocol_id, 14, "FetchAppResponse", solid::TypeToType<FetchAppResponse>());
    _rreg(protocol_id, 15, "ChangeAppItemStateRequest", solid::TypeToType<ChangeAppItemStateRequest>());

    _rreg(protocol_id, 16, "FetchBuildRequest", solid::TypeToType<FetchBuildRequest>());
    _rreg(protocol_id, 17, "FetchBuildResponse", solid::TypeToType<FetchBuildResponse>());
    _rreg(protocol_id, 18, "FetchBuildConfigurationRequest", solid::TypeToType<FetchBuildConfigurationRequest>());
    _rreg(protocol_id, 19, "FetchBuildConfigurationResponse", solid::TypeToType<FetchBuildConfigurationResponse>());

    _rreg(protocol_id, 20, "FetchBuildUpdatesRequest", solid::TypeToType<FetchBuildUpdatesRequest>());
    _rreg(protocol_id, 21, "FetchBuildUpdatesResponse", solid::TypeToType<FetchBuildUpdatesResponse>());
    
    _rreg(protocol_id, 22, "CreateAppRequest", solid::TypeToType<CreateAppRequest>());
}
} //namespace main
} //namespace front
} //namespace ola
