#pragma once

#include "ola/common/ola_front_protocol_core.hpp"
#include <limits>

#include <deque>
#include <fstream>
#include <sstream>

namespace ola {
namespace front {
namespace main {

constexpr uint8_t protocol_id = 2;

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
    Version          main_version_;
    core::Version    core_version_;
    utility::Version utility_version_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.main_version_, _rctx, 1, "main_version");
        _r.add(
            [&_rthis](Reflector& _r, Context& _rctx) {
                if (_rthis.main_version_.init_request_ == Version::init_request) {
                    _r.add(_rthis.core_version_, _rctx, 3, "core_version");
                    _r.add(_rthis.utility_version_, _rctx, 4, "utility_version");
                }
            },
            _rctx);
    }
};

struct ListOSesRequest : solid::frame::mprpc::Message {
    SOLID_REFLECT_V1(_r, _rthis, _rctx) {}
};

struct ListOSesResponse : solid::frame::mprpc::Message {
    uint32_t                 error_ = -1;
    std::string              message_;
    std::vector<std::string> osvec_;

    ListOSesResponse() {}

    ListOSesResponse(const ListOSesRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "error");
        _r.add(_rthis.message_, _rctx, 2, "message");
        _r.add(_rthis.osvec_, _rctx, 3, "osvec");
    }
};

struct ListAppsRequest : solid::frame::mprpc::Message {
    uint8_t choice_; // o - owned applications
        // a - aquired applications
        // A - all applications

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.choice_, _rctx, 1, "choice");
    }
};

struct ListAppsResponse : solid::frame::mprpc::Message {
    using AppVectorT = std::vector<utility::ApplicationListItem>;

    uint32_t    error_ = -1;
    std::string message_;
    AppVectorT  app_vec_;

    ListAppsResponse() {}

    ListAppsResponse(const ListAppsRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "error");
        _r.add(_rthis.message_, _rctx, 2, "message");
        _r.add(_rthis.app_vec_, _rctx, 3, "app_vec");
    }
};

struct FetchBuildUpdatesRequest : solid::frame::mprpc::Message {
    using StringPairT = std::pair<std::string, std::string>;

    std::string              lang_;
    std::string              os_id_;
    std::vector<StringPairT> app_id_vec_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.lang_, _rctx, 1, "lang");
        _r.add(_rthis.os_id_, _rctx, 2, "os_id");
        _r.add(_rthis.app_id_vec_, _rctx, 3, "app_id_vec");
    }
};

struct FetchBuildUpdatesResponse : solid::frame::mprpc::Message {
    uint32_t    error_ = -1;
    std::string message_;
    std::vector<std::pair<std::string, std::string>>
        app_vec_; // first means app_unique, second means build_unique

    FetchBuildUpdatesResponse() {}

    FetchBuildUpdatesResponse(const FetchBuildUpdatesRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "error");
        _r.add(_rthis.message_, _rctx, 2, "message");
        _r.add(_rthis.app_vec_, _rctx, 3, "app_vec");
    }
};

struct ListStoreRequest : solid::frame::mprpc::Message {
    std::string storage_id_;
    std::string path_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.storage_id_, _rctx, 1, "storage_id");
        _r.add(_rthis.path_, _rctx, 2, "path");
    }
};

struct ListStoreResponse : solid::frame::mprpc::Message {
    uint32_t                                error_ = -1;
    std::string                             message_;
    std::deque<ola::utility::ListStoreNode> node_dq_;
    uint32_t                                compress_chunk_capacity_ = 0;
    uint8_t                                 compress_algorithm_type_ = 0;

    ListStoreResponse() {}

    ListStoreResponse(const ListStoreRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "error");
        _r.add(_rthis.message_, _rctx, 2, "message");
        _r.add(_rthis.node_dq_, _rctx, 3, "node_dq");
        _r.add(_rthis.compress_chunk_capacity_, _rctx, 4, "compress_chunk_capacity");
        _r.add(_rthis.compress_algorithm_type_, _rctx, 5, "compress_algorithm_type");
    }
};

