#ifndef __Header_h__
#define __Header_h__

#include "MultiSys.h"
#include "SafeString.h"
#include "SafeMemory.h"
#include <string>
#include <map>

#include "Excel/book.hpp"

UsingXEngine;

#define ARRAY_TAG "[]"

#define INT_TAG "int"
#define FLOAT_TAG "float"
#define STRING_TAG "string"
#define BOOL_TAG "bool"
#define TABLE_TAG "table"
#define SCRIPT_TAG "script"

#define MEMBER_SPLIT_TAG ";"
#define ARRAY_SPLIT_TAG ";"

namespace ExcelConfiger {
    __forceinline std::string FormatExcelString(const std::wstring& str) {
        std::string ret = SafeString::WStringToUTF8(str.c_str());
        ret = SafeString::Replace(ret, "\n", "");
        ret = SafeString::Replace(ret, "\\", "\\\\");
        return ret;
    }

    __forceinline int GetStringMemorySize(const std::string& str) {
        return str.length() + 1;
    }

    class Enum;
    class Const;
    class StructDef;
    class Struct;
    class Array;

    enum class eValueType {
        Int,
        Float,
        String,
        Bool,
        Enum,
        Const,
        Table,
        Script
    };

    struct TypeInfo {
        const bool _IsArray;
        const eValueType _ValueType;
        const std::string _TypeString;

        TypeInfo(const bool isArray, const eValueType valueType, const std::string& typeString) : _IsArray(isArray), _ValueType(valueType), _TypeString(typeString) {}
    };

    struct Value {
        const TypeInfo _Info;
        const std::string _ValueString;
        const std::string _Desc;
        union {
            const Int64 _Int;
            const float _Float;
            const char* _String;
            const bool _Bool;

            const Struct* const _Struct;
            const Array* const _Array;
        };

        Value(const TypeInfo& info, const std::string& valueString, const std::string& desc, const Int64 value) : _Info(info), _ValueString(valueString), _Desc(desc), _Int(value) {}
        Value(const TypeInfo& info, const std::string& valueString, const std::string& desc, const float value) : _Info(info), _ValueString(valueString), _Desc(desc), _Float(value) {}

        Value(const TypeInfo& info, const std::string& valueString, const std::string& desc, const std::string& value) : _Info(info), _ValueString(valueString), _Desc(desc), _String((char*)txmalloc(GetStringMemorySize(value))) {
            SafeMemory::Memset((char*)_String, GetStringMemorySize(value), GetStringMemorySize(value), 0);
            strcpy((char*)_String, value.c_str());
        }

        Value(const TypeInfo& info, const std::string& valueString, const std::string& desc, const bool value) : _Info(info), _ValueString(valueString), _Desc(desc), _Bool(value) {}

        Value(const TypeInfo& info, const std::string& valueString, const std::string& desc, const Struct* s) : _Info(info), _ValueString(valueString), _Desc(desc), _Struct(s) {}
        Value(const TypeInfo& info, const std::string& valueString, const std::string& desc, const Array* a) : _Info(info), _ValueString(valueString), _Desc(desc), _Array(a) {}
    };

    TypeInfo GetTypeInfo(const std::string& str);
}

#endif //__Header_h__
