#pragma once

#include "iTimeWheel.h"
#include "XPool.h"
#include "SafeSystem.h"
#include "SafeTools.h"

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

}
