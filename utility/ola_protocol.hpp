#pragma once

#include <array>
#include <bitset>
#include <deque>
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

#include "cereal/cereal.hpp"
#include "solid/frame/mprpc/mprpcmessage.hpp"
#include "solid/reflection/v1/reflection.hpp"
#include "solid/system/cassert.hpp"
#include "solid/system/cstring.hpp"
#include "solid/system/exception.hpp"

namespace ola {
namespace utility {

inline constexpr auto metadata_factory = [](const auto& _rt, auto& _rctx, const solid::reflection::v1::TypeMapBase* _ptype_map) -> auto
{
    using value_t = std::decay_t<decltype(_rt)>;
    if constexpr (std::is_enum_v<value_t>) {
        return solid::reflection::v1::metadata::Enum{};
    } else if constexpr (solid::is_shared_ptr_v<value_t> || solid::is_unique_ptr_v<value_t>) {
        return solid::reflection::v1::metadata::Pointer{_ptype_map};
    } else if constexpr (std::is_signed_v<value_t>) {
        return solid::reflection::v1::metadata::SignedInteger{std::numeric_limits<value_t>::min(), std::numeric_limits<value_t>::max()};
    } else if constexpr (std::is_unsigned_v<value_t>) {
        return solid::reflection::v1::metadata::UnsignedInteger{std::numeric_limits<value_t>::max()};
    } else if constexpr (std::is_same_v<value_t, std::string>) {
        return solid::reflection::v1::metadata::String{1024 * 4};
    } else if constexpr (solid::is_container<value_t>::value) {
        return solid::reflection::v1::metadata::Container{1024 * 4};
    } else if constexpr (solid::is_std_array_v<value_t>) {
        return solid::reflection::v1::metadata::Array{std::tuple_size_v<value_t>, std::tuple_size_v<value_t>};
    } else if constexpr (std::is_base_of_v<std::istream, value_t>) {
        return solid::reflection::v1::metadata::IStream<std::decay_t<decltype(_rctx)>>{};
    } else if constexpr (std::is_base_of_v<std::ostream, value_t>) {
        return solid::reflection::v1::metadata::OStream<std::decay_t<decltype(_rctx)>>{};
    } else {
        return solid::reflection::v1::metadata::Generic{};
    }
};

enum struct AccountStateE : uint8_t {
    Invalid = 0,
    Inactive,
    Validate,
    Active,
    ActiveLocked,
    ValidateLocked,
};

struct Version {
    static constexpr uint32_t version               = 1;
    static constexpr uint32_t application           = 1;
    static constexpr uint32_t build                 = 1;
    static constexpr uint32_t build_shortcut        = 1;
    static constexpr uint32_t build_media           = 1;
    static constexpr uint32_t build_media_entry     = 1;
    static constexpr uint32_t build_configuration   = 1;
    static constexpr uint32_t list_store_node       = 1;
    static constexpr uint32_t application_list_item = 1;
    static constexpr uint32_t app_item_entry        = 1;

    uint32_t version_               = version;
    uint32_t application_           = application;
    uint32_t build_                 = build;
    uint32_t build_shortcut_        = build_shortcut;
    uint32_t build_media_           = build_media;
    uint32_t build_media_entry_     = build_media_entry;
    uint32_t build_configuration_   = build_configuration;
    uint32_t list_store_node_       = list_store_node;
    uint32_t application_list_item_ = application_list_item;
    uint32_t app_item_entry_        = app_item_entry;

    void clear()
    {
        application_           = -1;
        build_                 = -1;
        build_shortcut_        = -1;
        build_media_           = -1;
        build_media_entry_     = -1;
        build_configuration_   = -1;
        list_store_node_       = -1;
        application_list_item_ = -1;
        app_item_entry_        = -1;
    }

