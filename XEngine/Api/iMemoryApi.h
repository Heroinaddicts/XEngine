#ifndef __iMemoryApi_h__
#define __iMemoryApi_h__

#include "MultiSys.h"

namespace XEngine {
    namespace Api {
        enum class eAccess {
            ReadOnly,
            ReadWrite
        };

        class iMMap {
        public:
            virtual ~iMMap() {}

            virtual const std::string& FilePath() const = 0;
            virtual const eAccess Access() const = 0;
            virtual void* Address() const = 0;
            virtual UInt64 Length() const = 0;

            virtual void Flush() const = 0;
        };

        class iShareMemory {
        public:
            virtual ~iShareMemory() {}

            virtual void* Address() const = 0;
            virtual UInt64 Size() const = 0;
            virtual const std::string& Name() const = 0;
        };

        class iMemoryApi {
        public:
            virtual ~iMemoryApi() {}

            virtual iMMap* CreateMMap(const std::string& file, const eAccess a = eAccess::ReadOnly) = 0;
            virtual void CloseMMap(iMMap* mmap) = 0;

            virtual iShareMemory* CreateShareMemory(const std::string& name, const UInt64 size) = 0;
            virtual iShareMemory* LoadShareMemory(const std::string& name) = 0;
            virtual void ReleaseShareMemory(iShareMemory* ism) = 0;
        };
    }
}

#endif //__iMemoryApi_h__
