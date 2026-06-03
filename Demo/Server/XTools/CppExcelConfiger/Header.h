#ifndef __Header_h__
#define __Header_h__

#include "MultiSys.h"
#include "SafeString.h"
#include "SafeMemory.h"
#include <string>
#include <map>
#include "Excel/book.hpp"
UsingXEngine;

__forceinline std::string FormatExcelString(const std::wstring& str) {
    std::string ret = SafeString::WStringToUTF8(str.c_str());
    ret = SafeString::Replace(ret, "\n", "");
    ret = SafeString::Replace(ret, "\\", "\\\\");
    return ret;
}

enum class eAutoConfigType {
    __Int__ = 0,
    __IntArray__ = 1,
    __Float__ = 3,
    __FloatArray__ = 4,
    __String__ = 5,
    __StringArray__ = 6,
    __Bool__ = 7,
    __BoolArray__ = 8,
    __Enum__ = 9,
    __EnumArray__ = 10
};

eAutoConfigType GetType(const std::string& typeString);

struct AutoConfigEnum {
    struct Member {
        const std::string _Name;
        const int _Value;
        const std::string _Desc;

        Member(const std::string& name, const int value, const std::string& desc) : _Name(name), _Value(value), _Desc(desc) {}
    };

    const std::string _Name;
    const std::string _Desc;
    const std::vector<Member*> _Members;

    AutoConfigEnum(const std::string& name, const std::string& desc, const std::vector<Member*>& members) : _Name(name), _Desc(desc), _Members(members) {}
};
extern std::map<std::string, AutoConfigEnum*> g_AutoConfigEnumMap;

struct AutoConfigConst {
    struct Member {
        const std::string _Name;
        const eAutoConfigType _Type;
        const std::string _TypeString;
        const std::string _Value;
        const std::string _Desc;

        Member(const std::string& name, const std::string& typeString, const std::string& value, const std::string& desc) : _Name(name), _Type(GetType(typeString)), _TypeString(typeString), _Value(value), _Desc(desc) {}
    };

    const std::string _Name;
    const std::string _Desc;
    const std::vector<Member*> _Members;

    AutoConfigConst(const std::string& name, const std::string& desc, const std::vector<Member*> members) : _Name(name), _Desc(desc), _Members(members) {}
};
extern std::map<std::string, AutoConfigConst*> g_AutoConfigConstMap;

eAutoConfigType GetType(const std::string& typeString) {
    if (typeString == "int") return eAutoConfigType::__Int__;
    if (typeString == "int[]") return eAutoConfigType::__IntArray__;

    if (typeString == "float") return eAutoConfigType::__Float__;
    if (typeString == "float[]") return eAutoConfigType::__FloatArray__;

    if (typeString == "string") return eAutoConfigType::__String__;
    if (typeString == "string[]") return eAutoConfigType::__StringArray__;

    if (typeString == "bool") return eAutoConfigType::__Bool__;
    if (typeString == "bool[]") return eAutoConfigType::__BoolArray__;

    if (g_AutoConfigEnumMap.find(SafeString::Replace(typeString, "[]", "")) != g_AutoConfigEnumMap.end()) {
        if (SafeString::StringMatch(typeString, "[]") == -1)
            return eAutoConfigType::__Enum__;
        else
            return eAutoConfigType::__EnumArray__;
    }

    XASSERT(false, "wtf");
}

extern std::map<std::string, const std::string> g_CodeTemplates;



__forceinline std::string GetTypeCode(const eAutoConfigType type, const std::string& typeString = "") {
    switch (type) {
    case eAutoConfigType::__Int__:
        return "Int64";
    case eAutoConfigType::__IntArray__:
        return "std::vector<Int64>";
    case eAutoConfigType::__Float__:
        return "float";
    case eAutoConfigType::__FloatArray__:
        return "std::vector<float>";
    case eAutoConfigType::__String__:
        return "char *";
    case eAutoConfigType::__StringArray__:
        return "std::vector<const char *>";
    case eAutoConfigType::__Bool__:
        return "bool";
    case eAutoConfigType::__BoolArray__:
        return "std::vector<bool>";
    case eAutoConfigType::__Enum__:
        return "Enum::" + typeString;
    case eAutoConfigType::__EnumArray__:
        return "std::vector<Enum::" + SafeString::Replace(typeString, "[]", "") + ">";
    }

    XASSERT(false, "wtf");
}

__forceinline std::string GetValueCode(const eAutoConfigType type, const std::string& valueString, const std::string& enumName = "") {
    if (valueString == "")
        return "{}";

    switch (type) {
    case eAutoConfigType::__Int__:
    case eAutoConfigType::__Float__:
    case eAutoConfigType::__Bool__:
        return "{ " + valueString + " }";
    case eAutoConfigType::__Enum__:
        return "{Enum::" + enumName + "::" + valueString + " }";
    case eAutoConfigType::__IntArray__:
    case eAutoConfigType::__FloatArray__:
    case eAutoConfigType::__BoolArray__:
        return "{" + SafeString::Replace(valueString, ";", ",") + "}";
    case eAutoConfigType::__EnumArray__: {
        return "{Enum::" + enumName + "::" + SafeString::Replace(valueString, ";", (", Enum::" + enumName + "::")) + "}";
        break;
    }
    case eAutoConfigType::__String__:
        return "{\"" + valueString + "\"}";
        break;
    case eAutoConfigType::__StringArray__: {
        std::string code = "{\"" + valueString + "\"}";
        return SafeString::Replace(code, ";", "\", \"");
    }
    }
}

struct AutoConfigStruct {
    struct ColumnInfo {
        const std::string _Name;
        const eAutoConfigType _Type;
        const std::string _TypeString;
        const std::string _Desc;

        ColumnInfo(const std::string& name, const eAutoConfigType type, const std::string& typeString, const std::string& desc)
            : _Name(name), _Type(type), _TypeString(typeString), _Desc(desc) {
        }
    };

    struct ColumnValue {
        const ColumnInfo* const _Info;
        const std::string _Value;

        ColumnValue(const ColumnInfo* info, const std::string& value) : _Info(info), _Value(value) {}
    };

    const std::string _Name;
    const std::vector<ColumnInfo*> _ColumnInfos;
    const std::vector<std::vector<const ColumnValue*>> _Rows;

    AutoConfigStruct(
        const std::string& name,
        const std::vector<ColumnInfo*>& columnInfos,
        const std::vector<std::vector<const ColumnValue*>>& rows
    ) : _Name(name), _ColumnInfos(columnInfos), _Rows(rows) {
    }
};

extern std::map<std::string, const AutoConfigStruct*> g_AutoConfigData;
extern std::map<std::string, const  Excel::Sheet*> g_DataExcel;

#endif //__Header_h__