    bool operator<=(const Version& _rthat) const
    {
        return version <= _rthat.version_ && application_ <= _rthat.application_ && build_ <= _rthat.build_ && build_shortcut_ <= _rthat.build_shortcut_ && build_media_ <= _rthat.build_media_ && build_media_entry_ <= _rthat.build_media_entry_ && build_configuration_ <= _rthat.build_configuration_ && list_store_node_ <= _rthat.list_store_node_ && application_list_item_ <= _rthat.application_list_item_ && app_item_entry_ <= _rthat.app_item_entry_;
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.version_, _rctx, 1, "version");
        _r.add([&_rthis](Reflector& _r, Context& _rctx) {
            if constexpr (!Reflector::is_const_reflector) {
                if (_rthis.version > Version::version) {
                    _rthis.clear();
                    return;
                }
            }
            _r.add(_rthis.application_, _rctx, 3, "application");
            _r.add(_rthis.build_, _rctx, 4, "build");
            _r.add(_rthis.build_shortcut_, _rctx, 5, "build_shortcut");
            _r.add(_rthis.build_media_, _rctx, 6, "build_media");
            _r.add(_rthis.build_media_entry_, _rctx, 7, "build_media_entry");
            _r.add(_rthis.build_configuration_, _rctx, 8, "build_configuration");
            _r.add(_rthis.list_store_node_, _rctx, 9, "list_store_node");
            _r.add(_rthis.application_list_item_, _rctx, 10, "application_list_item");
            _r.add(_rthis.app_item_entry_, _rctx, 11, "app_item_entry");
        },
            _rctx);
    }
};

constexpr Version version;

enum struct ApplicationFlagE : int8_t {
    Test = 0,
};

//NOTE: class versioning at the end of the file
struct Application {
    std::string name_;
    uint64_t    flags_ = 0;

    bool isFlagSet(const ApplicationFlagE _flag) const
    {
        return flags_ & (1ULL << static_cast<int8_t>(_flag));
    }

    void setFlag(const ApplicationFlagE _flag)
    {
        flags_ |= (1ULL << static_cast<int8_t>(_flag));
    }

    void resetFlag(const ApplicationFlagE _flag)
    {
        flags_ &= (~(1ULL << static_cast<int8_t>(_flag)));
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.name_, _rctx, 1, "name");
        _r.add(_rthis.flags_, _rctx, 2, "flags");
    }

    template <class Archive>
    void serialize(Archive& _a, const uint32_t _version)
    {
        solid_assert(Version::application == _version);
        _a(name_, flags_);
    }

    bool operator==(const Application& _ac) const
    {
        return name_ == _ac.name_ && flags_ == _ac.flags_;
    }

    uint64_t computeCheck() const
    {
        return std::hash<std::string>{}(name_) ^ flags_;
    }
};

//NOTE: class versioning at the end of the file
struct Build {
    enum struct FetchOptionsE : size_t {
        Name = 0,
        Directory,
        Flags,
        OSes,
        Mounts,
        EXEs,
        Shortcuts,
        Image,
        Media,

        FetchCount, //NOTE: add above
    };

    static constexpr size_t OptionsCount = static_cast<size_t>(FetchOptionsE::FetchCount);
    using FetchOptionBitsetT             = std::bitset<OptionsCount>;
    using StringPairVectorT              = std::vector<std::pair<std::string, std::string>>;
    using StringPairDequeT               = std::deque<std::pair<std::string, std::string>>;
    using StringVectorT                  = std::vector<std::string>;

    static void set_option(FetchOptionBitsetT& _opt_bs, const FetchOptionsE _opt)
    {
        _opt_bs.set(static_cast<size_t>(_opt));
    }

    static void reset_option(FetchOptionBitsetT& _opt_bs, const FetchOptionsE _opt)
    {
        _opt_bs.reset(static_cast<size_t>(_opt));
    }

    static bool has_option(const FetchOptionBitsetT& _opt_bs, const FetchOptionsE _opt)
    {
        return _opt_bs[static_cast<size_t>(_opt)];
    }

    //NOTE: class versioning at the end of the file
    struct Shortcut {
        std::string name_;
        std::string command_;
        std::string arguments_;
        std::string run_folder_;
        std::string icon_;

        SOLID_REFLECT_V1(_r, _rthis, _rctx)
        {
            _r.add(_rthis.name_, _rctx, 1, "name");
            _r.add(_rthis.command_, _rctx, 2, "command");
            _r.add(_rthis.arguments_, _rctx, 3, "arguments");
            _r.add(_rthis.run_folder_, _rctx, 4, "run_folder");
            _r.add(_rthis.icon_, _rctx, 5, "icon");
        }

