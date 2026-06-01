#include "Memory.h"
#include "MMap.h"
#include "ShareMemory.h"

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

    void Memory::CloseMMap(Api::iMMap* mmap) {
        MMap::Release(static_cast<MMap*>(mmap));
    }

    Api::iShareMemory* Memory::CreateShareMemory(const std::string& name, const UInt64 size) {
        return ShareMemory::Create(name, size);
    }

    Api::iShareMemory* Memory::LoadShareMemory(const std::string& name) {
        return ShareMemory::Load(name);
    }

    void Memory::ReleaseShareMemory(Api::iShareMemory* ism) {
        ShareMemory::Release(static_cast<ShareMemory*>(ism));
    }
}