struct FetchStoreRequest : solid::frame::mprpc::Message {
    std::string storage_id_;
    std::string path_;
    uint32_t    chunk_index_  = 0;
    uint32_t    chunk_offset_ = 0;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.storage_id_, _rctx, 1, "storage_id");
        _r.add(_rthis.path_, _rctx, 2, "path");
        _r.add(_rthis.chunk_index_, _rctx, 5, "chunk_index");
        _r.add(_rthis.chunk_offset_, _rctx, 6, "chunk_offset");
    }
};

struct FetchStoreResponse : solid::frame::mprpc::Message {
    uint32_t                        error_ = -1;
    std::string                     message_;
    mutable std::stringstream       ioss_;
    ola::utility::StorageFetchChunk chunk_;

    FetchStoreResponse() {}

    FetchStoreResponse(const FetchStoreRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "error");
        _r.add(_rthis.message_, _rctx, 2, "message");
        _r.add(_rthis.chunk_, _rctx, 5, "chunk");

        if constexpr (Reflector::is_const_reflector) {
            auto progress_lambda = [](Context& _rctx, std::istream& _ris,
                                       uint64_t _len, const bool _done,
                                       const size_t _index, const char* _name) {
                // NOTE: here you can use context.anyTuple for actual implementation
            };
            _r.add(static_cast<std::istream&>(_rthis.ioss_), _rctx, 4, "stream", [&progress_lambda](auto& _rmeta) {
                _rmeta.progressFunction(progress_lambda);
            }); // TODO:
        } else {
            auto progress_lambda = [](Context& _rctx, std::ostream& _ros,
                                       uint64_t _len, const bool _done,
                                       const size_t _index, const char* _name) {
                // NOTE: here you can use context.anyTuple for actual implementation
            };
            // NOTE: we need the static cast below, because ioss_ is both an istream
            // and ostream and the metadata dispatch function cannot know which one to
            // take
            _r.add(static_cast<std::ostream&>(_rthis.ioss_), _rctx, 4, "stream",
                [&progress_lambda](auto& _rmeta) {
                    _rmeta.progressFunction(progress_lambda);
                }); // TODO:
        }
    }
};

struct FetchAppRequest : solid::frame::mprpc::Message {
    std::string app_id_;
    std::string os_id_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.app_id_, _rctx, 1, "app_id");
        _r.add(_rthis.os_id_, _rctx, 2, "os_id");
    }
};

struct ChangeAppItemStateRequest : solid::frame::mprpc::Message {
    std::string                app_id_;
    std::string                os_id_;
    ola::utility::AppItemEntry item_;
    uint8_t                    new_state_ = 0;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.app_id_, _rctx, 1, "app_id");
        _r.add(_rthis.os_id_, _rctx, 2, "os_id");
        _r.add(_rthis.item_, _rctx, 3, "item");
        _r.add(_rthis.new_state_, _rctx, 4, "new_state");
    }
};

struct FetchAppResponse : solid::frame::mprpc::Message {
    using ItemEntryVectorT = std::vector<ola::utility::AppItemEntry>;

    uint32_t             error_ = -1;
    std::string          message_;
    utility::Application application_;
    ItemEntryVectorT     item_vec_;

    FetchAppResponse() {}

    FetchAppResponse(const FetchAppRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    FetchAppResponse(const ChangeAppItemStateRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "error");
        _r.add(_rthis.message_, _rctx, 2, "message");
        _r.add(_rthis.application_, _rctx, 3, "application");
        _r.add(_rthis.item_vec_, _rctx, 4, "item_vec");
    }
};

struct FetchBuildRequest : solid::frame::mprpc::Message {
    std::string app_id_;
    std::string build_id_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.app_id_, _rctx, 1, "app_id");
        _r.add(_rthis.build_id_, _rctx, 2, "build_id");
    }
};

struct FetchBuildResponse : solid::frame::mprpc::Message {
    uint32_t          error_ = -1;
    std::string       message_;
    std::string       storage_id_;
    std::vector<char> image_blob_;
    utility::Build    build_;

    FetchBuildResponse() {}

