#include "Http.h"
#include "Request.h"
#include "Downloader.h"

namespace XEngine {
    SafeQueue::SpscQueue<HttpReq*, 10240, 10240> g_RequestQueue;
    SafeQueue::SpscQueue<HttpReq*, 10240, 10240> g_ResponseQueue;

    XPool<XBuffer> g_XBufferPool;

    iHttp* Http::GetInstance() {
        static Http s_Instance;
        return &s_Instance;
    }

    bool Http::Initialize(Api::iEngine* const engine) {
        int httpThreadCount = 4;
        if (engine->GetLaunchParameter("http_thread_count")) {
            int count = engine->GetLaunchParameterInt32("http_thread_count");
            if (count > 0) {
                httpThreadCount = count;
            }
        }

        Start(httpThreadCount);
        return true;
    }

    bool Http::Launch(Api::iEngine* const engine) {
        return true;
    }

    void Http::Release(Api::iEngine* const engine) {

    }

    void Http::EarlyUpdate(Api::iEngine* const engine) {

    }

    void Http::Update(Api::iEngine* const engine) {

    }

    void Http::LaterUpdate(Api::iEngine* const engine) {
        UInt64 tick = SafeSystem::Time::GetMicroSecond();
        HttpReq* req;
        while (g_ResponseQueue.Pull(req)) {
            req->_Action->Response(req->_Response);
            if (SafeSystem::Time::GetMicroSecond() - tick > 1000) {
                break;
            }
        }
    }

    Api::iHttpRequest* Http::CreateHttpRequest() {
        return Request::Create();
    }

    //     void Http::DownloadFile(const std::string& url, const std::string& savePath, const Api::fDownloadCallback callback, const int resumeOffset, const bool resume) {
    //         g_RequestQueue.Push(Downloader::Create(callback, url, savePath, resumeOffset, resume));
    //     }

    void Http::Run(void* context) {
        while (!IsClose()) {
            HttpReq* req;
            if (g_RequestQueue.Pull(req)) {
                req->_Action->Deal(req->_Url, req->_Type);
                g_ResponseQueue.Push(req);
            }
            else {
                SafeSystem::Time::MillisecondSleep(100);
            }
        }
    }
}
