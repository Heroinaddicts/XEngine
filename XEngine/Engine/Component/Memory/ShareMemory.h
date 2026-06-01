#ifndef __ShareMemory_h__
#define __ShareMemory_h__

#include "Header.h"

namespace XEngine {
    class ShareMemory : public Api::iShareMemory {
    public:
        virtual ~ShareMemory() {}

        static ShareMemory* Create(const std::string& name, const UInt64 size);
        static ShareMemory* Load(const std::string& name);
        static void Release(ShareMemory* sm);

        __forceinline void* Address() const override {
            return _Address;
        }

        __forceinline UInt64 Size() const override {
            return _Size;
        }

        __forceinline const std::string& Name() const override {
            return _Name;
        }

    private:
        ShareMemory(const std::string& name, void* mappingAddress, void* address, const UInt64 size, const UInt64 mappedSize, const uintptr_t mappingHandle)
            : _Name(name), _MappingAddress(mappingAddress), _Address(address), _Size(size), _MappedSize(mappedSize), _MappingHandle(mappingHandle) {
        }

    private:
        const std::string _Name;
        void* const _MappingAddress;
        void* const _Address;
        const UInt64 _Size;
        const UInt64 _MappedSize;
        const uintptr_t _MappingHandle;
    };
}

#endif //__ShareMemory_h__
