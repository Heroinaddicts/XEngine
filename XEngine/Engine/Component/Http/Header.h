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

        virtual void Deal(const std::string& url, const Api::eHttpRequestType type) = 0;
        virtual void Response(const Api::fHttpResponse response) = 0;

    protected:
        int _Code;
    };

    struct HttpReq {
        iHttpAction* _Action;
        std::string _Url;
        Api::eHttpRequestType _Type;
        Api::fHttpResponse _Response;
    };

    extern SafeQueue::SpscQueue<HttpReq*, 10240, 10240> g_RequestQueue;
    extern SafeQueue::SpscQueue<HttpReq*, 10240, 10240> g_ResponseQueue;

    extern XPool<XBuffer> g_XBufferPool;
}

