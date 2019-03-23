#pragma once

#include "solid/frame/mprpc/mprpcprotocol_serialization_v2.hpp"
#include <string>
#include <vector>

namespace ola {
namespace utility {

struct AppConfig {
    using StringPairVectorT = std::vector<std::pair<std::string, std::string>>;

    std::string name_;
    std::string short_description_;
    std::string description_;
    StringPairVectorT   name_vec_;

    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.name_, _rctx, "name");
        _s.add(_rthis.short_description_, _rctx, "short_description");
        _s.add(_rthis.description_, _rctx, "description");
        _s.add(_rthis.name_vec_, _rctx, "name_vec");
    }

    template <class Archive>
    void serialize(Archive& _a)
    {
        _a(name_, short_description_, description_, name_vec_);
    }
};

struct BuildConfig {
    using StringPairVectorT = std::vector<std::pair<std::string, std::string>>;
    using StringVectorT     = std::vector<std::string>;
    
    struct OSShortcut{
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
    };
    
    using OSShortcutVectorT = std::vector<OSShortcut>;
    
    struct OSSpecific{
        StringVectorT       os_vec_;
        StringPairVectorT   mount_vec_;
        StringVectorT       exe_vec_;
        OSShortcutVectorT   shortcut_vec_;
        
        SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
        {
            _s.add(_rthis.os_vec_, _rctx, "os_vec");
            _s.add(_rthis.mount_vec_, _rctx, "mount_vec");
            _s.add(_rthis.exe_vec_, _rctx, "exe_vec");
            _s.add(_rthis.shortcut_vec_, _rctx, "shortcut_vec");
        }

        template <class Archive>
        void serialize(Archive& _a)
        {
            _a(os_vec_, mount_vec_, exe_vec_, shortcut_vec_);
        }
    };
    
    using OSSpecificVectorT = std::vector<OSSpecific>;
    
    std::string       name_;
    OSSpecificVectorT os_specific_vec_;
    
    SOLID_PROTOCOL_V2(_s, _rthis, _rctx, _name)
    {
        _s.add(_rthis.name_, _rctx, "name");
        _s.add(_rthis.os_specific_vec_, _rctx, "os_specific_vec");
    }

    template <class Archive>
    void serialize(Archive& _a)
    {
        _a(name_, os_specific_vec_);
    }
};

} //namespace utility
} //namespace ola
