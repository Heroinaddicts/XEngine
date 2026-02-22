#pragma once

#include "iTimeWheel.h"
#include "XPool.h"
#include "SafeSystem.h"
#include "SafeTools.h"
#include "Engine.h"

#include <string>

namespace XEngine {
#define JIFF 2

    enum {
        TQTvnBits = 6,
        TQTvrBits = 8,
        TQTvnSize = 1 << TQTvnBits,//64
        TQTvrSize = 1 << TQTvrBits,//256
        TQTvnMask = TQTvnSize - 1,//63
        TQTvrMask = TQTvrSize - 1,//255
    };

    struct TimerContext {
        const int _Id;
        const UInt64 _Context;
        TimerContext(const int id, const UInt64 context) : _Id(id), _Context(context) {}

        bool operator ==(const TimerContext& target) const {
            return _Id == target._Id && _Context == target._Context;
        }
    };

    __forceinline size_t TimerContextToSizeT(const TimerContext& context) {
        return context._Id + context._Context;
    }

    typedef std::unordered_map<TimerContext, TimeBase*, SafeTools::T_Hash<TimerContext, TimerContextToSizeT>> TIMEBASE_MAP;
    typedef std::unordered_map<Api::iTimer*, TIMEBASE_MAP> TIME_MAP;
    extern TIME_MAP g_TimerMap;

    TimeBase* CreateTimerBase(Api::iTimer* timer, const int id, const UInt64 context, int count, int interval, const char* file, const int line);
    void RemoveTimerBase(TimeBase* base, Api::iTimer* timer, const int id, const UInt64 context);
    TimeBase* FindTimerBase(Api::iTimer* timer, const int id, const UInt64 context);
}
