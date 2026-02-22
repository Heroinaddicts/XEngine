#ifndef __ICombat_h__
#define __ICombat_h__

#include "iModule.h"
#include "SQLCacheTemplate.h"
#include <functional>
UsingXEngine;

UsingSQLCacheTemplate;

class ISQLCacheRow;
class ISQLCacheColumn {
public:
    virtual ~ISQLCacheColumn() {}

    virtual ISQLCacheRow* Host() const = 0;
    virtual const ColumnTemplate* GetColumnTemplate() const = 0;

    virtual bool HasValue() const = 0;

    virtual const std::string GetValueString() const = 0;
    virtual bool SetValueString(const std::string& value, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual Int32 GetInt32(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual UInt32 GetUInt32(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual Int64 GetInt64(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual UInt64 GetUInt64(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual float GetFloat(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual const std::string GetString(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool GetBool(const char* file = __FILE__, const int line = __LINE__) const = 0;

    virtual bool SetInt32(const Int32 value, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual bool SetUInt32(const UInt32 value, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual bool SetInt64(const Int64 value, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual bool SetUInt64(const UInt64 value, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual bool SetFloat(const float value, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual bool SetString(const std::string& value, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual bool SetBool(const bool value, const char* file = __FILE__, const int line = __LINE__) = 0;
};

enum class eSQLOperatorType {
    Create = 0,
    Update = 1,
    Read = 2,
    Delete = 3
};

class ISQLCacheTable;
class ISQLCacheRow {
public:
    virtual ~ISQLCacheRow() {}

    virtual ISQLCacheTable* Host() const = 0;
    virtual ISQLCacheColumn* GetColumn(const int column) const = 0;
};

class ISQLCacheTable {
public:
    virtual ~ISQLCacheTable() {}

    virtual const TableTemplate* GetTemplate() const = 0;

    typedef std::function<void(ISQLCacheRow* const row)> fRowInit, fRowReturn;

    virtual ISQLCacheRow* Insert(const fRowInit& init, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual int RowCount() const = 0;

    virtual bool IsColumnExists(const int column, const std::string& value) const = 0;

    void Query(const int conditionColumn, const Int32 conditionValue, const fRowReturn& ret, const char* file = __FILE__, const int line = __LINE__) const {
        Query(conditionColumn, SafeString::Int32ToString(conditionValue), ret, file, line);
    }

    void Query(const int conditionColumn, const UInt32 conditionValue, const fRowReturn& ret, const char* file = __FILE__, const int line = __LINE__) const {
        Query(conditionColumn, SafeString::UInt32ToString(conditionValue), ret, file, line);
    }

    void Query(const int conditionColumn, const Int64 conditionValue, const fRowReturn& ret, const char* file = __FILE__, const int line = __LINE__) const {
        Query(conditionColumn, SafeString::Int64ToString(conditionValue), ret, file, line);
    }

    void Query(const int conditionColumn, const UInt64 conditionValue, const fRowReturn& ret, const char* file = __FILE__, const int line = __LINE__) const {
        Query(conditionColumn, SafeString::UInt64ToString(conditionValue), ret, file, line);
    }

    virtual void Query(const int conditionColumn, const std::string& conditionValue, const fRowReturn& ret, const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual void Delete(const int conditionColumn, const std::string& conditionValue, const fRowReturn& ret, const char* file = __FILE__, const int line = __LINE__) = 0;
};

class ISQLCache : public Api::iModule {
public:
    virtual ~ISQLCache() {}

    virtual ISQLCacheRow* CreateTempRow(const eTable tab, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual void ReleaseTempRow(ISQLCacheRow*& row, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual ISQLCacheTable* CreateTable(const eTable table, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual void ReleaseTable(ISQLCacheTable*& table) = 0;
};

#endif //__ICombat_h__
