#include "ShareMemory.h"

#include <limits>

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
    namespace {
        constexpr UInt64 kShareMemoryMagic = 0x58454E4753484D31ULL;
        constexpr UInt64 kShareMemoryVersion = 1;

        struct ShareMemoryHeader {
            UInt64 _Magic;
            UInt64 _Version;
            UInt64 _Size;
            UInt64 _Reserved0;
            UInt64 _Reserved1;
            UInt64 _Reserved2;
            UInt64 _Reserved3;
            UInt64 _Reserved4;
        };

        UInt64 CalcMappedSize(const UInt64 size) {
            if (size > (std::numeric_limits<UInt64>::max)() - sizeof(ShareMemoryHeader)) {
                return 0;
            }

            return static_cast<UInt64>(sizeof(ShareMemoryHeader)) + size;
        }

        ShareMemoryHeader* ToHeader(void* mappingAddress) {
            return static_cast<ShareMemoryHeader*>(mappingAddress);
        }

        const ShareMemoryHeader* ToHeader(const void* mappingAddress) {
            return static_cast<const ShareMemoryHeader*>(mappingAddress);
        }

        void* ToPayload(void* mappingAddress) {
            return static_cast<unsigned char*>(mappingAddress) + sizeof(ShareMemoryHeader);
        }

        bool IsValidHeader(const ShareMemoryHeader* header) {
            if (nullptr == header) {
                return false;
            }

            if (header->_Magic != kShareMemoryMagic || header->_Version != kShareMemoryVersion) {
                return false;
            }

            return CalcMappedSize(header->_Size) != 0;
        }

        void InitHeader(void* mappingAddress, const UInt64 size) {
            ShareMemoryHeader* header = ToHeader(mappingAddress);
            header->_Magic = kShareMemoryMagic;
            header->_Version = kShareMemoryVersion;
            header->_Size = size;
            header->_Reserved0 = 0;
            header->_Reserved1 = 0;
            header->_Reserved2 = 0;
            header->_Reserved3 = 0;
            header->_Reserved4 = 0;
        }

#ifdef Linux
        std::string NormalizeShareMemoryName(const std::string& name) {
            if (name.empty()) {
                return "/";
            }

            std::string normalized = name;
            for (char& ch : normalized) {
                if (ch == '/' || ch == '\\') {
                    ch = '_';
                }
            }

            if (normalized.front() != '/') {
                normalized.insert(normalized.begin(), '/');
            }

            return normalized;
        }
#endif //Linux
    }

    ShareMemory* ShareMemory::Create(const std::string& name, const UInt64 size) {
        if (name.empty() || size == 0) {
            return nullptr;
        }

        UInt64 mappedSize = CalcMappedSize(size);
        if (mappedSize == 0) {
            return nullptr;
        }

#ifdef WIN32
        const DWORD protect = PAGE_READWRITE;
        const DWORD sizeHigh = static_cast<DWORD>(mappedSize >> 32);
        const DWORD sizeLow = static_cast<DWORD>(mappedSize & 0xffffffffULL);

        SetLastError(0);
        HANDLE mappingHandle = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, protect, sizeHigh, sizeLow, name.c_str());
        if (nullptr == mappingHandle) {
            return nullptr;
        }

        const bool alreadyExists = GetLastError() == ERROR_ALREADY_EXISTS;
        void* mappingAddress = MapViewOfFile(mappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (nullptr == mappingAddress) {
            CloseHandle(mappingHandle);
            return nullptr;
        }

        if (alreadyExists) {
            const ShareMemoryHeader* header = ToHeader(mappingAddress);
            if (!IsValidHeader(header)) {
                UnmapViewOfFile(mappingAddress);
                CloseHandle(mappingHandle);
                return nullptr;
            }

            const UInt64 logicalSize = header->_Size;
            mappedSize = CalcMappedSize(logicalSize);
            return txnew ShareMemory(name, mappingAddress, ToPayload(mappingAddress), logicalSize, mappedSize, reinterpret_cast<uintptr_t>(mappingHandle));
        }

        InitHeader(mappingAddress, size);
        return txnew ShareMemory(name, mappingAddress, ToPayload(mappingAddress), size, mappedSize, reinterpret_cast<uintptr_t>(mappingHandle));
#endif //WIN32

#ifdef Linux
        const std::string normalizedName = NormalizeShareMemoryName(name);

        bool created = false;
        int fd = shm_open(normalizedName.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
        if (fd >= 0) {
            created = true;
            if (0 != ftruncate(fd, static_cast<off_t>(mappedSize))) {
                close(fd);
                shm_unlink(normalizedName.c_str());
                return nullptr;
            }
        }
        else if (errno == EEXIST) {
            fd = shm_open(normalizedName.c_str(), O_RDWR, 0666);
            if (fd < 0) {
                return nullptr;
            }
        }
        else {
            return nullptr;
        }

        struct stat st;
        if (0 != fstat(fd, &st) || st.st_size < static_cast<off_t>(sizeof(ShareMemoryHeader))) {
            close(fd);
            if (created) {
                shm_unlink(normalizedName.c_str());
            }
            return nullptr;
        }

        mappedSize = static_cast<UInt64>(st.st_size);
        void* mappingAddress = mmap(nullptr, static_cast<size_t>(mappedSize), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (MAP_FAILED == mappingAddress) {
            close(fd);
            if (created) {
                shm_unlink(normalizedName.c_str());
            }
            return nullptr;
        }

        if (created) {
            InitHeader(mappingAddress, size);
            return txnew ShareMemory(name, mappingAddress, ToPayload(mappingAddress), size, CalcMappedSize(size), static_cast<uintptr_t>(fd));
        }

        const ShareMemoryHeader* header = ToHeader(mappingAddress);
        if (!IsValidHeader(header)) {
            munmap(mappingAddress, static_cast<size_t>(mappedSize));
            close(fd);
            return nullptr;
        }

        const UInt64 logicalSize = header->_Size;
        return txnew ShareMemory(name, mappingAddress, ToPayload(mappingAddress), logicalSize, mappedSize, static_cast<uintptr_t>(fd));
#endif //Linux

        return nullptr;
    }

    ShareMemory* ShareMemory::Load(const std::string& name) {
        if (name.empty()) {
            return nullptr;
        }

#ifdef WIN32
        HANDLE mappingHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, name.c_str());
        if (nullptr == mappingHandle) {
            return nullptr;
        }

        void* mappingAddress = MapViewOfFile(mappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (nullptr == mappingAddress) {
            CloseHandle(mappingHandle);
            return nullptr;
        }

        const ShareMemoryHeader* header = ToHeader(mappingAddress);
        if (!IsValidHeader(header)) {
            UnmapViewOfFile(mappingAddress);
            CloseHandle(mappingHandle);
            return nullptr;
        }

        const UInt64 logicalSize = header->_Size;
        const UInt64 mappedSize = CalcMappedSize(logicalSize);
        return txnew ShareMemory(name, mappingAddress, ToPayload(mappingAddress), logicalSize, mappedSize, reinterpret_cast<uintptr_t>(mappingHandle));
#endif //WIN32

#ifdef Linux
        const std::string normalizedName = NormalizeShareMemoryName(name);

        const int fd = shm_open(normalizedName.c_str(), O_RDWR, 0666);
        if (fd < 0) {
            return nullptr;
        }

        struct stat st;
        if (0 != fstat(fd, &st) || st.st_size < static_cast<off_t>(sizeof(ShareMemoryHeader))) {
            close(fd);
            return nullptr;
        }

        const UInt64 mappedSize = static_cast<UInt64>(st.st_size);
        void* mappingAddress = mmap(nullptr, static_cast<size_t>(mappedSize), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (MAP_FAILED == mappingAddress) {
            close(fd);
            return nullptr;
        }

        const ShareMemoryHeader* header = ToHeader(mappingAddress);
        if (!IsValidHeader(header)) {
            munmap(mappingAddress, static_cast<size_t>(mappedSize));
            close(fd);
            return nullptr;
        }

        return txnew ShareMemory(name, mappingAddress, ToPayload(mappingAddress), header->_Size, mappedSize, static_cast<uintptr_t>(fd));
#endif //Linux

        return nullptr;
    }

    void ShareMemory::Release(ShareMemory* sm) {
        if (nullptr == sm) {
            return;
        }

#ifdef WIN32
        if (sm->_MappingAddress) {
            UnmapViewOfFile(sm->_MappingAddress);
        }

        HANDLE mappingHandle = reinterpret_cast<HANDLE>(sm->_MappingHandle);
        if (mappingHandle && mappingHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(mappingHandle);
        }
#endif //WIN32

#ifdef Linux
        if (sm->_MappingAddress && sm->_MappedSize > 0) {
            munmap(sm->_MappingAddress, static_cast<size_t>(sm->_MappedSize));
        }

        const int fd = static_cast<int>(sm->_MappingHandle);
        if (fd >= 0) {
            close(fd);
        }
#endif //Linux

        txdel sm;
    }
}
