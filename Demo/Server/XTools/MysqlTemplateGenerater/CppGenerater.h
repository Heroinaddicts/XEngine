#ifndef __CppGenerater_h__
#define __CppGenerater_h__

#include "Header.h"

__forceinline std::string GetCppTypeString(const eColumnType type) {
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
        return "std::string &";
    case eColumnType::Float:
        return "float";
    case eColumnType::Bool:
        return "bool";
    default:
        XASSERT(false, "wtf");
        break;
    }
    return "";
}

__forceinline std::string eColumnTypeToString(const eColumnType type) {
    switch (type) {
    case eColumnType::Int32:
        return "__Int32__";
    case eColumnType::UInt32:
        return "__UInt32__";
    case eColumnType::Int64:
        return "__Int64__";
    case eColumnType::UInt64:
        return "__UInt64__";
    case eColumnType::String:
        return "__String__";
    case eColumnType::Float:
        return "__Float__";
    case eColumnType::Bool:
        return "__Bool__";
    default:
        XASSERT(false, "wtf");
        break;
    }
    return "";
}

__forceinline std::string GetSqlValueType(const eColumnType type) {
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
    case eColumnType::Bool:
        return "Bool";
    default:
        XASSERT(false, "wtf");
        break;
    }
    return "";
}

__forceinline void GeneratorTableCppCode(TabInfo* const tab, std::string& tableNamespacesCode, std::string& tableTemplatesCode) {
    {
        tableNamespacesCode += g_CodeTemplates["TableNamespace"];
        tableNamespacesCode = SafeString::Replace(tableNamespacesCode, "{TableName}", tab->_Name);
        std::string eColumnsCode;
        for (int i = 0; i < tab->_ColumnCount; i++) {
            eColumnsCode += g_CodeTemplates["Column"];
            eColumnsCode = SafeString::Replace(eColumnsCode, "{ColumnName}", tab->_Columns[i]->_Name);
            eColumnsCode = SafeString::Replace(eColumnsCode, "{ColumnType}", eColumnTypeToString(tab->_Columns[i]->_Type));
            eColumnsCode = SafeString::Replace(eColumnsCode, "{Primary}", (tab->_Columns[i]->_Primary ? "Primary" : ""));
            eColumnsCode = SafeString::Replace(eColumnsCode, "{Unique}", (tab->_Columns[i]->_Unique ? "Unique" : ""));
            eColumnsCode = SafeString::Replace(eColumnsCode, "{ColumnIndex}", SafeString::Int32ToString(i));
        }
        tableNamespacesCode = SafeString::Replace(tableNamespacesCode, "{eColumns}", eColumnsCode);
        tableNamespacesCode = SafeString::Replace(tableNamespacesCode, "{eColumnCount}", SafeString::Int32ToString(tab->_ColumnCount));
    }

    {
        tableTemplatesCode += g_CodeTemplates["TableTemplate"];
        tableTemplatesCode = SafeString::Replace(tableTemplatesCode, "{TableName}", tab->_Name);
        tableTemplatesCode = SafeString::Replace(tableTemplatesCode, "{PrimaryName}", tab->_Primary->_Name);
        tableTemplatesCode = SafeString::Replace(tableTemplatesCode, "{PrimaryType}", eColumnTypeToString(tab->_Primary->_Type));
        std::string columnTemplatesCode;
        for (int i = 0; i < tab->_ColumnCount; i++) {
            columnTemplatesCode += g_CodeTemplates["ColumnTemplate"];
            columnTemplatesCode = SafeString::Replace(columnTemplatesCode, "{ColumnName}", tab->_Columns[i]->_Name);
            columnTemplatesCode = SafeString::Replace(columnTemplatesCode, "{TableName}", tab->_Name);
            columnTemplatesCode = SafeString::Replace(columnTemplatesCode, "{ColumnType}", eColumnTypeToString(tab->_Columns[i]->_Type));
            columnTemplatesCode = SafeString::Replace(columnTemplatesCode, "{IsPrimary}", tab->_Columns[i]->_Primary ? "true" : "false");
            columnTemplatesCode = SafeString::Replace(columnTemplatesCode, "{IsUnique}", tab->_Columns[i]->_Unique ? "true" : "false");
            columnTemplatesCode = SafeString::Replace(columnTemplatesCode, "{Primary}", tab->_Columns[i]->_Primary ? "Primary" : "");
            columnTemplatesCode = SafeString::Replace(columnTemplatesCode, "{Unique}", tab->_Columns[i]->_Unique ? "Unique" : "");
        }
        tableTemplatesCode = SafeString::Replace(tableTemplatesCode, "{ColumnTemplates}", columnTemplatesCode);
    }
}

__forceinline std::string GeneratorCppCode() {
    std::string sqlCacheTemplate = g_CodeTemplates["SQLCacheTemplate"];
    std::string eTableCode, tableNamesCode, tableNamespacesCode, tableTemplatesCode;
    for (int i = 0; i < g_Tables.size(); i++) {
        eTableCode += g_Tables[i]->_Name + " = " + SafeString::Int32ToString(i) + ",\n";
        tableNamesCode += "\"" + g_Tables[i]->_Name + "\"" + (i != g_Tables.size() - 1 ? ", " : "");
        GeneratorTableCppCode(g_Tables[i], tableNamespacesCode, tableTemplatesCode);
        tableTemplatesCode += (i != g_Tables.size() - 1 ? ",\n" : "\n");
    }

    sqlCacheTemplate = SafeString::Replace(sqlCacheTemplate, "{eTable}", eTableCode);
    sqlCacheTemplate = SafeString::Replace(sqlCacheTemplate, "{eTableCount}", SafeString::Int32ToString(g_Tables.size()));
    sqlCacheTemplate = SafeString::Replace(sqlCacheTemplate, "{tableNames}", tableNamesCode);
    sqlCacheTemplate = SafeString::Replace(sqlCacheTemplate, "{TableNamespaces}", tableNamespacesCode);
    sqlCacheTemplate = SafeString::Replace(sqlCacheTemplate, "{TableTemplates}", tableTemplatesCode);

    return sqlCacheTemplate;
}

#endif //__CppGenerater_h__