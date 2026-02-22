#include "Request.h"
#include "Engine.h"
#include "CUrl/curl.h"

namespace XEngine {
    XPool<Request> Request::s_Pool;

    int Request::OnHttpHeaderWrite(void* data, size_t size, size_t count, void* context) {
        if (nullptr == data) {
            return -1;
        }

        Request* request = (Request*)context;
        request->_Header.Write(data, size * count, __FILE__, __LINE__);
        return size * count;
    }

    int Request::OnHttpDataWrite(void* data, size_t size, size_t count, void* context) {
        if (nullptr == data) {
            return -1;
        }

        Request* request = (Request*)context;
        request->_ResponseData.Write(data, size * count, __FILE__, __LINE__);
        return size * count;
    }

    void Request::AppendHeader(const std::string& content) {
        _AppendHeader.push_back(content);
    }

    void Request::Deal(const std::string& url, const Api::eHttpRequestType type) {
        if (_Curl == nullptr) {
            _Curl = curl_easy_init();
        }

        curl_easy_setopt(_Curl, CURLOPT_URL, url.c_str());

        //curl_easy_setopt(_Curl, CURLOPT_SSL_VERIFYPEER, 1L);
        //curl_easy_setopt(_Curl, CURLOPT_SSL_VERIFYHOST, 2L);

        curl_easy_setopt(_Curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(_Curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(_Curl, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(_Curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(_Curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(_Curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(_Curl, CURLOPT_HEADERFUNCTION, Request::OnHttpHeaderWrite);
        curl_easy_setopt(_Curl, CURLOPT_WRITEHEADER, (void*)this);
        curl_easy_setopt(_Curl, CURLOPT_WRITEFUNCTION, Request::OnHttpDataWrite);
        curl_easy_setopt(_Curl, CURLOPT_WRITEDATA, (void*)this);

        switch (type) {
        case Api::eHttpRequestType::GET: {
            curl_easy_setopt(_Curl, CURLOPT_POST, 0L);
            break;
        }
        case Api::eHttpRequestType::POST: {
            curl_easy_setopt(_Curl, CURLOPT_POST, 1L);
            break;
        }
        }

        if (!_PostFields.empty()) {
            curl_easy_setopt(_Curl, CURLOPT_POSTFIELDS, _PostFields.c_str());
        }

        struct curl_slist* headers = nullptr;
        if (_KeepLive) {
            headers = curl_slist_append(headers, "Connection: keep-alive");
            curl_easy_setopt(_Curl, CURLOPT_FORBID_REUSE, 0L);
        }

        for (auto itor = _AppendHeader.begin(); itor != _AppendHeader.end(); itor++) {
            headers = curl_slist_append(headers, itor->c_str());
        }
        if (headers) {
            curl_easy_setopt(_Curl, CURLOPT_HTTPHEADER, headers);
        }

        curl_mime* mime = nullptr;
        if (!_PostFileMap.empty() || !_ParamterMap.empty()) {
            mime = curl_mime_init(_Curl);
            for (auto itor = _PostFileMap.cbegin(); itor != _PostFileMap.cend(); itor++) {
                curl_mimepart* part = curl_mime_addpart(mime);
                curl_mime_name(part, itor->first.c_str());
                curl_mime_filedata(part, itor->second.c_str());
            }

            for (auto itor = _ParamterMap.cbegin(); itor != _ParamterMap.cend(); itor++) {
                curl_mimepart* part = curl_mime_addpart(mime);
                curl_mime_name(part, itor->first.c_str());
                curl_mime_data(part, itor->second.c_str(), itor->second.length());
            }

            curl_easy_setopt(_Curl, CURLOPT_MIMEPOST, mime);
        }

        Int64 tick = SafeSystem::Time::GetMilliSecond();
        _Code = curl_easy_perform(_Curl);
        DEBUG(g_Engine, "Http Request use tick %lld", SafeSystem::Time::GetMilliSecond() - tick);

        if (_Code == CURLcode::CURLE_OK) {
            std::string headerStr;
            _Header.Read(
                [&](const void* const data, const UInt32 size) -> UInt32 {
                    if (data) {
                        headerStr.append((char*)data, size);
                    }
                    return size;
                }
            );

            std::vector<std::string> headers;
            SafeString::Split(headerStr, "\r\n", headers);

            for (int i = 0; i < headers.size(); i++) {
                size_t pos = headers[i].find(':'); // 只查找第一个冒号
                if (pos != std::string::npos) {
                    std::string key = headers[i].substr(0, pos);
                    std::string value = headers[i].substr(pos + 1); // 后面可能还有冒号，但属于 value

                    // 去除 value 前导空格
                    value.erase(0, value.find_first_not_of(" \t"));

                    _HeaderMap[key] = value;
                }
                else {
                    _HeaderMap[headers[i]] = "";
                }
            }
        }

        if (mime) {
            curl_mime_free(mime);
        }

        if (headers) {
            curl_slist_free_all(headers);
        }

        curl_easy_reset(_Curl);
    }

    void Request::Response(const Api::fHttpResponse response) {
        if (response) {
            response(_Code == CURLcode::CURLE_OK, _HeaderMap, _ResponseData);
            _ParamterMap.clear();
            _PostFileMap.clear();
            _PostFields.clear();
            _AppendHeader.clear();
        }
    }
}
