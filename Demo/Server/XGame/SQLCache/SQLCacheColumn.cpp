#include "SQLCacheColumn.h"
#include "SQLCacheRow.h"

static XPool<SQLCacheColumn> s_Pool;
SQLCacheColumn* SQLCacheColumn::CreateSQLCacheColumn(SQLCacheRow* const host, const TableTemplate* tableT, const ColumnTemplate* t, const char* file, const int line) {
    XASSERT(host, "wtf");
    return XPOOL_CREATE(s_Pool, host, tableT, t, file, line);
}

void SQLCacheColumn::ReleaseSQLCacheColumn(SQLCacheColumn*& column) {
    XPOOL_RELEASE(s_Pool, column);
    column = nullptr;
}

ISQLCacheRow* SQLCacheColumn::Host() const {
    return _Host;
}

bool SQLCacheColumn::SetInt32(const Int32 value, const char* file, const int line) {
    XASSERT(_Template->_Type == eColumnType::__Int32__, "SQLCacheColumn::SetInt32 Type Error %s:%d", file, line);
    if (_Template->_Type != eColumnType::__Int32__) {
        ErrorLog(g_Engine, "SQLCacheColumn::SetSetInt32 Type Error %s:%d", file, line);
        return false;
    }

    if (value == _Int32) {
        _HasValue = true;
        return true;
    }

    if (_Template->_Unique) {
        if (_Host->Host()) {
            if (_Host->Host()->IsColumnExists(_Template->_Index, SafeString::Int32ToString(value))) {
                ErrorLog(g_Engine, "SetColumnValue Int32 Column value %s already exists | %s:%d", GetValueString().c_str(), file, line);
                return false;
            }
        }
    }
    _Int32 = value;
    TraceLog(g_Engine, "Table %s SetColumn %s Value %s", _TableTemplate->_Name.c_str(), _Template->_Name.c_str(), GetValueString().c_str());
    _HasValue = true;
    return true;
}

bool SQLCacheColumn::SetUInt32(const UInt32 value, const char* file, const int line) {
    XASSERT(_Template->_Type == eColumnType::__UInt32__, "SQLCacheColumn::SetUInt32 Type Error %s:%d", file, line);
    if (_Template->_Type != eColumnType::__UInt32__) {
        ErrorLog(g_Engine, "SQLCacheColumn::SetSetUInt32 Type Error %s:%d", file, line);
        return false;
    }

    if (value == _UInt32) {
        _HasValue = true;
        return true;
    }

    if (_Template->_Unique) {
        if (_Host->Host()) {
            if (_Host->Host()->IsColumnExists(_Template->_Index, SafeString::UInt32ToString(value))) {
                ErrorLog(g_Engine, "SetColumnValue Int32 Column value %s already exists | %s:%d", GetValueString().c_str(), file, line);
                return false;
            }
        }
    }
    _UInt32 = value;
    TraceLog(g_Engine, "Table %s SetColumn %s Value %s", _TableTemplate->_Name.c_str(), _Template->_Name.c_str(), GetValueString().c_str());
    _HasValue = true;
    return true;
}

bool SQLCacheColumn::SetInt64(const Int64 value, const char* file, const int line) {
    XASSERT(_Template->_Type == eColumnType::__Int64__, "SQLCacheColumn::SetInt64 Type Error %s:%d", file, line);
    if (_Template->_Type != eColumnType::__Int64__) {
        ErrorLog(g_Engine, "SQLCacheColumn::SetSetInt64 Type Error %s:%d", file, line);
        return false;
    }

    if (value == _Int64) {
        _HasValue = true;
        return true;
    }

    if (_Template->_Unique) {
        if (_Host->Host()) {
            if (_Host->Host()->IsColumnExists(_Template->_Index, SafeString::Int64ToString(value))) {
                ErrorLog(g_Engine, "SetColumnValue Int32 Column value %s already exists | %s:%d", GetValueString().c_str(), file, line);
                return false;
            }
        }
    }
    _Int64 = value;
    TraceLog(g_Engine, "Table %s SetColumn %s Value %s", _TableTemplate->_Name.c_str(), _Template->_Name.c_str(), GetValueString().c_str());
    _HasValue = true;
    return true;
}

