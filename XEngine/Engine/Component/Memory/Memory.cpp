#include "Memory.h"
#include "MMap.h"
#include "ShareMemoryMap.h"

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

    void Memory::DestoryShareMemoryMap(const std::string& shareMemoryName, const std::string& mapName) {
        ShareMemoryMap::Destory(shareMemoryName, mapName);
    }

    Api::iShareMemoryMap* Memory::OpenOrCreateShareMemoryMapInternal(const std::string& shareMemoryName, const UInt64 shareMemorySize, const std::string& name, const UInt64 keySize, const UInt64 valueSize) {
        return ShareMemoryMap::OpenOrCreate(shareMemoryName, shareMemorySize, name, keySize, valueSize);
    }

    Api::iShareMemoryMap* Memory::LoadShareMemoryMapInternal(const std::string& shareMemoryName, const std::string& name, const UInt64 keySize, const UInt64 valueSize) {
        return ShareMemoryMap::Load(shareMemoryName, name, keySize, valueSize);
    }

    void Memory::ReleaseShareMemoryMapInternal(Api::iShareMemoryMap* map) {
        ShareMemoryMap::Release(static_cast<ShareMemoryMap*>(map));
    }
}
