#include "SQLCacheRow.h"
#include "SQLCacheColumn.h"
#include "SQLCacheTable.h"

static XPool<SQLCacheRow> s_Pool;

SQLCacheRow::SQLCacheRow(SQLCacheTable* const host, const TableTemplate* const tableTemplate, const char* file, const int line)
    : _Host(host), _Template(tableTemplate), _Columns(txnew SQLCacheColumn* [_Template->_ColumnCount]), _File(file), _Line(line)
{
    for (int i = 0; i < _Template->_ColumnCount; i++) {
        _Columns[i] = SQLCacheColumn::CreateSQLCacheColumn(this, _Template, &_Template->_ColumnTempates[i], file, line);
    }
}

ISQLCacheTable* SQLCacheRow::Host() const {
    return _Host;
}

ISQLCacheColumn* SQLCacheRow::GetColumn(const int column) const {
    return _Columns[column];
}

SQLCacheRow::~SQLCacheRow() {
    for (int i = 0; i < _Template->_ColumnCount; i++) {
        SQLCacheColumn::ReleaseSQLCacheColumn(_Columns[i]);
    }

    txdel[] _Columns;
}

SQLCacheRow* SQLCacheRow::CreateSQLCacheRow(SQLCacheTable* const host, const TableTemplate* const tableTemplate, const char* file, const int line) {
    return XPOOL_CREATE(s_Pool, host, tableTemplate, file, line);
}

void SQLCacheRow::ReleaseSQLCacheRow(SQLCacheRow*& row) {
    XPOOL_RELEASE(s_Pool, row);
    row = nullptr;
}
