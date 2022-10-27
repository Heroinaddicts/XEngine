#ifndef __Attribute_h__
#define __Attribute_h__

#include "Header.h"
#include "Value.h"

class Attribute {
public:
    virtual ~Attribute() {}
    Attribute(AttrIndex* index) : _Index(index), _Value(index->_Type) {}

    __forceinline bool SetInt(const int64 value) {
        return _Value.SetInt(value);
    }

    __forceinline bool SetFloat(const float value) {
        return _Value.SetFloat(value);
    }

    __forceinline bool SetString(const std::string& value) {
        return _Value.SetString(value);
    }

    __forceinline bool SetBool(const bool value) {
        return _Value.SetBool(value);
    }

    __forceinline int64 GetInt() const {
        return _Value.GetInt();
    }

    __forceinline float GetFloat() const {
        return _Value.GetFloat();
    }

    __forceinline const char* GetString() const {
        return _Value.GetString();
    }

    __forceinline bool GetBool() const {
        return _Value.GetBool();
    }

    AttrIndex* const _Index;

    Value _Value;
};

#endif //__Attribute_h__
