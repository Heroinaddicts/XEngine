#ifndef __Request_h__
#define __Request_h__

#include "Header.h"

namespace XEngine {
    class Request : public iHttpAction, public Api::iHttpRequest {
    public:
        virtual ~Request() {}

        static Request* Create() {
            return XPOOL_CREATE(s_Pool);
        }

        // 通过 iHttpRequest 继承
        __forceinline void SetKeepConnection(const bool keep) {
            _KeepLive = keep;
        }

        __forceinline void SetParameter(const std::string& key, const std::string& value) {
            _ParamterMap[key] = value;
        }

        __forceinline void SetPostFileds(const std::string& fields) override {
            _PostFields = fields;
        }

        __forceinline void PostFile(const std::string& name, const std::string& path) {
            _PostFileMap.insert(std::make_pair(name, path));
        }

        __forceinline void Do(const std::string& url, const Api::eHttpRequestType type, const Api::fHttpResponse response) {
            _HeaderMap.clear();
            _ResponseData.Clear();
            _Header.Clear();

            HttpReq* req = txnew HttpReq();
            req->_Url = url;
            req->_Action = this;
            req->_Type = type;
            req->_Response = response;
            g_RequestQueue.Push(req);
        }

        __forceinline void Clear() {
            _ResponseData.Clear();
            _Header.Clear();
            _ParamterMap.clear();
            _PostFileMap.clear();
            _PostFields.clear();
            _AppendHeader.clear();
        }

        __forceinline void Release() {
            if (_Curl) {
                curl_easy_cleanup(_Curl);
                _Curl = nullptr;
                XPOOL_RELEASE(s_Pool, this);
            }
        }

        __forceinline void SetData(const void* data, const int len) {
            _ResponseData.Write(data, len, __FILE__, __LINE__);
        }

        __forceinline const std::map<std::string, std::string>& GetPostFileMap() const {
            return _PostFileMap;
        }

        // 通过 iHttpAction 继承
        void Deal(const std::string& url, const Api::eHttpRequestType type) override;
        void Response(const Api::fHttpResponse response) override;
    private:
        friend XPool<Request>;
        Request() : _Curl(nullptr), _KeepLive(false), _ResponseData(256, 256), _Header(256, 256) {}

    private:
        std::map<std::string, std::string> _ParamterMap;
        std::map<std::string, std::string> _PostFileMap;
        std::list<std::string> _AppendHeader;
        std::string _PostFields;

        CURL* _Curl;
        bool _KeepLive;

        XBuffer _ResponseData;
        XBuffer _Header;

        std::map<std::string, std::string> _HeaderMap;

        static XPool<Request> s_Pool;

    private:
        // 通过 iHttpRequest 继承
        __forceinline const std::map<std::string, std::string>& GetParamters() const override { return _ParamterMap; }
        void AppendHeader(const std::string& content) override;

    private:
        static int OnHttpHeaderWrite(void* data, size_t size, size_t count, void* context);
        static int OnHttpDataWrite(void* data, size_t size, size_t count, void* context);
    };
}

#endif //__Request_h__
