#ifndef __SQLCacheTable_h__
#define __SQLCacheTable_h__

#include "Header.h"
#include "SQLCacheRow.h"

class SQLCacheTable : public ISQLCacheTable {
public:
    virtual ~SQLCacheTable();

    const TableTemplate* const _Template;
    const char* const _File;
    const int _Line;

    static SQLCacheTable* CreateSQLCacheTable(const TableTemplate* tableTemplate, const char* file, const int line);
    static void ReleaseSQLCacheTable(SQLCacheTable*& tab);

    // 通过 ISQLCacheTable 继承
    ISQLCacheRow* Insert(const fRowInit& init, const char* file, const int line) override;


    int RowCount() const override {
        return _Rows.size();
    }

    bool IsColumnExists(const int column, const std::string& value) const override {
        for (auto itor = _Rows.begin(); itor != _Rows.end(); itor++) {
            if ((*itor)->GetColumn(column)->GetValueString() == value) {
                return true;
            }
        }

        return false;
    }

    void Query(const int conditionColumn, const std::string& conditionValue, const fRowReturn& ret, const char* file, const int line) const override;
    void Delete(const int conditionColumn, const std::string& conditionValue, const fRowReturn& ret, const char* file, const int line) override;

private:
    friend XPool<SQLCacheTable>;
    SQLCacheTable(const TableTemplate* tableTemplate, const char* file, const int line);

    // 通过 ISQLCacheTable 继承
    __forceinline const TableTemplate* GetTemplate() const override { return _Template; }
private:
    std::list<SQLCacheRow*> _Rows;
};

#endif //__SQLCacheTable_h__
