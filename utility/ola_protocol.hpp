#pragma once

#include "solid/frame/mprpc/mprpcprotocol_serialization_v2.hpp"
#include <deque>
#include <string>
#include <vector>

namespace ola {
namespace utility {

struct AppConfig {
    using StringPairDequeT  = std::deque<std::pair<std::string, std::string>>;
    using StringPairVectorT = std::vector<std::pair<std::string, std::string>>;

    StringPairDequeT  dictionary_dq_;
    StringPairVectorT property_vec_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.dictionary_dq_, _rctx, "dictionary_dq");
        _s.add(_rthis.property_vec_, _rctx, "property_vec");
    }

    template <class Archive>
    void serialize(Archive& _a)
    {
        _a(dictionary_dq_, property_vec_);
    }

    bool operator==(const AppConfig& _ac) const
    {
        return dictionary_dq_ == _ac.dictionary_dq_ && property_vec_ == _ac.property_vec_;
    }
};

struct BuildConfig {
    using StringPairVectorT = std::vector<std::pair<std::string, std::string>>;
    using StringPairDequeT  = std::deque<std::pair<std::string, std::string>>;
    using StringVectorT     = std::vector<std::string>;

    struct Shortcut {
        std::string name_;
        std::string command_;
        std::string run_folder_;
        std::string icon_;

        SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
        {
            _s.add(_rthis.name_, _rctx, "name");
            _s.add(_rthis.command_, _rctx, "command");
            _s.add(_rthis.run_folder_, _rctx, "run_folder");
            _s.add(_rthis.icon_, _rctx, "icon");
        }

        template <class Archive>
        void serialize(Archive& _a)
        {
            _a(name_, command_, run_folder_, icon_);
        }

        bool operator==(const Shortcut& _s) const
        {
            return name_ == _s.name_ && command_ == _s.command_ && run_folder_ == _s.run_folder_ && icon_ == _s.icon_;
        }
    };

    using ShortcutVectorT = std::deque<Shortcut>;

    struct Component {
        std::string       name_;
        std::string       directory_;
        StringVectorT     os_vec_;
        StringPairVectorT mount_vec_;
        StringVectorT     exe_vec_;
        ShortcutVectorT   shortcut_vec_;
        StringVectorT     property_vec_;

        SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
        {
            _s.add(_rthis.name_, _rctx, "name");
            _s.add(_rthis.directory_, _rctx, "directory");
            _s.add(_rthis.os_vec_, _rctx, "os_vec");
            _s.add(_rthis.mount_vec_, _rctx, "mount_vec");
            _s.add(_rthis.exe_vec_, _rctx, "exe_vec");
            _s.add(_rthis.shortcut_vec_, _rctx, "shortcut_vec");
            _s.add(_rthis.property_vec_, _rctx, "property_vec");
        }

        template <class Archive>
        void serialize(Archive& _a)
        {
            _a(name_, directory_, os_vec_, mount_vec_, exe_vec_, shortcut_vec_, property_vec_);
        }

        bool operator==(const Component& _c) const
        {
            return name_ == _c.name_ && directory_ == _c.directory_ && os_vec_ == _c.os_vec_ && mount_vec_ == _c.mount_vec_ && exe_vec_ == _c.exe_vec_ && shortcut_vec_ == _c.shortcut_vec_ && property_vec_ == _c.property_vec_;
        }
    };

    using ComponentVectorT = std::deque<Component>;

    std::string      name_;
    std::string      tag_;
    StringPairDequeT dictionary_dq_;
    StringVectorT    property_vec_;
    ComponentVectorT component_vec_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.name_, _rctx, "name");
        _s.add(_rthis.tag_, _rctx, "tag");
        _s.add(_rthis.dictionary_dq_, _rctx, "dictionary_dq");
        _s.add(_rthis.property_vec_, _rctx, "property_vec");
        _s.add(_rthis.component_vec_, _rctx, "component_vec");
    }

    template <class Archive>
    void serialize(Archive& _a)
    {
        _a(name_, tag_, dictionary_dq_, component_vec_);
    }

    bool operator==(const BuildConfig& _bc) const
    {
        return name_ == _bc.name_ && tag_ == _bc.tag_ && component_vec_ == _bc.component_vec_ && property_vec_ == _bc.property_vec_ && dictionary_dq_ == _bc.dictionary_dq_;
    }
};

struct ListStoreNode {
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