        template <class Archive>
        void serialize(Archive& _a, std::uint32_t const _version)
        {
            solid_assert(_version == Version::build_shortcut);
            _a(name_, command_, arguments_, run_folder_, icon_);
        }

        bool operator==(const Shortcut& _s) const
        {
            return name_ == _s.name_ && command_ == _s.command_ && run_folder_ == _s.run_folder_ && icon_ == _s.icon_ && arguments_ == _s.arguments_;
        }
    };

    using ShortcutVectorT = std::deque<Shortcut>;

    struct Media {
        struct Entry {
            std::string thumbnail_path_;
            std::string path_;

            Entry() {}

            Entry(const std::string& _thumbnail, const std::string& _path)
                : thumbnail_path_(_thumbnail)
                , path_(_path)
            {
            }

            SOLID_REFLECT_V1(_r, _rthis, _rctx)
            {
                _r.add(_rthis.thumbnail_path_, _rctx, 1, "thumbnail_path");
                _r.add(_rthis.path_, _rctx, 2, "path");
            }

            template <class Archive>
            void serialize(Archive& _a, std::uint32_t const _version)
            {
                solid_assert(Version::build_media_entry == _version);
                _a(thumbnail_path_, path_);
            }
            bool operator==(const Entry& _bc) const
            {
                return thumbnail_path_ == _bc.thumbnail_path_ && path_ == _bc.path_;
            }
        };

        using EntryVectorT = std::vector<Entry>;
        std::string  name_;
        EntryVectorT entry_vec_;

        SOLID_REFLECT_V1(_r, _rthis, _rctx)
        {
            _r.add(_rthis.name_, _rctx, 1, "name");
            _r.add(_rthis.entry_vec_, _rctx, 2, "entry_vec");
        }

        template <class Archive>
        void serialize(Archive& _a, std::uint32_t const _version)
        {
            solid_assert(Version::build_media == _version);
            _a(name_, entry_vec_);
        }

        bool operator==(const Media& _bc) const
        {
            return name_ == _bc.name_ && entry_vec_ == _bc.entry_vec_;
        }

        uint64_t computeCheck() const
        {
            return name_.size() ^ entry_vec_.size();
        }
    };

    //NOTE: class versioning at the end of the file
    struct Configuration {
        enum {
            HiddenDirectory = 0,

            LastFlagId //add above
        };

        static constexpr const char* flag_names[LastFlagId] = {
            "HiddenDirectory"};

        static uint64_t flag(const char* _name)
        {
            for (size_t i = 0; i < LastFlagId; ++i) {
                if (solid::cstring::casecmp(flag_names[i], _name) == 0) {
                    return 1ULL << i;
                }
            }
            return 0;
        }

        template <class F>
        static void for_each_flag(const uint64_t _flags, F _f)
        {
            for (size_t i = 0; i < LastFlagId; ++i) {
                if ((_flags & (1ULL << i)) != 0) {
                    _f(flag_names[i]);
                }
            }
        }

        static uint64_t compute_flags(std::initializer_list<const char*> l)
        {
            uint64_t flags = 0;
            for (const auto& f : l) {
                flags |= flag(f);
            }
            return flags;
        }

        std::string       name_;
        std::string       directory_;
        uint64_t          flags_ = 0;
        StringVectorT     os_vec_;
        StringPairVectorT mount_vec_;
        StringVectorT     exe_vec_;
        ShortcutVectorT   shortcut_vec_;
        StringPairVectorT property_vec_;
        Media             media_;

        SOLID_REFLECT_V1(_r, _rthis, _rctx)
        {
            _r.add(_rthis.name_, _rctx, 1, "name");
            _r.add(_rthis.directory_, _rctx, 2, "directory");
            _r.add(_rthis.flags_, _rctx, 3, "flags");
            _r.add(_rthis.os_vec_, _rctx, 4, "os_vec");
            _r.add(_rthis.mount_vec_, _rctx, 5, "mount_vec");
            _r.add(_rthis.exe_vec_, _rctx, 6, "exe_vec");
            _r.add(_rthis.shortcut_vec_, _rctx, 7, "shortcut_vec");
            _r.add(_rthis.property_vec_, _rctx, 8, "property_vec");
            _r.add(_rthis.media_, _rctx, 9, "media");
        }

