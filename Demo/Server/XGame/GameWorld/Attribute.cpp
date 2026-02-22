#include "Attribute.h"
#include "GameObject.h"

static XPool<Attribute> s_Pool;

Attribute* Attribute::Create(GameObject* const go, const eAttributeType type, const std::string& name, const int index, const bool sync, const bool share) {
    return XPOOL_CREATE(s_Pool, go, type, name, index, sync, share);
}

void Attribute::Release(Attribute*& attr) {
    XPOOL_RELEASE(s_Pool, attr);
    attr = nullptr;
}

#define SET_VALUE(TYPE, value) \
    XASSERT(_Type == eAttributeType::__##TYPE##__, "Attribute "#TYPE" error %s:%d", file, line); \
    if (eAttributeType::__##TYPE##__ != _Type) { \
        ErrorLog(g_Engine, "Attribute %s type "#TYPE" error, match type is %d %s:%d", _Name.c_str(), _Type, file, line); \
        return false; \
    } \
    if (_Host->_Inited) {\
        for(int i=0; i<(int)eGameObjectType::__AllType__; i++) {\
            if (_Host->Is((eGameObjectType)i)) { \
                auto itor = g_AttributeCallback.find((eGameObjectType)i); \
                if (g_AttributeCallback.end() != itor) { \
                    for (auto it = itor->second._##TYPE##Judge.begin(); it != itor->second._##TYPE##Judge.end(); it++) { \
                        if (!(*it)(_Host, _Index, _Sync, _Share, __##TYPE, value)) { \
                            return false; \
                        } \
                    } \
                    for (auto it = itor->second._##TYPE##Changed.begin(); it != itor->second._##TYPE##Changed.end(); it++) { \
                        (*it)(_Host, _Index, _Sync, _Share, __##TYPE, value); \
                    } \
                } \
            } \
        }\
    }\
    __##TYPE = value; \
    return true;


bool Attribute::SetUInt32(const UInt32 value, const bool trigger, const char* file, const int line) {
    SET_VALUE(UInt32, value);
}

bool Attribute::SetInt32(const Int32 value, const bool trigger, const char* file, const int line) {
    SET_VALUE(Int32, value);
}

bool Attribute::SetUInt64(const UInt64 value, const bool trigger, const char* file, const int line) {
    SET_VALUE(UInt64, value);
}

bool Attribute::SetInt64(const Int64 value, const bool trigger, const char* file, const int line) {
    SET_VALUE(Int64, value);
}

bool Attribute::SetFloat(const float value, const bool trigger, const char* file, const int line) {
    SET_VALUE(Float, value);
}

bool Attribute::SetBool(const bool value, const bool trigger, const char* file, const int line) {
    SET_VALUE(Bool, value);
}

bool Attribute::SetString(const std::string& value, const bool trigger, const char* file, const int line) {
    SET_VALUE(String, value);
}

bool Attribute::SetVector2(const Vector2& value, const bool trigger, const char* file, const int line) {
    XASSERT(_Type == eAttributeType::__Vector2__, "Attribute Vector2 error %s:%d", file, line);
    if (eAttributeType::__Vector2__ != _Type) {
        ErrorLog(g_Engine, "Attribute %s type Vector2 error, match type is %d %s:%d", _Name.c_str(), _Type, file, line);
        return false;
    }
    if (_Host->_Inited) {
        for (int i = 0; i < (int)eGameObjectType::__AllType__; i++) {
            if (_Host->Is((eGameObjectType)i)) {
                auto itor = g_AttributeCallback.find((eGameObjectType)i);
                if (g_AttributeCallback.end() != itor) {
                    for (auto it = itor->second._Vector2Judge.begin(); it != itor->second._Vector2Judge.end(); it++) {
                        if (!(*it)(_Host, _Index, _Sync, _Share, __Vector2, value)) {
                            return false;
                        }
                    }
                    for (auto it = itor->second._Vector2Changed.begin(); it != itor->second._Vector2Changed.end(); it++) {
                        (*it)(_Host, _Index, _Sync, _Share, __Vector2, value);
                    }
                }
            }
        }
    }
    __Vector2 = value;
    return true;
}

bool Attribute::SetVector3(const Vector3& value, const bool trigger, const char* file, const int line) {
    SET_VALUE(Vector3, value);
}
