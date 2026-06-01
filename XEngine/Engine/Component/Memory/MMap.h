#ifndef __MMap_h__
#define __MMap_h__

#include "Header.h"

namespace XEngine {
    class MMap : public Api::iMMap {
    public:
        virtual ~MMap() {}

        static MMap* Create(const std::string& path, const Api::eAccess access);
        static void Release(MMap* mmap);

        // 通过 iMMap 继承
        __forceinline const std::string& FilePath() const override {
            return _Path;
        }

        __forceinline const Api::eAccess Access() const override {
            return _Access;
        }

        __forceinline void* Address() const override {
            return _Address;
        }

        const std::string _Path;
        const Api::eAccess _Access;

    private:
        MMap(const std::string& path, const Api::eAccess access, void* address, const uintptr_t fileHandle, const uintptr_t mappingHandle, const UInt64 mappedLength)
            : _Path(path), _Access(access), _Address(address), _FileHandle(fileHandle), _MappingHandle(mappingHandle), _MappedLength(mappedLength) {
        }

    private:
        void* const _Address;
        const uintptr_t _FileHandle;
        const uintptr_t _MappingHandle;
        const UInt64 _MappedLength;
    };
}

#endif //__MMap_h__
