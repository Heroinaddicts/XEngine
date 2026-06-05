#ifndef __PerfTestProtocol_h__
#define __PerfTestProtocol_h__

#include "MultiSys.h"

#include <atomic>
#include <cstring>
#include <vector>

namespace PerfTest {

constexpr UInt16 MaxBodySize = 4 * 1024;
constexpr int HeaderSize = sizeof(UInt16);
constexpr int MaxPacketSize = HeaderSize + MaxBodySize;

__forceinline UInt16 BodySize(const void* const content) {
    UInt16 bodySize = 0;
    memcpy(&bodySize, content, HeaderSize);
    return bodySize;
}

template<typename F>
__forceinline int ConsumePackets(const void* const content, const int size, const F& onPacket) {
    int offset = 0;
    const char* const bytes = (const char* const)content;
    while (size - offset >= HeaderSize) {
        const UInt16 bodySize = BodySize(bytes + offset);
        const int packetSize = HeaderSize + bodySize;
        if (bodySize > MaxBodySize) {
            return size;
        }
        if (size - offset < packetSize) {
            break;
        }

        onPacket(bytes + offset, packetSize, bytes + offset + HeaderSize, bodySize);
        offset += packetSize;
    }
    return offset;
}

__forceinline void BuildPacket(std::vector<char>& packet, const UInt16 bodySize) {
    packet.resize(HeaderSize + bodySize);
    memcpy(packet.data(), &bodySize, HeaderSize);
    if (bodySize > 0) {
        memset(packet.data() + HeaderSize, 0, bodySize);
    }
}

struct PacketStatsSnapshot {
    UInt64 Count = 0;
    UInt64 TotalCount = 0;
    UInt64 Bytes = 0;
    UInt64 Zero = 0;
    UInt64 Size1To512 = 0;
    UInt64 Size512To1024 = 0;
    UInt64 Size1024To2048 = 0;
    UInt64 Size2048To4096 = 0;
};

class PacketStats {
public:
    void Add(const UInt16 bodySize, const int packetSize) {
        _Count.fetch_add(1, std::memory_order_relaxed);
        _TotalCount.fetch_add(1, std::memory_order_relaxed);
        _Bytes.fetch_add((UInt64)packetSize, std::memory_order_relaxed);

        if (bodySize == 0) {
            _Zero.fetch_add(1, std::memory_order_relaxed);
        } else if (bodySize <= 512) {
            _Size1To512.fetch_add(1, std::memory_order_relaxed);
        } else if (bodySize <= 1024) {
            _Size512To1024.fetch_add(1, std::memory_order_relaxed);
        } else if (bodySize <= 2048) {
            _Size1024To2048.fetch_add(1, std::memory_order_relaxed);
        } else {
            _Size2048To4096.fetch_add(1, std::memory_order_relaxed);
        }
    }

    PacketStatsSnapshot TakeSnapshotAndReset() {
        PacketStatsSnapshot snapshot;
        snapshot.Count = _Count.exchange(0, std::memory_order_relaxed);
        snapshot.TotalCount = _TotalCount.load(std::memory_order_relaxed);
        snapshot.Bytes = _Bytes.exchange(0, std::memory_order_relaxed);
        snapshot.Zero = _Zero.exchange(0, std::memory_order_relaxed);
        snapshot.Size1To512 = _Size1To512.exchange(0, std::memory_order_relaxed);
        snapshot.Size512To1024 = _Size512To1024.exchange(0, std::memory_order_relaxed);
        snapshot.Size1024To2048 = _Size1024To2048.exchange(0, std::memory_order_relaxed);
        snapshot.Size2048To4096 = _Size2048To4096.exchange(0, std::memory_order_relaxed);
        return snapshot;
    }

private:
    std::atomic<UInt64> _Count{0};
    std::atomic<UInt64> _TotalCount{0};
    std::atomic<UInt64> _Bytes{0};
    std::atomic<UInt64> _Zero{0};
    std::atomic<UInt64> _Size1To512{0};
    std::atomic<UInt64> _Size512To1024{0};
    std::atomic<UInt64> _Size1024To2048{0};
    std::atomic<UInt64> _Size2048To4096{0};
};

}

#endif //__PerfTestProtocol_h__
