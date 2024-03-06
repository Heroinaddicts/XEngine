#pragma once

#include "iLog.h"
#include "SafeMemory.h"
#include "SafeSystem.h"
#include "SafeString.h"
#include "SafeThread.h"
#include "SafeQueue.h"
#include "XPool.h"
#include "XLock.h"
#include "XFile.h"

#include <string>

namespace XEngine {
    class LogContent {
    public:
        const std::string _Head;
        const std::string _Content;
        const char* _File;
        const int _Line;
        const UInt64 _Tick;

        LogContent(const std::string& head, const std::string& content, const char* file, const int line, const UInt64 tick)
            : _Head(head), _Content(content), _File(file), _Line(line), _Tick(tick) {}
    };
}

