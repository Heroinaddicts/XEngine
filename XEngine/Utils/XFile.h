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

        __forceinline bool Open(const std::string& path, const std::string& model = "r+") {
            _file = fopen(path.c_str(), model.c_str());
            if (nullptr == _file) {
                XASSERT(false, "XFILE open %s", path.c_str());
                return false;
            }

            return true;
        }

        __forceinline void Close() {
            if (_file) {
                fclose(_file);
                _file = nullptr;
            }
        }

        __forceinline bool GetLine(char* buff, const int len) {
            return fgets(buff, len, _file) != nullptr;
        }

    private:
        FILE* _file;
    };
}

#endif //__XFile_h__