        template <class Archive>
        void serialize(Archive& _a, std::uint32_t const _version)
        {
            solid_assert(Version::build_configuration == _version);
            _a(name_, directory_, flags_, os_vec_, mount_vec_, exe_vec_, shortcut_vec_, property_vec_, media_);
        }

        bool operator==(const Configuration& _c) const
        {
            return name_ == _c.name_ && directory_ == _c.directory_ && flags_ == _c.flags_ && os_vec_ == _c.os_vec_ && mount_vec_ == _c.mount_vec_ && exe_vec_ == _c.exe_vec_ && shortcut_vec_ == _c.shortcut_vec_ && property_vec_ == _c.property_vec_ && media_ == _c.media_;
        }

        bool hasHiddenDirectoryFlag() const
        {
            return flags_ & (1 << HiddenDirectory);
        }
    };

    using ConfigurationVectorT = std::deque<Configuration>;

    std::string          name_;
    std::string          tag_;
    StringPairDequeT     dictionary_dq_;
    StringPairVectorT    property_vec_;
    ConfigurationVectorT configuration_vec_;

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.name_, _rctx, 1, "name");
        _r.add(_rthis.tag_, _rctx, 2, "tag");
        _r.add(_rthis.dictionary_dq_, _rctx, 3, "dictionary_dq");
        _r.add(_rthis.property_vec_, _rctx, 4, "property_vec");
        _r.add(_rthis.configuration_vec_, _rctx, 5, "configuration_vec");
    }

    template <class Archive>
    void serialize(Archive& _a, std::uint32_t const _version)
    {
        solid_assert(Version::build == _version);
        _a(name_, tag_, dictionary_dq_, property_vec_, configuration_vec_);
    }

    bool operator==(const Build& _bc) const
    {
        return name_ == _bc.name_ && tag_ == _bc.tag_ && configuration_vec_ == _bc.configuration_vec_ && property_vec_ == _bc.property_vec_ && dictionary_dq_ == _bc.dictionary_dq_;
    }

    uint64_t computeCheck() const
    {
        return std::hash<std::string>{}(name_) ^ std::hash<std::string>{}(tag_) ^ dictionary_dq_.size() ^ property_vec_.size() ^ configuration_vec_.size();
    }
};

enum struct AppItemStateE : uint8_t {
    Invalid = 0,
    Deleting,
    Trash,
    PrivateAlpha,
    ReviewRequest,
    ReviewStarted,
    ReviewAccepted,
    ReviewRejected,
    PublicAlpha,
    PublicBeta,
    PublicRelease,

    StateCount //Add above
};

enum struct AppItemFlagE : int8_t {
    Invalid        = -1,
    ReviewAccepted = 0,
    ReviewRejected,
};

enum struct AppItemTypeE : uint8_t {
    Build = 0,
    Media
};

constexpr const char* app_item_type_build     = "Build";
constexpr const char* app_item_type_media     = "Media";
constexpr const char* app_item_private_alpha  = "private_alpha";
constexpr const char* app_item_public_alpha   = "public_alpha";
constexpr const char* app_item_public_beta    = "public_beta";
constexpr const char* app_item_public_release = "public_release";
constexpr const char* app_item_invalid        = "none";
constexpr const char* app_item_trash          = "trash";
constexpr const char* app_item_deleting       = "deleting";

inline bool app_item_is_default_public_name(const std::string& _name)
{
    static const std::unordered_set<std::string> item_default_names{app_item_public_alpha, app_item_public_beta, app_item_public_release};
    return item_default_names.find(_name) != item_default_names.end();
}

inline bool app_item_is_default_name(const std::string& _name)
{
    static const std::unordered_set<std::string> item_default_names{app_item_private_alpha, app_item_invalid, app_item_trash};
    return app_item_is_default_public_name(_name) || item_default_names.find(_name) != item_default_names.end();
}

