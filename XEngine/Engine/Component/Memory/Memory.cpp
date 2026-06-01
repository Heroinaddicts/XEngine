#include "Memory.h"
#include "MMap.h"

namespace XEngine {
    Memory* Memory::GetInstance() {
        static Memory* s_Instance = nullptr;
        if (nullptr == s_Instance) {
            s_Instance = txnew Memory();
        }

        return s_Instance;
    }

    bool Memory::Initialize(Api::iEngine* const engine) {
        return true;
    }

    bool Memory::Launch(Api::iEngine* const engine) {
        return true;
    }

    void Memory::Release(Api::iEngine* const engine) {
    }

    void Memory::EarlyUpdate(Api::iEngine* const engine) {
    }

    void Memory::Update(Api::iEngine* const engine) {
    }

    void Memory::LaterUpdate(Api::iEngine* const engine) {
    }

    Api::iMMap* Memory::CreateMMap(const std::string& file, const Api::eAccess a) {
        return MMap::Create(file, a);
    }

    void Memory::Close(Api::iMMap* mmap) {
        MMap::Release(static_cast<MMap*>(mmap));
    }
}
