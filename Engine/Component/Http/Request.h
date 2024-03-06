#ifndef __Request_h__
#define __Request_h__

#include "Header.h"

namespace XEngine {
    class Request : public iHttpAction, public Api::iHttpRequest {
    public:
        virtual ~Request() {}

        static Request* Create(const Api::fHttpResponse const response) {
            return XPOOL_CREATE(s_Pool, response);
        }

        __forceinline void SetUrl(const std::string& url) {
            _Url = url;
        }

        __forceinline void SetType(const Api::eHttpRequestType type) {
            _Type = type;
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

        __forceinline void Do() {
            g_RequestQueue.Push(this);
        }

        __forceinline void SetData(const void* data, const int len) {
            _ResponseData.Write(data, len);
        }

        __forceinline const std::map<std::string, std::string>& GetPostFileMap() const {
            return _PostFileMap;
        }

		// 通过 iHttpAction 继承
		void Deal(CURL* const handle) override;
		void Response() override;
    private:
        friend XPool<Request>;
        Request(const Api::fHttpResponse const response) : _Type(Api::eHttpRequestType::POST), _Response(response) {}

    private:
        Api::eHttpRequestType _Type;
        std::string _Url;
        std::map<std::string, std::string> _ParamterMap;
        std::map<std::string, std::string> _PostFileMap;
        std::list<std::string> _AppendHeader;
        std::string _PostFields;

        XBuffer<> _ResponseData;
        const Api::fHttpResponse const _Response;
        static XPool<Request> s_Pool;

    private:
		// 通过 iHttpRequest 继承
        __forceinline const char* GetUrl() const override { return _Url.c_str(); }
        __forceinline Api::eHttpRequestType GetType() const override { return _Type; }
		__forceinline const std::map<std::string, std::string>& GetParamters() const override { return _ParamterMap; }
		void AppendHeader(const std::string& content) override;

    private:
        static int OnHttpDataWrite(void* data, size_t size, size_t count, void* context);
    };
}

#endif //__Request_h__
