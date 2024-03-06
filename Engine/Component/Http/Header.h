#pragma once

#include "iHttp.h"
#include "SafeMemory.h"
#include "SafeSystem.h"
#include "SafeString.h"
#include "SafeThread.h"
#include "SafeQueue.h"
#include "XPool.h"
#include "XLock.h"
#include "XFile.h"
#include "XBuffer.h"
#include "CUrl/curl.h"

#include <list>

namespace XEngine {
#define MAX_HTTP_DATA_LEN (64 * 1024)

    class iHttpAction {
    public:
        virtual ~iHttpAction() {}

        virtual void Deal(CURL* const curl) = 0;
        virtual void Response() = 0;

    protected:
        int _Code;
    };

	extern SafeQueue::SpscQueue<iHttpAction*, 10240, 10240> g_RequestQueue;
	extern SafeQueue::SpscQueue<iHttpAction*, 10240, 10240> g_ResponseQueue;

    extern XPool<XBuffer<>> g_XBufferPool;
}

