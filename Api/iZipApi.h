#ifndef __iZipApi_h__
#define __iZipApi_h__

#include "MultiSys.h"
#include <string>
#include <functional>

namespace XEngine {
    namespace Api {
        class iZipApi {
        public:
            virtual ~iZipApi() {}

            typedef std::function<void(const bool success)> fUnzipCallback;
            virtual void Unzip(const std::string& file, const std::string& folder, const fUnzipCallback fun) = 0;
        };
    }
}

#endif //__iZipApi_h__
