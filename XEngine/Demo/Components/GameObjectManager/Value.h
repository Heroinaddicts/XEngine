#ifndef __Value_h__
#define __Value_h__

#include "Header.h"

class Value {
public:
    virtual ~Value() {}
    Value(eType type) : _Type(type) {
        SafeMemory::Memset(_String, sizeof(_String), 0, sizeof(_String));
    }

    __forceinline bool SetInt(const int64 value) {
        XASSERT(_Type == eType::__int__, "error Value type, not int");
        if (_Type == eType::__int__) {
            _Int = value;
            return true;
        }
        return false;
    }

    __forceinline bool SetFloat(const float value) {
        XASSERT(_Type == eType::__float__, "error Value type, not int");
        if (_Type == eType::__float__) {
            _Float = value;
            return true;
        }
        return false;
    }

    __forceinline bool SetString(const std::string& value) {
        XASSERT(_Type == eType::__string__, "error Value type, not int");
        if (_Type == eType::__string__) {
            int len = value.length() <= MAX_STRING_LEN ? value.length() : MAX_STRING_LEN;
            SafeMemory::Memcpy(_String, sizeof(_String), value.c_str(), len);
            XASSERT(len <= MAX_STRING_LEN, "wtf");
            _String[len + 1] = 0;
            return true;
        }
        return false;
    }

    __forceinline bool SetBool(const bool value) {
        XASSERT(_Type == eType::__bool__, "error Value type, not int");
        if (_Type == eType::__bool__) {
            _Bool = value;
            return true;
        }
        return false;
    }

    __forceinline int64 GetInt() const {
        XASSERT(_Type == eType::__int__, "error Value type, not int");
        return _Type == eType::__int__ ? _Int : 0xFFFF;
    }

    __forceinline float GetFloat() const {
        XASSERT(_Type == eType::__float__, "error Value type, not int");
        return _Type == eType::__float__ ? _Float : 0xFFFF;
    }

    __forceinline const char* GetString() const {
        XASSERT(_Type == eType::__string__, "error Value type, not int");
        return _Type == eType::__string__ ? _String : nullptr;
    }

    __forceinline bool GetBool() const {
        XASSERT(_Type == eType::__bool__, "error Value type, not int");
        return _Type == eType::__bool__ ? _Bool : false;
    }

    const eType _Type;

private:
    union {
        int64 _Int;
        float _Float;
        char _String[MAX_STRING_LEN + 1];
        bool _Bool;
    };
};

#endif __Value_h__
