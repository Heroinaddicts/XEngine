#ifndef __iRedis_h__
#define __iRedis_h__

#include "iEngineComponent.h"
#include "iRedisApi.h"


namespace XEngine {
    class iRedis : public iEngineComponent, public Api::iRedisApi {
    public:
        virtual ~iRedis() {}
    };
}

#endif //__iRedis_h__
