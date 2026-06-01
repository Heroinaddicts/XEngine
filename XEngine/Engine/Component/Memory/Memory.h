#ifndef __Memory_h__
#define __Memory_h__

#include "Header.h"

namespace XEngine {
    class Memory : public iMemory {
    public:
        virtual ~Memory() {}

        static Memory* GetInstance();

        // 通过 iMemory 继承
        bool Initialize(Api::iEngine* const engine) override;
        bool Launch(Api::iEngine* const engine) override;
        void Release(Api::iEngine* const engine) override;
        void EarlyUpdate(Api::iEngine* const engine) override;
        void Update(Api::iEngine* const engine) override;
        void LaterUpdate(Api::iEngine* const engine) override;
        Api::iMMap* CreateMMap(const std::string& file, const Api::eAccess a) override;
        void Close(Api::iMMap* mmap) override;
    };
}

#endif //__Memory_h__
