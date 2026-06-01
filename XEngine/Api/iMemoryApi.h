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

        class iMemoryApi {
        public:
            virtual ~iMemoryApi() {}

            virtual iMMap* CreateMMap(const std::string& file, const eAccess a = eAccess::ReadOnly) = 0;
            virtual void Close(iMMap* mmap) = 0;
        };
    }
}

#endif //__iMemoryApi_h__
