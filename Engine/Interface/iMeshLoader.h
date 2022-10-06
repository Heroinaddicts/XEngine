#ifndef __iMeshLoader_h__
#define __iMeshLoader_h__

#include "iEngineComponent.h"
#include "iMeshLoaderApi.h"

namespace XEngine {
    class iMeshLoader : public iEngineComponent, public Api::iMeshLoaderApi {
    public:
        virtual ~iMeshLoader() {}

    };
}

#endif //__iMeshLoader_h__
