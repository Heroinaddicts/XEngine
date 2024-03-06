#ifndef __iLog_h__
#define __iLog_h__

#include "iEngineComponent.h"

namespace XEngine {
    class iLog : public iEngineComponent {
    public:
        virtual ~iLog() {}

        virtual void LogAsync(const char* header, const char* content, const char* file, const int line) = 0;
        virtual void LogSync(const char* header, const char* content, const char* file, const int line) = 0;
    };
}

#endif //__iLog_h__