inline const char* app_item_type_name(const AppItemTypeE _item_type)
{
    switch (_item_type) {
    case AppItemTypeE::Build:
        return app_item_type_build;
    case AppItemTypeE::Media:
        return app_item_type_media;
    default:
        return "";
    }
}

inline const char* app_item_state_name(const AppItemStateE _state)
{
    switch (_state) {
    case AppItemStateE::Invalid:
        return app_item_invalid;
    case AppItemStateE::Deleting:
        return app_item_deleting;
    case AppItemStateE::Trash:
        return app_item_trash;
    case AppItemStateE::PrivateAlpha:
        return app_item_private_alpha;
    case AppItemStateE::ReviewRequest:
        return "review_request";
    case AppItemStateE::ReviewStarted:
        return "review_started";
    case AppItemStateE::ReviewAccepted:
        return "review_accepted";
    case AppItemStateE::ReviewRejected:
        return "review_rejected";
    case AppItemStateE::PublicAlpha:
        return app_item_public_alpha;
    case AppItemStateE::PublicBeta:
        return app_item_public_beta;
    case AppItemStateE::PublicRelease:
        return app_item_public_release;
    default:
        return "";
    }
}

inline AppItemStateE app_item_state_from_name(const char* _name)
{
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::Invalid)) == 0) {
        return AppItemStateE::Invalid;
    }
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::Deleting)) == 0) {
        return AppItemStateE::Deleting;
    }
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::Trash)) == 0) {
        return AppItemStateE::Trash;
    }
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::PrivateAlpha)) == 0) {
        return AppItemStateE::PrivateAlpha;
    }
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::ReviewRequest)) == 0) {
        return AppItemStateE::ReviewRequest;
    }
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::ReviewStarted)) == 0) {
        return AppItemStateE::ReviewStarted;
    }
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::ReviewAccepted)) == 0) {
        return AppItemStateE::ReviewAccepted;
    }
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::ReviewRejected)) == 0) {
        return AppItemStateE::ReviewRejected;
    }
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::PublicAlpha)) == 0) {
        return AppItemStateE::PublicAlpha;
    }
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::PublicBeta)) == 0) {
        return AppItemStateE::PublicBeta;
    }
    if (solid::cstring::casecmp(_name, app_item_state_name(AppItemStateE::PublicRelease)) == 0) {
        return AppItemStateE::PublicRelease;
    }
    return AppItemStateE::StateCount;
}

inline const char* app_item_flag_name(const AppItemFlagE _flag)
{
    switch (_flag) {
    case AppItemFlagE::ReviewAccepted:
        return "ReviewAccepted";
    case AppItemFlagE::ReviewRejected:
        return "ReviewRejected";
    default:
        return "";
    }
}

inline AppItemFlagE app_item_flag(const char* _name)
{
    if (solid::cstring::casecmp(_name, app_item_flag_name(AppItemFlagE::ReviewAccepted)) == 0) {
        return AppItemFlagE::ReviewAccepted;
    }
    if (solid::cstring::casecmp(_name, app_item_flag_name(AppItemFlagE::ReviewRejected)) == 0) {
        return AppItemFlagE::ReviewRejected;
    }
    return AppItemFlagE::Invalid;
}

struct AppItemEntry {
    std::string name_;

    union {
        struct {
            uint64_t state_ : 8;
            uint64_t type_ : 4;
            uint64_t flags_ : 52;
        } s_;
        uint64_t value_ = 0;
    } u_;

    AppItemEntry(std::string&& _name, const AppItemStateE _state)
        : name_(std::move(_name))
    {
        state(_state);
    }

    AppItemEntry(const uint64_t _value = 0)
    {
        u_.value_ = _value;
    }

    uint64_t flags() const
    {
        return u_.s_.flags_;
    }

    void flags(const uint64_t _flags)
    {
        u_.s_.flags_ = _flags;
    }

    void setFlag(const AppItemFlagE _flag)
    {
        flags(flags() | (1ULL << static_cast<uint8_t>(_flag)));
    }

    void resetFlag(const AppItemFlagE _flag)
    {
        flags(flags() & (~(1ULL << static_cast<uint8_t>(_flag))));
    }

    bool isFlagSet(const AppItemFlagE _flag) const
    {
        return flags() & (1ULL << static_cast<uint8_t>(_flag));
    }

