// #include "Downloader.h"
// #include "Engine.h"
// 
// namespace XEngine {
//     static XPool<Downloader> s_Pool;
// 
//     Downloader* Downloader::Create(const Api::fDownloadCallback& fun, const std::string& url, const std::string& path, const int resumeOffset, const bool resume) {
//         return XPOOL_CREATE(s_Pool, fun, url, path, resumeOffset, resume);
//     }
// 
//     void Downloader::Release(Downloader* const downloader) {
//         XPOOL_RELEASE(s_Pool, downloader);
//     }
// 
//     int Downloader::OnHttpDataWrite(void* data, size_t size, size_t count, void* context) {
//         Downloader* downloader = (Downloader*)context;
//         int totalSize = size * count;
//         int writeSize = fwrite(data, 1, totalSize, downloader->_DownloadFile);
//         downloader->_ResumeOffset += writeSize;
//         return writeSize;
//     }
// 
//     void Downloader::Deal(CURL* const curl) {
//         // 设置下载的 URL
//         curl_easy_setopt(curl, CURLOPT_URL, _Url.c_str());
//         Int64 tick = SafeSystem::Time::GetMilliSecond();
//         _Resume == true ? _DownloadFile = fopen(_SavePath.c_str(), "ab") : _DownloadFile = fopen(_SavePath.c_str(), "wb");
//         if (_DownloadFile) {
//             if (_Resume) {
//                 fseek(_DownloadFile, 0, SEEK_END);
//                 if (ftell(_DownloadFile) != _ResumeOffset) {
//                     fclose(_DownloadFile);
//                     _DownloadFile = fopen(_SavePath.c_str(), "wb");
//                     _ResumeOffset = 0;
//                 }
//             }
// 
//             // 设置 libcurl 的写入回调函数
//             curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Downloader::OnHttpDataWrite);
//             curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
// 
//             // 设置断点续传的起始位置
//             curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, _ResumeOffset);
// 
//             // 执行文件下载
//             _Code = curl_easy_perform(curl);
//             // 关闭本地文件
//             fclose(_DownloadFile);
//         }
//         else {
//             _Code = -1;
//         }
// 
//         TRACE(g_Engine, "Download file %s use tick %lld, Code %d", _SavePath.c_str(), SafeSystem::Time::GetMilliSecond() - tick, _Code);
//     }
// 
//     void Downloader::Response() {
//         _Callback(_Code == CURLcode::CURLE_OK, _ResumeOffset, _Url, _SavePath);
// 
//         XPOOL_RELEASE(s_Pool, this);
//     }
// }
