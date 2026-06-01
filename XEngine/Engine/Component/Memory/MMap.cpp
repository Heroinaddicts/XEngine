#include "MMap.h"

#ifdef WIN32
#include <windows.h>
#endif //WIN32

#ifdef Linux
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif //Linux

namespace XEngine {
    MMap* MMap::Create(const std::string& path, const Api::eAccess access) {
#ifdef WIN32
        const DWORD desiredAccess = access == Api::eAccess::ReadWrite ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_READ;

        HANDLE fileHandle = CreateFileA(path.c_str(), desiredAccess, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (INVALID_HANDLE_VALUE == fileHandle) {
            return nullptr;
        }

        LARGE_INTEGER fileSize;
        if (FALSE == GetFileSizeEx(fileHandle, &fileSize) || fileSize.QuadPart <= 0) {
            CloseHandle(fileHandle);
            return nullptr;
        }

        const DWORD protect = access == Api::eAccess::ReadWrite ? PAGE_READWRITE : PAGE_READONLY;
        HANDLE mappingHandle = CreateFileMappingA(fileHandle, nullptr, protect, 0, 0, nullptr);
        if (nullptr == mappingHandle) {
            CloseHandle(fileHandle);
            return nullptr;
        }

        const DWORD mapDesiredAccess = access == Api::eAccess::ReadWrite ? FILE_MAP_WRITE : FILE_MAP_READ;
        void* address = MapViewOfFile(mappingHandle, mapDesiredAccess, 0, 0, 0);
        if (nullptr == address) {
            CloseHandle(mappingHandle);
            CloseHandle(fileHandle);
            return nullptr;
        }

        return txnew MMap(path, access, address, reinterpret_cast<uintptr_t>(fileHandle), reinterpret_cast<uintptr_t>(mappingHandle), static_cast<UInt64>(fileSize.QuadPart));
#endif //WIN32

#ifdef Linux
        const int flags = access == Api::eAccess::ReadWrite ? O_RDWR : O_RDONLY;
        int fd = open(path.c_str(), flags);
        if (fd < 0) {
            return nullptr;
        }

        struct stat st;
        if (0 != fstat(fd, &st) || st.st_size <= 0) {
            close(fd);
            return nullptr;
        }

        const int prot = access == Api::eAccess::ReadWrite ? (PROT_READ | PROT_WRITE) : PROT_READ;
        void* address = mmap(nullptr, static_cast<size_t>(st.st_size), prot, MAP_SHARED, fd, 0);
        if (MAP_FAILED == address) {
            close(fd);
            return nullptr;
        }

        return txnew MMap(path, access, address, static_cast<uintptr_t>(fd), 0, static_cast<UInt64>(st.st_size));
#endif //Linux

        return nullptr;
    }

    void MMap::Release(MMap* mmap) {
        if (nullptr == mmap) {
            return;
        }

#ifdef WIN32
        if (mmap->_Address) {
            if (mmap->_Access == Api::eAccess::ReadWrite) {
                FlushViewOfFile(mmap->_Address, 0);
            }
            UnmapViewOfFile(mmap->_Address);
        }

        HANDLE mappingHandle = reinterpret_cast<HANDLE>(mmap->_MappingHandle);
        if (mappingHandle) {
            CloseHandle(mappingHandle);
        }

        HANDLE fileHandle = reinterpret_cast<HANDLE>(mmap->_FileHandle);
        if (fileHandle && fileHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(fileHandle);
        }
#endif //WIN32

#ifdef Linux
        if (mmap->_Address && mmap->_MappedLength > 0) {
            if (mmap->_Access == Api::eAccess::ReadWrite) {
                msync(mmap->_Address, static_cast<size_t>(mmap->_MappedLength), MS_SYNC);
            }
            munmap(mmap->_Address, static_cast<size_t>(mmap->_MappedLength));
        }

        const int fd = static_cast<int>(mmap->_FileHandle);
        if (fd >= 0) {
            close(fd);
        }
#endif //Linux

        txdel mmap;
    }
}
