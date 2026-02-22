#ifndef __iHttpApi_h__
#define __iHttpApi_h__

#include "MultiSys.h"
#include "XBuffer.h"
#include <string>
#include <map>
#include <functional>

namespace XEngine {
    namespace Api {
        typedef std::function<void(const bool, const std::map<std::string, std::string>& headers, IBuffer& buffer)> fHttpResponse;
        typedef std::function<void(const bool success, const int downloadSize, const std::string& url, const std::string& filePath)> fDownloadCallback;

        enum class eHttpRequestType {
            POST,
            GET
        };

        class iHttpRequest {
        public:
            virtual ~iHttpRequest() {}

            virtual void SetKeepConnection(const bool keep) = 0;

            virtual void AppendHeader(const std::string& content) = 0;

            virtual void SetParameter(const std::string& key, const std::string& value) = 0;
            virtual const std::map<std::string, std::string>& GetParamters() const = 0;

            virtual void SetPostFileds(const std::string& fields) = 0;
            virtual void PostFile(const std::string& name, const std::string& path) = 0;

            virtual void Do(const std::string& url, const eHttpRequestType type, const fHttpResponse reponse) = 0;

            virtual void Clear() = 0;

            virtual void Release() = 0;
        };

        class iHttpApi {
        public:
            virtual ~iHttpApi() {}

            virtual iHttpRequest* CreateHttpRequest() = 0;
        };
    }
}

#endif //__iHttpApi_h__
