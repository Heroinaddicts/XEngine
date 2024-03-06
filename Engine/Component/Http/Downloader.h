#ifndef __Downloader_h__
#define __Downloader_h__

#include "Header.h"

namespace XEngine {
	class Downloader : public iHttpAction{
	public:
		virtual ~Downloader() {}

		const std::string _Url;
		const std::string _SavePath;

		static Downloader* Create(const Api::fDownloadCallback& fun, const std::string& url, const std::string& path, const int resumeOffset, const bool resume);
		static void Release(Downloader* const downloader);

	private:
		static int OnHttpDataWrite(void* data, size_t size, size_t count, void* context);

	private:
		friend XPool<Downloader>;
		Downloader(const Api::fDownloadCallback & fun, const std::string & url, const std::string & path, const int resumeOffset, const bool resume) : _Callback(fun), _Url(url), _SavePath(path), _ResumeOffset(resumeOffset), _Resume(resume) {}

		const Api::fDownloadCallback const _Callback;
		// Í¨¹ý iHttpAction ¼Ì³Ð
		void Deal(CURL* const curl) override;
		void Response() override;

		const bool _Resume;
		int _ResumeOffset;

		FILE* _DownloadFile;
	};
}

#endif //__Downloader_h__
