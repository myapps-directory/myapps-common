#pragma once

#include "cereal/cereal.hpp"
#include "solid/frame/mprpc/mprpcprotocol_serialization_v2.hpp"
#include "solid/system/cassert.hpp"
#include "solid/system/cstring.hpp"
#include "solid/system/exception.hpp"
#include <bitset>
#include <deque>
#include <functional>
#include <string>
#include <vector>

namespace ola {
namespace utility {

enum struct AccountStateE : uint8_t {
    Invalid = 0,
    Inactive,
    Validate,
    Active,
    ActiveLocked,
    ValidateLocked,
};

//NOTE: class versioning at the end of the file
struct Application {
    static constexpr uint32_t version = 1;

    std::string name_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.name_, _rctx, "name");
    }

    template <class Archive>
    void serialize(Archive& _a, const uint32_t _version)
    {
        solid_assert(version == _version);
        _a(name_);
    }

    bool operator==(const Application& _ac) const
    {
        return name_ == _ac.name_;
    }

    uint64_t computeCheck() const
    {
        return std::hash<std::string>{}(name_);
    }
};

//NOTE: class versioning at the end of the file
struct Build {
    static constexpr uint32_t version = 1;

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
        static constexpr uint32_t version = 1;

        std::string name_;
        std::string command_;
        std::string arguments_;
        std::string run_folder_;
        std::string icon_;

        SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
        {
            _s.add(_rthis.name_, _rctx, "name");
            _s.add(_rthis.command_, _rctx, "command");
            _s.add(_rthis.arguments_, _rctx, "arguments");
            _s.add(_rthis.run_folder_, _rctx, "run_folder");
            _s.add(_rthis.icon_, _rctx, "icon");
        }

        template <class Archive>
        void serialize(Archive& _a, std::uint32_t const _version)
        {
            solid_assert(_version == _version);
            _a(name_, command_, arguments_, run_folder_, icon_);
        }