bool SQLCacheColumn::SetUInt64(const UInt64 value, const char* file, const int line) {
    XASSERT(_Template->_Type == eColumnType::__UInt64__, "SQLCacheColumn::SetUInt64 Type Error %s:%d", file, line);
    if (_Template->_Type != eColumnType::__UInt64__) {
        ErrorLog(g_Engine, "SQLCacheColumn::SetSetUInt64 Type Error %s:%d", file, line);
        return false;
    }

    if (value == _UInt64) {
        _HasValue = true;
        return true;
    }

    if (_Template->_Unique) {
        if (_Host->Host()) {
            if (_Host->Host()->IsColumnExists(_Template->_Index, SafeString::UInt64ToString(value))) {
                ErrorLog(g_Engine, "SetColumnValue Int32 Column value %s already exists | %s:%d", GetValueString().c_str(), file, line);
                return false;
            }
        }
    }
    _UInt64 = value;
    TraceLog(g_Engine, "Table %s SetColumn %s Value %s", _TableTemplate->_Name.c_str(), _Template->_Name.c_str(), GetValueString().c_str());
    _HasValue = true;
    return true;
}

bool SQLCacheColumn::SetFloat(const float value, const char* file, const int line) {
    XASSERT(_Template->_Type == eColumnType::__Float__, "SQLCacheColumn::SetFloat Type Error %s:%d", file, line);
    if (_Template->_Type != eColumnType::__Float__) {
        ErrorLog(g_Engine, "SQLCacheColumn::SetSetFloat Type Error %s:%d", file, line);
        return false;
    }

    if (value == _Float) {
        _HasValue = true;
        return true;
    }

    _Float = value;
    TraceLog(g_Engine, "Table %s SetColumn %s Value %s", _TableTemplate->_Name.c_str(), _Template->_Name.c_str(), GetValueString().c_str());
    _HasValue = true;
    return true;
}

bool SQLCacheColumn::SetString(const std::string& value, const char* file, const int line) {
    XASSERT(_Template->_Type == eColumnType::__String__, "SQLCacheColumn::SetString Type Error %s:%d", file, line);
    if (_Template->_Type != eColumnType::__String__) {
        ErrorLog(g_Engine, "SQLCacheColumn::SetSetString Type Error %s:%d", file, line);
        return false;
    }

    if (value == _String) {
        _HasValue = true;
        return true;
    }

    if (_Template->_Unique) {
        if (_Host->Host()) {
            if (_Host->Host()->IsColumnExists(_Template->_Index, value)) {
                ErrorLog(g_Engine, "SetColumnValue Int32 Column value %s already exists | %s:%d", GetValueString().c_str(), file, line);
                return false;
            }
        }
    }
    _String = value;
    TraceLog(g_Engine, "Table %s SetColumn %s Value %s", _TableTemplate->_Name.c_str(), _Template->_Name.c_str(), GetValueString().c_str());
    _HasValue = true;
    return true;
}

bool SQLCacheColumn::SetBool(const bool value, const char* file, const int line) {
    XASSERT(_Template->_Type == eColumnType::__Bool__, "SQLCacheColumn::SetBool Type Error %s:%d", file, line);
    if (_Template->_Type != eColumnType::__Bool__) {
        ErrorLog(g_Engine, "SQLCacheColumn::SetBool Type Error %s:%d", file, line);
        return false;
    }

    if (value == _Bool) {
        _HasValue = true;
        return true;
    }

    _Bool = value;
    TraceLog(g_Engine, "Table %s SetColumn %s Value %s", _TableTemplate->_Name.c_str(), _Template->_Name.c_str(), GetValueString().c_str());
    _HasValue = true;
    return true;
}

SQLCacheColumn::SQLCacheColumn(SQLCacheRow* const host, const TableTemplate* tableT, const ColumnTemplate* t, const char* file, const int line)
    : _Host(host), _TableTemplate(tableT), _Template(t), _File(file), _Line(line), _UInt64(0), _HasValue(false) {
}
