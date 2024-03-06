#include "Http.h"
#include "Request.h"
#include "Downloader.h"

namespace XEngine {
    SafeQueue::SpscQueue<iHttpAction*, 10240, 10240> g_RequestQueue;
    SafeQueue::SpscQueue<iHttpAction*, 10240, 10240> g_ResponseQueue;

    XPool<XBuffer<>> g_XBufferPool;

    iHttp* Http::GetInstance() {
        static Http s_Instance;
        return &s_Instance;
    }

    bool Http::Initialize(Api::iEngine* const engine) {
        int httpThreadCount = 4;
        if (engine->GetLaunchParameter("http_thread_count")) {
            int count = SafeString::StringToInt(engine->GetLaunchParameter("http_thread_count"));
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
        iHttpAction* action;
        while (g_ResponseQueue.Pull(action)) {
            action->Response();
        }
    }

    Api::iHttpRequest* Http::CreateHttpRequest(const Api::fHttpResponse reponse) {
        return Request::Create(reponse);
    }

    void Http::DownloadFile(const std::string& url, const std::string& savePath, const Api::fDownloadCallback callback, const int resumeOffset, const bool resume) {
        g_RequestQueue.Push(Downloader::Create(callback, url, savePath, resumeOffset, resume));
    }

    void Http::Run(void* context) {
        CURL* handle = curl_easy_init();
        XASSERT(handle, "wtf");
        while (!IsClose()) {
            iHttpAction* action;
            if (g_RequestQueue.Pull(action)) {
                curl_easy_reset(handle);
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
                action->Deal(handle);
                g_ResponseQueue.Push(action);
            }
            else {
                SafeSystem::Time::MillisecondSleep(100);
            }
        }
        curl_easy_cleanup(handle);
    }
}