    FetchBuildResponse(const FetchBuildRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "error");
        _r.add(_rthis.message_, _rctx, 2, "message");
        _r.add(_rthis.storage_id_, _rctx, 3, "storage_id");
        _r.add(_rthis.image_blob_, _rctx, 4, "image_blob",
            [](auto& _rmeta) { _rmeta.maxSize(1024 * 1024); });
        _r.add(_rthis.build_, _rctx, 5, "build");
    }
};

struct FetchBuildConfigurationRequest : solid::frame::mprpc::Message {
    std::string                             app_id_;
    std::string                             build_id_;
    std::string                             lang_;
    std::string                             os_id_;
    ola::utility::Build::FetchOptionBitsetT fetch_options_;
    std::vector<std::string>                property_vec_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.app_id_, _rctx, 1, "app_id");
        _r.add(_rthis.build_id_, _rctx, 2, "build_id");
        _r.add(_rthis.lang_, _rctx, 3, "lang");
        _r.add(_rthis.os_id_, _rctx, 4, "os_id");
        _r.add(_rthis.fetch_options_, _rctx, 5, "fetch_options");
        _r.add(_rthis.property_vec_, _rctx, 6, "property_vec");
    }
};

struct FetchBuildConfigurationResponse : solid::frame::mprpc::Message {
    uint32_t                           error_ = -1;
    std::string                        message_;
    std::string                        app_unique_;
    std::string                        build_unique_;
    std::string                        build_storage_id_;
    std::string                        media_storage_id_;
    ola::utility::Build::Configuration configuration_;
    std::vector<char>                  image_blob_;

    FetchBuildConfigurationResponse() {}

    FetchBuildConfigurationResponse(const FetchBuildConfigurationRequest& _rreq)
        : solid::frame::mprpc::Message(_rreq)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.error_, _rctx, 1, "error");
        _r.add(_rthis.message_, _rctx, 2, "message");
        _r.add(_rthis.app_unique_, _rctx, 3, "app_unique");
        _r.add(_rthis.build_unique_, _rctx, 4, "build_unique");
        _r.add(_rthis.build_storage_id_, _rctx, 5, "build_storage_id");
        _r.add(_rthis.media_storage_id_, _rctx, 6, "media_storage_id");
        _r.add(_rthis.configuration_, _rctx, 7, "configuration");
        _r.add(_rthis.image_blob_, _rctx, 8, "image_blob",
            [](auto& _rmeta) { _rmeta.maxSize(1024 * 1024); });
    }
};

struct CreateAppRequest : solid::frame::mprpc::Message {
    utility::Application application_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.application_, _rctx, 1, "application");
    }
};

struct CreateBuildRequest : solid::frame::mprpc::Message {
    std::string       app_id_;
    std::string       unique_; // there cannot be two builds with the same tag per application
    uint64_t          size_;
    std::string       sha_sum_;
    std::vector<char> image_blob_;
    utility::Build    build_;

    CreateBuildRequest()
        : size_(0)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.app_id_, _rctx, 1, "app_id");
        _r.add(_rthis.unique_, _rctx, 2, "unique");
        _r.add(_rthis.size_, _rctx, 3, "size");
        _r.add(_rthis.sha_sum_, _rctx, 4, "sha_sum");
        _r.add(_rthis.image_blob_, _rctx, 5, "image_blob",
            [](auto& _rmeta) { _rmeta.maxSize(1024 * 1024); });
        _r.add(_rthis.build_, _rctx, 6, "build");
    }
};

struct CreateMediaRequest : solid::frame::mprpc::Message {
    std::string app_id_;
    std::string unique_; // there cannot be two media with the same tag per application
    uint64_t    size_;
    std::string sha_sum_;

    CreateMediaRequest()
        : size_(0)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.app_id_, _rctx, 1, "app_id");
        _r.add(_rthis.unique_, _rctx, 2, "unique");
        _r.add(_rthis.size_, _rctx, 3, "size");
        _r.add(_rthis.sha_sum_, _rctx, 4, "sha_sum");
    }
};

