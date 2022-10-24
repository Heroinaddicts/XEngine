#ifndef __XFile_h__
#define __XFile_h__

#include <string>
#include "MultiSys.h"

namespace XEngine {
    class XFile {
    public:
        virtual ~XFile() {
            if (_file) {
                Close();
            }
        }
        XFile() : _file(nullptr) {}

        __forceinline bool Open(const std::string& path, const std::string& model = "a+") {
            _file = fopen(path.c_str(), model.c_str());
            if (nullptr == _file) {
                XASSERT(false, "XFILE open %s", path.c_str());
                return false;
            }

            return true;
        }

        __forceinline bool GetLine(char* buff, const int len) {
            return fgets(buff, len, _file) != nullptr;
        }

        XFile& operator << (const std::string& content) {
            fwrite(content.c_str(), content.size(), 1, _file);
            return *this;
        }

        __forceinline void Save() {
            if (_file) {
                fflush(_file);
            }
        }

        __forceinline void Close() {
            if (_file) {
                fclose(_file);
                _file = nullptr;
            }
        }

    private:
        FILE* _file;
    };
}

#endif //__XFile_h__
