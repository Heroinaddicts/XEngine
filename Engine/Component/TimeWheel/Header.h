#pragma once

#include "iTimeWheel.h"
#include "XPool.h"
#include "SafeSystem.h"
#include "SafeTools.h"

#include <string>

namespace XEngine {
#define MIN_INTERVAL 4
#define JIFF 4

    enum {
        tq_tvn_bits = 6,
        tq_tvr_bits = 8,
        tq_tvn_size = 1 << tq_tvn_bits,//64
        tq_tvr_size = 1 << tq_tvr_bits,//256
        tq_tvn_mask = tq_tvn_size - 1,//63
        tq_tvr_mask = tq_tvr_size - 1,//255
    };

}
