#ifndef __iObjLoader_h__
#define __iObjLoader_h__

#include "iModule.h"

namespace XEngine {
    class X3DObj;
    class iObjLoader : public Api::iModule {
    public:
        virtual ~iObjLoader() {}

        virtual const X3DObj* Get3DObj(const std::string& paht) = 0;
    };
}

#endif //__iObjLoader_h__