        bool operator==(const Shortcut& _s) const
        {
            return name_ == _s.name_ && command_ == _s.command_ && run_folder_ == _s.run_folder_ && icon_ == _s.icon_ && arguments_ == _s.arguments_;
        }
    };

    using ShortcutVectorT = std::deque<Shortcut>;

    struct Media {
        static constexpr uint32_t version = 1;

        struct Entry {
            static constexpr uint32_t version = 1;
            std::string               thumbnail_path_;
            std::string               path_;

            Entry() {}

            Entry(const std::string& _thumbnail, const std::string& _path)
                : thumbnail_path_(_thumbnail)
                , path_(_path)
            {
            }

            SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
            {
                _s.add(_rthis.thumbnail_path_, _rctx, "thumbnail_path");
                _s.add(_rthis.path_, _rctx, "path");
            }

            template <class Archive>
            void serialize(Archive& _a, std::uint32_t const _version)
            {
                solid_assert(version == _version);
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

        SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
        {
            _s.add(_rthis.name_, _rctx, "name");
            _s.add(_rthis.entry_vec_, _rctx, "entry_vec");
        }

        template <class Archive>
        void serialize(Archive& _a, std::uint32_t const _version)
        {
            solid_assert(version == _version);
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
        static constexpr uint32_t version = 1;
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

        SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
        {
            _s.add(_rthis.name_, _rctx, "name");
            _s.add(_rthis.directory_, _rctx, "directory");
            _s.add(_rthis.flags_, _rctx, "flags");
            _s.add(_rthis.os_vec_, _rctx, "os_vec");
            _s.add(_rthis.mount_vec_, _rctx, "mount_vec");
            _s.add(_rthis.exe_vec_, _rctx, "exe_vec");
            _s.add(_rthis.shortcut_vec_, _rctx, "shortcut_vec");
            _s.add(_rthis.property_vec_, _rctx, "property_vec");
            _s.add(_rthis.media_, _rctx, "media");
        }

        template <class Archive>
        void serialize(Archive& _a, std::uint32_t const _version)
        {
            solid_assert(version == _version);
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

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.name_, _rctx, "name");
        _s.add(_rthis.tag_, _rctx, "tag");
        _s.add(_rthis.dictionary_dq_, _rctx, "dictionary_dq");
        _s.add(_rthis.property_vec_, _rctx, "property_vec");
        _s.add(_rthis.configuration_vec_, _rctx, "configuration_vec");
    }

    template <class Archive>
    void serialize(Archive& _a, std::uint32_t const _version)
    {
        solid_assert(version == _version);
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

#if 0
struct Media {
    static constexpr uint32_t version = 1;

    using StringVectorT = std::vector<std::string>;

    struct Entry {
        static constexpr uint32_t version = 1;
        std::string               thumbnail_path_;
        std::string               path_;

        Entry() {}

        Entry(const std::string& _thumbnail, const std::string& _path)
            : thumbnail_path_(_thumbnail)
            , path_(_path)
        {
        }

        SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
        {
            _s.add(_rthis.thumbnail_path_, _rctx, "thumbnail_path");
            _s.add(_rthis.path_, _rctx, "path");
        }

        template <class Archive>
        void serialize(Archive& _a, std::uint32_t const _version)
        {
            solid_assert(version == _version);
            _a(thumbnail_path_, path_);
        }
        bool operator==(const Entry& _bc) const
        {
            return thumbnail_path_ == _bc.thumbnail_path_ && path_ == _bc.path_;
        }
    };

    using EntryVectorT = std::vector<Entry>;

    struct Configuration {
        static constexpr uint32_t version = 1;
        StringVectorT             os_vec_;
        EntryVectorT              entry_vec_;

        SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
        {
            _s.add(_rthis.os_vec_, _rctx, "os_vec");
            _s.add(_rthis.entry_vec_, _rctx, "entry_vec");
        }

        template <class Archive>
        void serialize(Archive& _a, std::uint32_t const _version)
        {
            solid_assert(version == _version);
            _a(os_vec_, entry_vec_);
        }

        bool operator==(const Configuration& _c) const
        {
            return os_vec_ == _c.os_vec_ && entry_vec_ == _c.entry_vec_;
        }
    };

    using ConfigurationVectorT = std::deque<Configuration>;

    ConfigurationVectorT configuration_vec_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.configuration_vec_, _rctx, "configuration_vec");
    }

    template <class Archive>
    void serialize(Archive& _a, std::uint32_t const _version)
    {
        solid_assert(version == _version);
        _a(configuration_vec_);
    }

    bool operator==(const Media& _bc) const
    {
        return configuration_vec_ == _bc.configuration_vec_;
    }

    uint64_t computeCheck() const
    {
        return configuration_vec_.size();
    }
};
#endif

struct ListStoreNode {
    static constexpr uint32_t version = 1;

    std::string name_;
    uint64_t    size_ = 0;

    ListStoreNode() {}

    ListStoreNode(const std::string& _name, uint64_t _size = 0)
        : name_(_name)
        , size_(_size)
    {
    }

    ListStoreNode(std::string&& _name, uint64_t _size = 0)
        : name_(std::move(_name))
        , size_(_size)
    {
    }

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.name_, _rctx, "name");
        _s.add(_rthis.size_, _rctx, "size");
    }
};

struct ListApplicationItem {
    static constexpr uint32_t version = 1;
    std::string               id_;
    std::string               unique_;
    std::string               name_;

    ListApplicationItem() {}

    ListApplicationItem(const std::string& _id, const std::string& _unique, const std::string& _name = "")
        : id_(_id)
        , unique_(_unique)
        , name_(_name)
    {
    }
    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.id_, _rctx, "id");
        _s.add(_rthis.unique_, _rctx, "unique");
        _s.add(_rthis.name_, _rctx, "name");
    }
};

} //namespace utility
} //namespace ola

CEREAL_CLASS_VERSION(ola::utility::Application, ola::utility::Application::version);
CEREAL_CLASS_VERSION(ola::utility::Build, ola::utility::Build::version);
CEREAL_CLASS_VERSION(ola::utility::Build::Media, ola::utility::Build::Media::version);
CEREAL_CLASS_VERSION(ola::utility::Build::Shortcut, ola::utility::Build::Shortcut::version);
CEREAL_CLASS_VERSION(ola::utility::Build::Configuration, ola::utility::Build::Configuration::version);
//CEREAL_CLASS_VERSION(ola::utility::Media::Configuration, ola::utility::Media::Configuration::version);
CEREAL_CLASS_VERSION(ola::utility::Build::Media::Entry, ola::utility::Build::Media::Entry::version);
