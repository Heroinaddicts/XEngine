#pragma once

#include "MultiSys.h"
#include "SafeString.h"
#include "SafeMemory.h"
#include "SafeSystem.h"
#include "Launcher.h"
#include "XDumper.h"
#include <vector>
#include <map>

UsingXEngine;

enum class eColumnType {
    Int32 = 0,
    UInt32 = 1,
    Int64 = 2,
    UInt64 = 3,
    Float = 4,
    String = 5,
    Bool = 6,

    __Unknown__ = 7
};

__forceinline std::string GetColumnTypeString(const eColumnType type) {
    switch (type) {
    case eColumnType::Int32:
        return "Int32";
    case eColumnType::UInt32:
        return "UInt32";
    case eColumnType::Int64:
        return "Int64";
    case eColumnType::UInt64:
        return "UInt64";
    case eColumnType::String:
        return "String";
    case eColumnType::Float:
        return "Float";
    default:
        XASSERT(false, "wtf");
        break;
    }
    return "";
}

__forceinline eColumnType Type(const std::string& type) {
    if (SafeString::StringMatch(type, "int") == 0) {
        if (SafeString::StringMatch(type, "unsigned") >= 0)
            return eColumnType::UInt32;
        else
            return eColumnType::Int32;
    }

    if (SafeString::StringMatch(type, "bigint") == 0) {
        if (SafeString::StringMatch(type, "unsigned") >= 0)
            return eColumnType::UInt64;
        else
            return eColumnType::Int64;
    }

    if (SafeString::StringMatch(type, "varchar") == 0 || SafeString::StringMatch(type, "text") == 0 || SafeString::StringMatch(type, "timestamp") == 0) return eColumnType::String;
    if (SafeString::StringMatch(type, "float") == 0) return eColumnType::Float;
    if (SafeString::StringMatch(type, "bit") == 0) return eColumnType::Bool;

    XASSERT(false, "wtf %s", type.c_str());
    return eColumnType::__Unknown__;
}

struct ColumnInfo {
    const std::string _Name;
    const bool _Primary;
    const bool _Unique;
    const int _Index;
    const eColumnType _Type;
    const std::string _ColumnCodeString;

    ColumnInfo(const std::string& name, const bool primary, const bool unique, const int index, const eColumnType type)
        : _Name(name), _Primary(primary), _Unique(unique), _Index(index), _Type(type),
        _ColumnCodeString(_Name) {
    }
};

struct TabInfo {
    const std::string _Name;
    ColumnInfo* const _Primary;
    ColumnInfo** const _Columns;
    const int _ColumnCount;

    TabInfo(const std::string& name, ColumnInfo* const primary, ColumnInfo** const columns, const int columnCount)
        : _Name(name), _Primary(primary), _Columns(columns), _ColumnCount(columnCount) {
    }
};

extern std::vector<TabInfo*> g_Tables;
extern std::map<std::string, std::string> g_CodeTemplates;

