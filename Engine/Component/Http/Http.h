#ifndef __Http_h__
#define __Http_h__

#include "Header.h"

namespace XEngine {
    class Http : public iHttp, public SafeThread {
    public:
        virtual ~Http() {}

        static iHttp* GetInstance();

        // 通过 iHttp 继承
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;
        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

        // Inherited via iHttp
		virtual Api::iHttpRequest* CreateHttpRequest(const Api::fHttpResponse reponse) override;
		virtual void DownloadFile(const std::string& url, const std::string& savePath, const Api::fDownloadCallback callback, const int resumeOffset, const bool resume) override;

    protected:
        // Inherited via SafeThread
        virtual void Run(void* context) override;

    private:
        Http() {}
    };
}

#endif //__Http_h__
