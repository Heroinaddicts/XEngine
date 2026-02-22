#ifndef __SQLCacheColumn_h__
#define __SQLCacheColumn_h__

#include "Header.h"

class SQLCacheRow;
class SQLCacheColumn : public ISQLCacheColumn {
public:
    virtual ~SQLCacheColumn() {}

    SQLCacheRow* const _Host;
    const ColumnTemplate* const _Template;
    const TableTemplate* const _TableTemplate;


    const char* const _File;
    const int _Line;

    static SQLCacheColumn* CreateSQLCacheColumn(SQLCacheRow* const host, const TableTemplate* tableT, const ColumnTemplate* t, const char* file, const int line);
    static void ReleaseSQLCacheColumn(SQLCacheColumn*& column);

    // Í¨¹ý ISQLCacheColumn ¼Ì³Ð
    ISQLCacheRow* Host() const override;
    __forceinline const ColumnTemplate* GetColumnTemplate() const override { return _Template; }

    __forceinline bool HasValue() const { return _HasValue; }

    __forceinline const std::string GetValueString() const override {
        switch (_Template->_Type) {
        case eColumnType::__Int32__: {
            return SafeString::Int32ToString(_Int32);
        }
        case eColumnType::__UInt32__: {
            return SafeString::UInt32ToString(_UInt32);
        }
        case eColumnType::__Int64__: {
            return SafeString::Int64ToString(_Int64);
        }
        case eColumnType::__UInt64__: {
            return SafeString::UInt64ToString(_UInt64);
        }
        case eColumnType::__Float__: {
            return SafeString::FloatToString(_Float);
        }
        case eColumnType::__String__: {
            return _String;
        }
        case eColumnType::__Bool__: {
            return _Bool ? "true" : "false";
        }
        default:
            XASSERT(false, "wtf");
            return "---";
        }
    }

    __forceinline bool SetValueString(const std::string& value, const char* file = __FILE__, const int line = __LINE__) override {
        if (GetValueString() == value) {
            return true;
        }

        switch (_Template->_Type) {
        case eColumnType::__Int32__: {
            return SetInt32(SafeString::StringToInt32(value), file, line);
        }
        case eColumnType::__UInt32__: {
            return SetUInt32(SafeString::StringToUInt32(value), file, line);
        }
        case eColumnType::__Int64__: {
            return SetInt64(SafeString::StringToInt64(value), file, line);
        }
        case eColumnType::__UInt64__: {
            return SetUInt64(SafeString::StringToUInt64(value), file, line);
        }
        case eColumnType::__Float__: {
            return SetFloat(SafeString::StringToFloat(value), file, line);
        }
        case eColumnType::__String__: {
            return SetString(value, file, line);
        }
        case eColumnType::__Bool__: {
            return SetBool(value == "true", file, line);
        }
        default:
            XASSERT(false, "wtf");
            return false;
        }
    }

#define GetColumnValue(TYPE, file, line) \
    XASSERT(_Template->_Type == eColumnType::__##TYPE##__, "SQLCacheColumn::Get"#TYPE" Type Error %s:%d", file, line); \
    if (_Template->_Type != eColumnType::__##TYPE##__) \
        ErrorLog(g_Engine, "SQLCacheColumn::Get"#TYPE" Type Error %s:%d", file, line); \
    return _##TYPE;
    __forceinline UInt32 GetUInt32(const char* file, const int line) const override { GetColumnValue(UInt32, file, line); }
    __forceinline Int32 GetInt32(const char* file, const int line) const override { GetColumnValue(Int32, file, line); }
    __forceinline UInt64 GetUInt64(const char* file, const int line) const override { GetColumnValue(UInt64, file, line); }
    __forceinline Int64 GetInt64(const char* file, const int line) const override { GetColumnValue(Int64, file, line); }
    __forceinline float GetFloat(const char* file, const int line) const override { GetColumnValue(Float, file, line); }
    __forceinline const std::string GetString(const char* file, const int line) const override { GetColumnValue(String, file, line); }
    __forceinline bool GetBool(const char* file, const int line) const override { GetColumnValue(Bool, file, line); }

    bool SetInt32(const Int32 value, const char* file, const int line) override;
    bool SetUInt32(const UInt32 value, const char* file, const int line) override;
    bool SetInt64(const Int64 value, const char* file, const int line) override;
    bool SetUInt64(const UInt64 value, const char* file, const int line) override;
    bool SetFloat(const float value, const char* file, const int line) override;
    bool SetString(const std::string& value, const char* file, const int line) override;
    bool SetBool(const bool value, const char* file, const int line) override;
private:
    friend XPool<SQLCacheColumn>;
    SQLCacheColumn(SQLCacheRow* const host, const TableTemplate* tableT, const ColumnTemplate* t, const char* file, const int line);

private:
    union {
        Int32 _Int32;
        UInt32 _UInt32;
        Int64 _Int64;
        UInt64 _UInt64;
        float _Float;
        bool _Bool;
    };

    std::string _String;

    bool _HasValue;
};

#endif //__SQLCacheColumn_h__
