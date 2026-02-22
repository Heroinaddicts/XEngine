#ifndef __SQLCacheRow_h__
#define __SQLCacheRow_h__

#include "Header.h"
#include "SQLCacheColumn.h"

class SQLCacheTable;
class SQLCacheRow : public ISQLCacheRow {
public:
    virtual ~SQLCacheRow();

    SQLCacheTable* const _Host;
    const TableTemplate* const _Template;
    const char* const _File;
    const int _Line;

    static SQLCacheRow* CreateSQLCacheRow(SQLCacheTable* const host, const TableTemplate* const tableTemplate, const char* file, const int line);
    static void ReleaseSQLCacheRow(SQLCacheRow*& row);

    // Í¨¹ý ISQLCacheRow ¼Ì³Ð
    ISQLCacheTable* Host() const override;
    ISQLCacheColumn* GetColumn(const int column) const override;

private:
    friend XPool<SQLCacheRow>;
    SQLCacheRow(SQLCacheTable* const host, const TableTemplate* const tableTemplate, const char* file, const int line);

private:
    SQLCacheColumn** const _Columns;
    int _Index;

};

#endif //__SQLCacheRow_h__
