#include "TestMemory.h"
#include "SafeSystem.h"

#ifdef WIN32
#include <windows.h>
#include <psapi.h>
#include <vector>
#pragma comment(lib, "psapi.lib")
#endif //WIN32

Api::iEngine* g_Engine = nullptr;
static TestMemory* s_Self = nullptr;

#ifdef WIN32
namespace {
    const char* ProtectToString(DWORD protect) {
        switch (protect & 0xff) {
        case PAGE_NOACCESS: return "NOACCESS";
        case PAGE_READONLY: return "R";
        case PAGE_READWRITE: return "RW";
        case PAGE_WRITECOPY: return "WC";
        case PAGE_EXECUTE: return "X";
        case PAGE_EXECUTE_READ: return "XR";
        case PAGE_EXECUTE_READWRITE: return "XRW";
        case PAGE_EXECUTE_WRITECOPY: return "XWC";
        default: return "?";
        }
    }

    const char* TypeToString(DWORD type) {
        switch (type) {
        case MEM_IMAGE: return "IMAGE";
        case MEM_MAPPED: return "MAPPED";
        case MEM_PRIVATE: return "PRIVATE";
        default: return "UNKNOWN";
        }
    }

    void LogMMapStats(Api::iEngine* const engine, Api::iMMap* const mmap) {
        if (nullptr == mmap || nullptr == mmap->Address() || 0 == mmap->Length()) {
            return;
        }

        SYSTEM_INFO si;
        GetSystemInfo(&si);
        const UInt64 pageSize = si.dwPageSize;
        const UInt64 pageCount = (mmap->Length() + pageSize - 1) / pageSize;

        std::vector<PSAPI_WORKING_SET_EX_INFORMATION> pages(static_cast<size_t>(pageCount));
        unsigned char* base = static_cast<unsigned char*>(mmap->Address());
        for (UInt64 i = 0; i < pageCount; i++) {
            pages[static_cast<size_t>(i)].VirtualAddress = base + i * pageSize;
        }

        if (!QueryWorkingSetEx(GetCurrentProcess(), pages.data(), static_cast<DWORD>(pages.size() * sizeof(PSAPI_WORKING_SET_EX_INFORMATION)))) {
            SYSERR(engine, "QueryWorkingSetEx mmap=%s error=%u", mmap->FilePath().c_str(), GetLastError());
            return;
        }

        UInt64 residentPages = 0;
        UInt64 shareablePages = 0;
        UInt64 sharedRefPages = 0;
        UInt64 exclusiveResidentPages = 0;
        UInt64 totalShareCount = 0;
        for (const PSAPI_WORKING_SET_EX_INFORMATION& page : pages) {
            if (!page.VirtualAttributes.Valid) {
                continue;
            }

            residentPages++;
            if (page.VirtualAttributes.Shared) {
                shareablePages++;
            }

            if (page.VirtualAttributes.ShareCount > 1) {
                sharedRefPages++;
            }
            else {
                exclusiveResidentPages++;
            }
            totalShareCount += page.VirtualAttributes.ShareCount;
        }

        PROCESS_MEMORY_COUNTERS_EX pmc;
        SafeMemory::Memset(&pmc, sizeof(pmc), 0, sizeof(pmc));
        pmc.cb = sizeof(pmc);
        if (!GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
            SYSERR(engine, "GetProcessMemoryInfo error=%u", GetLastError());
            return;
        }

        MEMORY_BASIC_INFORMATION mbi;
        SafeMemory::Memset(&mbi, sizeof(mbi), 0, sizeof(mbi));
        const bool vq = 0 != VirtualQuery(mmap->Address(), &mbi, sizeof(mbi));

        IMPORTANT(engine,
            "MMapStats path=%s length=%lluMB pages=%llu resident=%llu shareable=%llu sharedRefs=%llu exclusive=%llu avgShare=%.2f ws=%lluMB private=%lluMB pf=%u type=0x%llx protect=0x%llx",
            mmap->FilePath().c_str(),
            mmap->Length() / 1024 / 1024,
            pageCount,
            residentPages,
            shareablePages,
            sharedRefPages,
            exclusiveResidentPages,
            residentPages > 0 ? (double)totalShareCount / (double)residentPages : 0.0,
            static_cast<UInt64>(pmc.WorkingSetSize / 1024 / 1024),
            static_cast<UInt64>(pmc.PrivateUsage / 1024 / 1024),
            pmc.PageFaultCount,
            vq ? static_cast<UInt64>(mbi.Type) : 0,
            vq ? static_cast<UInt64>(mbi.Protect) : 0);
    }

