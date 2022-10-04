#ifndef __Fbxer_h__
#define __Fbxer_h__

#include "Header.h"

namespace XEngine {
    class Fbxer : public iFbxer {
    public:
        virtual ~Fbxer() {}

        static iFbxer* GetInstance();

        // 通过 iFbxer 继承
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;

        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void FixedUpdate(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

    private:

    };
}

#endif //__Fbxer_h__
