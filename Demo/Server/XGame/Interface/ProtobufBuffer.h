#ifndef __ProtobufBuffer_h__
#define __ProtobufBuffer_h__

#include "iModule.h"
#include "google/protobuf/message.h"

#include <limits>
#include <vector>

UsingXEngine;

inline bool SerializeProtobufToBuffer(const ::google::protobuf::Message& pb, std::vector<char>& buffer) {
    const size_t size = pb.ByteSizeLong();
    if (size > static_cast<size_t>(std::numeric_limits<int>::max())) {
        XASSERT(false, "protobuf too large %llu", static_cast<UInt64>(size));
        return false;
    }

    buffer.resize(size);
    return pb.SerializePartialToArray(buffer.empty() ? nullptr : buffer.data(), static_cast<int>(buffer.size()));
}

#endif //__ProtobufBuffer_h__
