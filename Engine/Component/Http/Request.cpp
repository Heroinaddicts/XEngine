#include "Request.h"
#include "Engine.h"
#include "CUrl/curl.h"

namespace XEngine {
    XPool<Request> Request::s_Pool;

    int Request::OnHttpDataWrite(void* data, size_t size, size_t count, void* context) {
        if (nullptr == data) {
            return -1;
        }

        Request* request = (Request*)context;
        request->_ResponseData.Write(data, size * count);
        return size * count;
    }

    void Request::AppendHeader(const std::string& content) {
        _AppendHeader.push_back(content);
    }

    void Request::Deal(CURL* const handle) {
        curl_easy_setopt(handle, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(handle, CURLOPT_HEADER, 0);
        curl_easy_setopt(handle, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(handle, CURLOPT_URL, GetUrl());
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, Request::OnHttpDataWrite);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)this);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);

        switch (_Type) {
        case Api::eHttpRequestType::GET: {
            curl_easy_setopt(handle, CURLOPT_POST, 0L);
            break;
        }
        case Api::eHttpRequestType::POST: {
            curl_easy_setopt(handle, CURLOPT_POST, 1L);
            break;
        }
        }

        if (!_PostFields.empty()) {
            curl_easy_setopt(handle, CURLOPT_POSTFIELDS, _PostFields.c_str());
        }

        struct curl_slist* headers = nullptr;
        for (auto itor = _AppendHeader.begin(); itor != _AppendHeader.end(); itor++) {
            headers = curl_slist_append(headers, itor->c_str());
        }
        if (headers) {
            curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
        }

        curl_mime* mime = nullptr;
        if (!_PostFileMap.empty() || !_ParamterMap.empty()) {
            mime = curl_mime_init(handle);
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

            curl_easy_setopt(handle, CURLOPT_MIMEPOST, mime);
        }

        Int64 tick = SafeSystem::Time::GetMilliSecond();
        _Code = curl_easy_perform(handle);
        TRACE(g_Engine, "Http Request use tick %lld", SafeSystem::Time::GetMilliSecond() - tick);

        if (mime) {
            curl_mime_free(mime);
        }

        if (headers) {
            curl_slist_free_all(headers);
        }
    }

    void Request::Response() {
        if (_Response) {
            const void* p = nullptr;
            int len = 0;
            _ResponseData.Read(
                [&](const void* const data, const UInt32 size) -> UInt32 {
                    p = data;
                    len = size;
                    return size;
                }
            );

            _Response(_Code == CURLcode::CURLE_OK, p, len);
        }

        XPOOL_RELEASE(s_Pool, this);
    }
}
