#ifndef __XFile_h__
#define __XFile_h__

#include <string>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h> // ftruncate
#include <sys/types.h>
#endif

#include "MultiSys.h"
#include "XBuffer.h"

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
            _Path = path;
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

        __forceinline void Write(const unsigned char* const data, const int len) {
            fwrite(data, len, 1, _File);
        }

        __forceinline void Clear() {
            int fd = 0;
#ifdef _WIN32
            fd = _fileno(_File);
            _chsize(fd, 0);
#else
            fd = fileno(_File);
            ftruncate(fd, 0);
#endif
        }

        __forceinline std::string ReadToString() {
            if (_File) {
                fseek(_File, 0, SEEK_END);
                long lsize = ftell(_File);
                rewind(_File);

                std::string str;
                char* temp = (char*)txmalloc(lsize * sizeof(char));
                SafeMemory::Memset(temp, lsize * sizeof(char), 0, lsize * sizeof(char));
                int result = fread(temp, 1, lsize, _File);
                if (result <= lsize * sizeof(char)) {
                    str.append(temp, result);
                }
                txfree(temp);
                return str;
            }

            XASSERT(false, "wtf");
            return "";
        }

#define READ_SIZE 1024
        __forceinline void ReadToBuff(OUT IBuffer& buff) {
            if (_File) {
                rewind(_File);
                char temp[READ_SIZE];
                size_t readSize = fread(temp, 1, READ_SIZE, _File);
                while (readSize > 0) {
                    buff.Write(temp, readSize, __FILE__, __LINE__);
                    readSize = fread(temp, 1, READ_SIZE, _File);
                }
            }
        }

        __forceinline void Save() {
            if (_File) {
                fflush(_File);
            }
        }

        __forceinline void Close(const bool save = true) {
            if (_File) {
                if (save) {
                    Save();
                }
                fclose(_File);
                _File = nullptr;
            }
        }

    private:
        FILE* _File;
        std::string _Path;
    };
}

#endif //__XFile_h__
