#ifndef __XFile_h__
#define __XFile_h__

#include <string>
#include "MultiSys.h"

namespace XEngine {
    class XFile {
    public:
        virtual ~XFile() {
            if (_File) {
                Close();
            }
        }
        XFile() : _File(nullptr) {}

        __forceinline bool Open(const std::string& path, const std::string& model = "a+") {
            _File = fopen(path.c_str(), model.c_str());
            if (nullptr == _File) {
                XASSERT(false, "XFILE open %s", path.c_str());
                return false;
            }

            return true;
        }

        __forceinline bool GetLine(char* buff, const int len) {
            return fgets(buff, len, _File) != nullptr;
        }

        XFile& operator << (const std::string& content) {
            fwrite(content.c_str(), content.size(), 1, _File);
            return *this;
        }

        __forceinline void Save() {
            if (_File) {
                fflush(_File);
            }
        }

        __forceinline void Close() {
            if (_File) {
                fclose(_File);
                _File = nullptr;
            }
        }

    private:
        FILE* _File;
    };
}

#endif //__XFile_h__
