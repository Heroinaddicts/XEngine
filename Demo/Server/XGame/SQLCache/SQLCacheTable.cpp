#include "SQLCacheTable.h"

static XPool<SQLCacheTable> s_Pool;

SQLCacheTable* SQLCacheTable::CreateSQLCacheTable(const TableTemplate* tableTemplate, const char* file, const int line) {
    return XPOOL_CREATE(s_Pool, tableTemplate, file, line);
}

SQLCacheTable::SQLCacheTable(const TableTemplate* tableTemplate, const char* file, const int line)
    : _Template(tableTemplate), _File(file), _Line(line) {
}

ISQLCacheRow* SQLCacheTable::Insert(const fRowInit& init, const char* file, const int line) {
    SQLCacheRow* row = SQLCacheRow::CreateSQLCacheRow(this, _Template, file, line);
    init(row);

    bool isUniqueExists = false;
    for (int column = 0; column < _Template->_ColumnCount; column++) {
        if (_Template->_ColumnTempates[column]._Unique) {
            if (IsColumnExists(column, row->GetColumn(column)->GetValueString())) {
                ErrorLog(g_Engine, "Table %s Column %s Value %s already exists", _Template->_Name.c_str(), _Template->_ColumnTempates[column]._Name.c_str(), row->GetColumn(column)->GetValueString().c_str());
                SQLCacheRow::ReleaseSQLCacheRow(row);
                return nullptr;
            }
        }
    }

    _Rows.push_back(row);
    return row;
}

void SQLCacheTable::Query(const int conditionColumn, const std::string& conditionValue, const fRowReturn& ret, const char* file, const int line) const {
    for (auto itor = _Rows.begin(); itor != _Rows.end(); itor++) {
        if ((*itor)->GetColumn(conditionColumn)->GetValueString() == conditionValue) {
            ret((*itor));
        }
    }
}

void SQLCacheTable::Delete(const int conditionColumn, const std::string& conditionValue, const fRowReturn& ret, const char* file, const int line) {
    for (auto itor = _Rows.begin(); itor != _Rows.end();) {
        if ((*itor)->GetColumn(conditionColumn)->GetValueString() == conditionValue) {
            ret((*itor));
            itor = _Rows.erase(itor);
        }
        else {
            ++itor;
        }
    }
}

SQLCacheTable::~SQLCacheTable() {
    for (auto itor = _Rows.begin(); itor != _Rows.end(); itor++) {
        SQLCacheRow::ReleaseSQLCacheRow(*itor);
    }
}

void SQLCacheTable::ReleaseSQLCacheTable(SQLCacheTable*& tab) {
    XPOOL_RELEASE(s_Pool, tab);
    tab = nullptr;
}
