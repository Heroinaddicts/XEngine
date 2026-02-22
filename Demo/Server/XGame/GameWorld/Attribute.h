#ifndef __GoAttribute_h__
#define __GoAttribute_h__

#include "Header.h"

class GameObject;

class Attribute : public IAttribute {
public:
    virtual ~Attribute() {}

    GameObject* const _Host;

    const bool _Share;
    const bool _Sync;

    static Attribute* Create(GameObject* const go, const eAttributeType type, const std::string& name, const int index, const bool sync, const bool share);
    static void Release(Attribute*& attr);

#define GET_VALUE(TYPE) \
    XASSERT(_Type == eAttributeType::__##TYPE##__, "Attribute "#TYPE " error %s:%d", file, line); \
    if (eAttributeType::__##TYPE##__ != _Type) { \
        ErrorLog(g_Engine, "Attribute %s type "#TYPE " error, match type is %d %s:%d", _Name.c_str(), _Type, file, line); \
        return 0; \
    } \
    return __##TYPE;

    __forceinline UInt32 GetUInt32(const char* file, const int line) const override {
        GET_VALUE(UInt32);
    }
    bool SetUInt32(const UInt32 value, const bool trigger, const char* file, const int line)override;

    __forceinline Int32 GetInt32(const char* file, const int line) const override {
        GET_VALUE(Int32);
    }
    bool SetInt32(const Int32 value, const bool trigger, const char* file, const int line)override;

    __forceinline UInt64 GetUInt64(const char* file, const int line) const override {
        GET_VALUE(UInt64);
    }
    bool SetUInt64(const UInt64 value, const bool trigger, const char* file, const int line) override;

    __forceinline Int64 GetInt64(const char* file, const int line) const override {
        GET_VALUE(Int64);
    }
    bool SetInt64(const Int64 value, const bool trigger, const char* file, const int line) override;

    __forceinline float GetFloat(const char* file, const int line) const override {
        GET_VALUE(Float);
    }
    bool SetFloat(const float value, const bool trigger, const char* file, const int line) override;

    __forceinline bool GetBool(const char* file, const int line) const override {
        GET_VALUE(Bool);
    }
    bool SetBool(const bool value, const bool trigger, const char* file, const int line) override;

    __forceinline const std::string& GetString(const char* file, const int line) const override {
        GET_VALUE(String);
    }
    bool SetString(const std::string& value, const bool trigger, const char* file, const int line) override;

    __forceinline Vector2 GetVector2(const char* file, const int line) const override {
        XASSERT(_Type == eAttributeType::__Vector2__, "Attribute Vector2 error %s:%d", file, line);
        if (eAttributeType::__Vector2__ != _Type) {
            ErrorLog(g_Engine, "Attribute %s type Vector2 error, match type is %d %s:%d", _Name.c_str(), _Type, file, line);
            return 0;
        }
        return __Vector2;
    }
    bool SetVector2(const Vector2& value, const bool trigger, const char* file, const int line) override;

    __forceinline Vector3 GetVector3(const char* file, const int line) const override {
        GET_VALUE(Vector3);
    }
    bool SetVector3(const Vector3& value, const bool trigger, const char* file, const int line) override;

private:
    union {
        UInt32 __UInt32;
        Int32 __Int32;
        UInt64 __UInt64;
        Int64 __Int64;
        float __Float;
        bool __Bool;
    };

    std::string __String;
    Vector2 __Vector2;
    Vector3 __Vector3;

    friend XPool<Attribute>;
    Attribute(GameObject* const go, const eAttributeType type, const std::string& name, const int index, const bool sync, const bool share)
        :IAttribute(type, name, index), _Host(go), __Int64(0), _Sync(sync), _Share(share) {
    }
};

#endif //__GoAttribute_h__
