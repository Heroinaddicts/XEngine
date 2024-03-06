#ifndef __iHttpApi_h__
#define __iHttpApi_h__

#include "MultiSys.h"
#include <string>
#include <map>
#include <functional>

namespace XEngine {
    namespace Api {
        typedef std::function<void(const bool, const void* const data, const int len)> fHttpResponse;
        typedef std::function<void(const bool success, const int downloadSize, const std::string& url, const std::string& filePath)> fDownloadCallback;

        enum class eHttpRequestType {
            POST,
            GET
        };

        class iHttpRequest {
        public:
            virtual ~iHttpRequest() {}

            virtual void SetUrl(const std::string& url) = 0;
            virtual const char* GetUrl() const = 0;

            virtual void SetType(const eHttpRequestType type) = 0;
            virtual eHttpRequestType GetType() const = 0;

            virtual void AppendHeader(const std::string& content) = 0;

            virtual void SetParameter(const std::string& key, const std::string& value) = 0;
            virtual const std::map<std::string, std::string>& GetParamters() const = 0;

            virtual void SetPostFileds(const std::string& fields) = 0;
            virtual void PostFile(const std::string& name, const std::string& path) = 0;

            virtual void Do() = 0;
        };

        class iHttpApi {
        public:
            virtual ~iHttpApi() {}

            virtual iHttpRequest* CreateHttpRequest(const fHttpResponse reponse) = 0;
            virtual void DownloadFile(const std::string& url, const std::string& savePath, const fDownloadCallback callback, const int resumeOffset = 0, const bool resume = false) = 0;
        };
    }
}

#endif //__iHttpApi_h__
