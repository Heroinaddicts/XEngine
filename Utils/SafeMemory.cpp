#include "SafeMemory.h"


namespace XEngine {
    namespace SafeMemory {
        void* AlignedMalloc(size_t size, size_t alignment) {
#ifdef WIN32
            return _aligned_malloc(size, alignment);
#else
            return aligned_alloc(alignment, size);
#endif //WIN32
        }

        void AlignedFree(void* p) {
#ifdef WIN32
            _aligned_free(p);
#else
            free(p);
#endif //WIN32
        }

    }
}