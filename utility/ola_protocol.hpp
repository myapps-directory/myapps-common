#pragma once

#include "cereal/cereal.hpp"
#include "solid/frame/mprpc/mprpcprotocol_serialization_v2.hpp"
#include "solid/system/cassert.hpp"
#include <deque>
#include <string>
#include <vector>

namespace ola {
namespace utility {

//NOTE: class versioning at the end of the file
struct Application {
    static constexpr uint32_t version = 1;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
    }

    template <class Archive>
    void serialize(Archive& _a, const uint32_t _version)
    {
        solid_assert(version == _version);
    }

    bool operator==(const Application& _ac) const
    {
        return true;
    }
};

//NOTE: class versioning at the end of the file
struct Build {
    static constexpr uint32_t version = 1;

    using StringPairVectorT = std::vector<std::pair<std::string, std::string>>;
    using StringPairDequeT  = std::deque<std::pair<std::string, std::string>>;
    using StringVectorT     = std::vector<std::string>;

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
    //NOTE: class versioning at the end of the file
    struct Configuration {
        static constexpr uint32_t version = 1;
        enum {
            HiddenDirectory = 0,
        };

        std::string       name_;
        std::string       directory_;
        uint64_t          flags_;
        StringVectorT     os_vec_;
        StringPairVectorT mount_vec_;
        StringVectorT     exe_vec_;
        ShortcutVectorT   shortcut_vec_;
        StringPairVectorT property_vec_;

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
        }

        template <class Archive>
        void serialize(Archive& _a, std::uint32_t const _version)
        {
            solid_assert(version == _version);
            _a(name_, directory_, flags_, os_vec_, mount_vec_, exe_vec_, shortcut_vec_, property_vec_);
        }

        bool operator==(const Configuration& _c) const
        {
            return name_ == _c.name_ && directory_ == _c.directory_ && flags_ == _c.flags_ && os_vec_ == _c.os_vec_ && mount_vec_ == _c.mount_vec_ && exe_vec_ == _c.exe_vec_ && shortcut_vec_ == _c.shortcut_vec_ && property_vec_ == _c.property_vec_;
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
        _a(name_, tag_, dictionary_dq_, configuration_vec_);
    }

    bool operator==(const Build& _bc) const
    {
        return name_ == _bc.name_ && tag_ == _bc.tag_ && configuration_vec_ == _bc.configuration_vec_ && property_vec_ == _bc.property_vec_ && dictionary_dq_ == _bc.dictionary_dq_;
    }
};

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

} //namespace utility
} //namespace ola

CEREAL_CLASS_VERSION(ola::utility::Application, ola::utility::Application::version);
CEREAL_CLASS_VERSION(ola::utility::Build, ola::utility::Build::version);
CEREAL_CLASS_VERSION(ola::utility::Build::Shortcut, ola::utility::Build::Shortcut::version);
CEREAL_CLASS_VERSION(ola::utility::Build::Configuration, ola::utility::Build::Configuration::version);