    void LogProcessMemoryLayout(Api::iEngine* const engine) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);

        UInt64 committedPrivate = 0;
        UInt64 committedMapped = 0;
        UInt64 committedImage = 0;
        UInt64 reservedBytes = 0;

        unsigned char* address = static_cast<unsigned char*>(si.lpMinimumApplicationAddress);
        unsigned char* maxAddress = static_cast<unsigned char*>(si.lpMaximumApplicationAddress);
        while (address < maxAddress) {
            MEMORY_BASIC_INFORMATION mbi;
            SIZE_T ret = VirtualQuery(address, &mbi, sizeof(mbi));
            if (0 == ret) {
                break;
            }

            if (mbi.State == MEM_COMMIT) {
                if (mbi.Type == MEM_PRIVATE) {
                    committedPrivate += static_cast<UInt64>(mbi.RegionSize);
                }
                else if (mbi.Type == MEM_MAPPED) {
                    committedMapped += static_cast<UInt64>(mbi.RegionSize);
                }
                else if (mbi.Type == MEM_IMAGE) {
                    committedImage += static_cast<UInt64>(mbi.RegionSize);
                }

                if (mbi.RegionSize >= 16 * 1024 * 1024) {
                    IMPORTANT(engine,
                        "Region base=%p size=%lluMB state=COMMIT type=%s protect=%s allocProtect=0x%llx",
                        mbi.BaseAddress,
                        static_cast<UInt64>(mbi.RegionSize / 1024 / 1024),
                        TypeToString(mbi.Type),
                        ProtectToString(mbi.Protect),
                        static_cast<UInt64>(mbi.AllocationProtect));
                }
            }
            else if (mbi.State == MEM_RESERVE) {
                reservedBytes += static_cast<UInt64>(mbi.RegionSize);
            }

            address += mbi.RegionSize;
        }

        IMPORTANT(engine,
            "ProcessLayout private=%lluMB mapped=%lluMB image=%lluMB reserved=%lluMB",
            committedPrivate / 1024 / 1024,
            committedMapped / 1024 / 1024,
            committedImage / 1024 / 1024,
            reservedBytes / 1024 / 1024);
    }
}
#endif //WIN32

bool TestMemory::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    return true;
}

bool TestMemory::Launch(Api::iEngine* const engine) {
    Api::iMemoryApi* mapi = engine->GetMemoryApi();

    _MMap = mapi->CreateMMap("E:/Game/World of Warcraft/Data/data/data.003", Api::eAccess::ReadOnly);
    _LastStatTick = 0;

    return _MMap != nullptr;
}

bool TestMemory::Destroy(Api::iEngine* const engine) {
    if (_MMap) {
        engine->GetMemoryApi()->Close(_MMap);
        _MMap = nullptr;
    }

    return true;
}

void TestMemory::Update(Api::iEngine* const engine) {
    if (nullptr == _MMap) {
        return;
    }

    UInt64 now = SafeSystem::Time::GetMilliSecond();
    if (_LastStatTick != 0 && now - _LastStatTick < 1000) {
        return;
    }
    _LastStatTick = now;

#ifdef WIN32
    LogMMapStats(engine, _MMap);
    LogProcessMemoryLayout(engine);
#else
    IMPORTANT(engine, "MMapStats path=%s length=%lluMB address=%p", _MMap->FilePath().c_str(), _MMap->Length() / 1024 / 1024, _MMap->Address());
#endif //WIN32
}