struct UploadRequest : solid::frame::mprpc::Message {
    mutable std::ifstream ifs_;
    std::ostringstream    oss_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        if constexpr (Reflector::is_const_reflector) {
            auto progress_lambda = [](Context& _rctx, std::istream& _ris,
                                       uint64_t _len, const bool _done,
                                       const size_t _index, const char* _name) {
                // NOTE: use _rctx.anyTuple for actual implementation
            };
            _r.add(_rthis.ifs_, _rctx, 1, "stream", [&progress_lambda](auto& _rmeta) {
                _rmeta.size(100 * 1024).progressFunction(progress_lambda);
            });
        } else {
            auto progress_lambda = [](Context& _rctx, std::ostream& _ros,
                                       uint64_t _len, const bool _done,
                                       const size_t _index, const char* _name) {
                // NOTE: use _rctx.anyTuple for actual implementation
            };
            _r.add(_rthis.oss_, _rctx, 1, "stream", [&progress_lambda](auto& _rmeta) {
                _rmeta.maxSize(100 * 1024).progressFunction(progress_lambda);
            });
        }
    }
};

struct AcquireAppRequest : solid::frame::mprpc::Message {
    std::string app_id_;
    bool        acquire_ = true;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.app_id_, _rctx, 1, "app_id");
        _r.add(_rthis.acquire_, _rctx, 2, "acquire");
    }
};

template <class Reg>
inline void configure_protocol(Reg _rreg)
{
    _rreg({protocol_id, 1}, "InitRequest", solid::TypeToType<InitRequest>());

    _rreg({protocol_id, 2}, "ListOSesRequest",
        solid::TypeToType<ListOSesRequest>());
    _rreg({protocol_id, 3}, "ListOSesResponse",
        solid::TypeToType<ListOSesResponse>());
    _rreg({protocol_id, 4}, "ListAppsRequest",
        solid::TypeToType<ListAppsRequest>());
    _rreg({protocol_id, 5}, "ListAppsResponse",
        solid::TypeToType<ListAppsResponse>());
    _rreg({protocol_id, 6}, "ListStoreRequest",
        solid::TypeToType<ListStoreRequest>());
    _rreg({protocol_id, 7}, "ListStoreResponse",
        solid::TypeToType<ListStoreResponse>());

    _rreg({protocol_id, 8}, "FetchStoreRequest",
        solid::TypeToType<FetchStoreRequest>());
    _rreg({protocol_id, 9}, "FetchStoreResponse",
        solid::TypeToType<FetchStoreResponse>());

    _rreg({protocol_id, 10}, "CreateBuildRequest",
        solid::TypeToType<CreateBuildRequest>());
    _rreg({protocol_id, 11}, "CreateMediaRequest",
        solid::TypeToType<CreateMediaRequest>());
    _rreg({protocol_id, 12}, "UploadRequest", solid::TypeToType<UploadRequest>());

    _rreg({protocol_id, 13}, "FetchAppRequest",
        solid::TypeToType<FetchAppRequest>());
    _rreg({protocol_id, 14}, "FetchAppResponse",
        solid::TypeToType<FetchAppResponse>());
    _rreg({protocol_id, 15}, "ChangeAppItemStateRequest",
        solid::TypeToType<ChangeAppItemStateRequest>());

    _rreg({protocol_id, 16}, "FetchBuildRequest",
        solid::TypeToType<FetchBuildRequest>());
    _rreg({protocol_id, 17}, "FetchBuildResponse",
        solid::TypeToType<FetchBuildResponse>());
    _rreg({protocol_id, 18}, "FetchBuildConfigurationRequest",
        solid::TypeToType<FetchBuildConfigurationRequest>());
    _rreg({protocol_id, 19}, "FetchBuildConfigurationResponse",
        solid::TypeToType<FetchBuildConfigurationResponse>());

    _rreg({protocol_id, 20}, "FetchBuildUpdatesRequest",
        solid::TypeToType<FetchBuildUpdatesRequest>());
    _rreg({protocol_id, 21}, "FetchBuildUpdatesResponse",
        solid::TypeToType<FetchBuildUpdatesResponse>());

    _rreg({protocol_id, 22}, "CreateAppRequest",
        solid::TypeToType<CreateAppRequest>());
    _rreg({protocol_id, 23}, "AcquireAppRequest",
        solid::TypeToType<AcquireAppRequest>());
}
} // namespace main
} // namespace front
} // namespace ola
