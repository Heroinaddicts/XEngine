#ifndef __GameObjectTemplate_h__
#define __GameObjectTemplate_h__

#include "MultiSys.h"
#include <string>

namespace GOT {
#define MAX_STRING_LEN 128

    enum class eType {
        __int__, //support 64bit
        __float__,
        __string__, //max length 128
        __bool__
    };

    struct AttrIndex {
        const eType _Type;
        const std::string _Name;
        const int16 _Index;

        const bool _Visual;
        const bool _Share;
        const bool _Save;
        const bool _Important;

        AttrIndex(const eType type, const char* name, const int16 index,
            const bool visual, const bool share, const bool save, const bool important)
            :_Type(type), _Name(name), _Index(index), _Visual(visual), _Share(share), _Save(save), _Important(important) { }
    };

    namespace Role {
        static int Index = 0;
        static const AttrIndex Name(eType::__int__, "name", 0, false, false, false, false);

        //Table
        namespace Path {
            static const char* Name = "Path";
            static const int32 Tag = 0;
            enum {
                Column_X_Float,
                Column_Y_Float,
                Column_Z_Float,
            };
        }
    }

    namespace Monster {
        static int Index = 1;
        static const AttrIndex name(eType::__int__, "name", 0, false, false, false, false);
        static const AttrIndex id(eType::__int__, "name", 0, false, false, false, false);

        //Table
        namespace Path {
            static const char* Name = "Path";
            static const int32 Tag = 0;
            enum {
                Column_X_Float,
                Column_Y_Float,
                Column_Z_Float,
            };
        }
    }
}

#endif //__GameObjectTemplate_h__