    AppItemStateE state() const
    {
        return static_cast<AppItemStateE>(u_.s_.state_);
    }

    void state(AppItemStateE _state)
    {
        u_.s_.state_ = static_cast<uint8_t>(_state);
    }

    AppItemTypeE type() const
    {
        return static_cast<AppItemTypeE>(u_.s_.type_);
    }

    void type(AppItemTypeE _type)
    {
        u_.s_.type_ = static_cast<uint8_t>(_type);
    }

    uint64_t value() const
    {
        return u_.value_;
    }

    void value(const uint64_t _value)
    {
        u_.value_ = _value;
    }

    template <class Archive>
    void serialize(Archive& _a)
    {
        _a(name_, u_.value_);
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.name_, _rctx, 1, "name");
        _r.add(_rthis.u_.value_, _rctx, 2, "value");
    }
};

struct ListStoreNode {
    std::string name_;
    uint64_t    size_ = 0;
    int64_t     base_time_ = 0;

    ListStoreNode() {}

    ListStoreNode(const std::string& _name, uint64_t _size = 0, int64_t _base_time = 0)
        : name_(_name)
        , size_(_size)
        , base_time_(_base_time)
    {
    }

    ListStoreNode(std::string&& _name, uint64_t _size = 0, int64_t _base_time = 0)
        : name_(std::move(_name))
        , size_(_size)
        , base_time_(_base_time)
    {
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.name_, _rctx, 1, "name");
        _r.add(_rthis.size_, _rctx, 2, "size");
        _r.add(_rthis.base_time_, _rctx, 3, "base_time");
    }
};

enum struct AppFlagE {
    ReviewRequest = 0,
    Owned,
    Default,
};

struct ApplicationListItem {
    std::string id_;
    std::string unique_;
    std::string name_;
    uint32_t    flags_ = 0;

    ApplicationListItem() {}

    ApplicationListItem(const std::string& _id, const std::string& _unique, const std::string& _name = "")
        : id_(_id)
        , unique_(_unique)
        , name_(_name)
    {
    }

    uint32_t flags() const
    {
        return flags_;
    }

    void flags(const uint32_t _flags)
    {
        flags_ = _flags;
    }

    void setFlag(const AppFlagE _flag)
    {
        flags(flags() | (1UL << static_cast<uint8_t>(_flag)));
    }

    void resetFlag(const AppFlagE _flag)
    {
        flags(flags() & (~(1UL << static_cast<uint8_t>(_flag))));
    }

    bool isFlagSet(const AppFlagE _flag) const
    {
        return flags() & (1UL << static_cast<uint8_t>(_flag));
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.id_, _rctx, 1, "id");
        _r.add(_rthis.unique_, _rctx, 2, "unique");
        _r.add(_rthis.name_, _rctx, 3, "name");
        _r.add(_rthis.flags_, _rctx, 4, "flags");
    }
};

struct StorageFetchChunk {
    union {
        uint32_t data_ = 0;
        struct {
            uint32_t size_ : 31;
            uint32_t is_compressed_ : 1;
        };
    };
    uint32_t crc_ = 0;

    bool isCompressed() const
    {
        return is_compressed_ == 1;
    }

    SOLID_REFLECT_V1(_r, _rthis, _rctx)
    {
        _r.add(_rthis.data_, _rctx, 1, "data");
        _r.add(_rthis.crc_, _rctx, 2, "crc");
    }
};

} //namespace utility
} //namespace ola

CEREAL_CLASS_VERSION(ola::utility::Application, ola::utility::Version::application);
CEREAL_CLASS_VERSION(ola::utility::Build, ola::utility::Version::build);
CEREAL_CLASS_VERSION(ola::utility::Build::Media, ola::utility::Version::build_media);
CEREAL_CLASS_VERSION(ola::utility::Build::Shortcut, ola::utility::Version::build_shortcut);
CEREAL_CLASS_VERSION(ola::utility::Build::Configuration, ola::utility::Version::build_configuration);
CEREAL_CLASS_VERSION(ola::utility::Build::Media::Entry, ola::utility::Version::build_media_entry);
