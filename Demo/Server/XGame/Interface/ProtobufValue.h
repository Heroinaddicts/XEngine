#ifndef __ProtobufValue_h__
#define __ProtobufValue_h__

#include "Game.pb.h"
#include "UnitCommon.h"
#include "MultiSys.h"
#include "iEngine.h"
#include <google/protobuf/stubs/common.h>

namespace XGameProtobuf {

__forceinline std::string HexPreview(const std::string& value, const int maxBytes = 32) {
    static const char* hex = "0123456789ABCDEF";
    std::string ret;
    const int count = (int)value.size() < maxBytes ? (int)value.size() : maxBytes;
    ret.reserve(count * 3);
    for (int i = 0; i < count; i++) {
        const unsigned char c = (unsigned char)value[i];
        if (i > 0)
            ret += ' ';
        ret += hex[c >> 4];
        ret += hex[c & 0x0F];
    }
    if ((int)value.size() > maxBytes)
        ret += " ...";
    return ret;
}

__forceinline void AppendReplacementChar(std::string& ret) {
    ret += (char)0xEF;
    ret += (char)0xBF;
    ret += (char)0xBD;
}

__forceinline bool IsContinuationByte(const unsigned char c) {
    return (c & 0xC0) == 0x80;
}

__forceinline bool AppendValidUtf8CharOrReplacement(const std::string& value, size_t& i, std::string& ret) {
    const unsigned char c0 = (unsigned char)value[i];
    const size_t remain = value.size() - i;
    int size = 0;

    if (c0 <= 0x7F) {
        size = 1;
    }
    else if (c0 >= 0xC2 && c0 <= 0xDF) {
        size = 2;
        if (remain < 2 || !IsContinuationByte((unsigned char)value[i + 1]))
            size = 0;
    }
    else if (c0 == 0xE0) {
        size = 3;
        if (remain < 3 || (unsigned char)value[i + 1] < 0xA0 || (unsigned char)value[i + 1] > 0xBF || !IsContinuationByte((unsigned char)value[i + 2]))
            size = 0;
    }
    else if ((c0 >= 0xE1 && c0 <= 0xEC) || (c0 >= 0xEE && c0 <= 0xEF)) {
        size = 3;
        if (remain < 3 || !IsContinuationByte((unsigned char)value[i + 1]) || !IsContinuationByte((unsigned char)value[i + 2]))
            size = 0;
    }
    else if (c0 == 0xED) {
        size = 3;
        if (remain < 3 || (unsigned char)value[i + 1] < 0x80 || (unsigned char)value[i + 1] > 0x9F || !IsContinuationByte((unsigned char)value[i + 2]))
            size = 0;
    }
    else if (c0 == 0xF0) {
        size = 4;
        if (remain < 4 || (unsigned char)value[i + 1] < 0x90 || (unsigned char)value[i + 1] > 0xBF || !IsContinuationByte((unsigned char)value[i + 2]) || !IsContinuationByte((unsigned char)value[i + 3]))
            size = 0;
    }
    else if (c0 >= 0xF1 && c0 <= 0xF3) {
        size = 4;
        if (remain < 4 || !IsContinuationByte((unsigned char)value[i + 1]) || !IsContinuationByte((unsigned char)value[i + 2]) || !IsContinuationByte((unsigned char)value[i + 3]))
            size = 0;
    }
    else if (c0 == 0xF4) {
        size = 4;
        if (remain < 4 || (unsigned char)value[i + 1] < 0x80 || (unsigned char)value[i + 1] > 0x8F || !IsContinuationByte((unsigned char)value[i + 2]) || !IsContinuationByte((unsigned char)value[i + 3]))
            size = 0;
    }

    if (size <= 0) {
        AppendReplacementChar(ret);
        i++;
        return false;
    }

    ret.append(value.data() + i, size);
    i += size;
    return true;
}

__forceinline std::string SanitizeUtf8(const std::string& value, bool& valid) {
    valid = google::protobuf::internal::IsStructurallyValidUTF8(value.data(), (int)value.size());
    if (valid)
        return value;

    std::string ret;
    ret.reserve(value.size());
    for (size_t i = 0; i < value.size();) {
        AppendValidUtf8CharOrReplacement(value, i, ret);
    }

    XASSERT(google::protobuf::internal::IsStructurallyValidUTF8(ret.data(), (int)ret.size()), "SanitizeUtf8 failed");
    return ret;
}

__forceinline void SetValueString(XEngine::Api::iEngine* const engine, oValue* const value, const std::string& text, const UInt64 guid, const int index, const char* name) {
    value->set__type((int)UnitCommon::eAttributeType::__String__);
    bool valid = false;
    std::string safe = SanitizeUtf8(text, valid);
    if (!valid) {
        XEngine::__Log__(engine, "Trace", false, __FILE__, __LINE__, "Invalid UTF8 string attribute guid %llu index %d name %s size %d hex %s", guid, index, name ? name : "", (int)text.size(), HexPreview(text).c_str());
    }
    value->set__string(safe);
}

__forceinline void SetValueUInt32(oValue* const value, const UInt32 v) {
    value->set__type((int)UnitCommon::eAttributeType::__UInt32__);
    value->set__uint(v);
}

__forceinline void SetValueInt32(oValue* const value, const Int32 v) {
    value->set__type((int)UnitCommon::eAttributeType::__Int32__);
    value->set__int(v);
}

__forceinline void SetValueUInt64(oValue* const value, const UInt64 v) {
    value->set__type((int)UnitCommon::eAttributeType::__UInt64__);
    value->set__uint(v);
}

__forceinline void SetValueInt64(oValue* const value, const Int64 v) {
    value->set__type((int)UnitCommon::eAttributeType::__Int64__);
    value->set__int(v);
}

__forceinline void SetValueFloat(oValue* const value, const float v) {
    value->set__type((int)UnitCommon::eAttributeType::__Float__);
    value->set__float(v);
}

__forceinline void SetValueBool(oValue* const value, const bool v) {
    value->set__type((int)UnitCommon::eAttributeType::__Bool__);
    value->set__bool(v);
}

__forceinline void SetValueVector2(oValue* const value, const XEngine::Vector2& v) {
    value->set__type((int)UnitCommon::eAttributeType::__Vector2__);
    oVector2* vector2 = value->mutable__vector2();
    vector2->set_x(v.x);
    vector2->set_y(v.y);
}

__forceinline void SetValueVector3(oValue* const value, const XEngine::Vector3& v) {
    value->set__type((int)UnitCommon::eAttributeType::__Vector3__);
    oVector3* vector3 = value->mutable__vector3();
    vector3->set_x(v.x);
    vector3->set_y(v.y);
    vector3->set_z(v.z);
}

}

#endif //__ProtobufValue_h__
