#include "SQLCache.h"
#include "SQLCacheTable.h"

Api::iEngine* g_Engine = nullptr;
static SQLCache* s_Self = nullptr;

bool SQLCache::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool SQLCache::Launch(Api::iEngine* const engine) {
    return true;
}

bool SQLCache::Destroy(Api::iEngine* const engine) {
    return true;
}

ISQLCacheRow* SQLCache::CreateTempRow(const eTable tab, const char* file, const int line) {
    return SQLCacheRow::CreateSQLCacheRow(nullptr, GetTableTemplate(tab), file, line);
}

void SQLCache::ReleaseTempRow(ISQLCacheRow*& row, const char* file, const int line) {
    SQLCacheRow::ReleaseSQLCacheRow((SQLCacheRow*&)row);
}

ISQLCacheTable* SQLCache::CreateTable(const eTable table, const char* file, const int line) {
    return SQLCacheTable::CreateSQLCacheTable(GetTableTemplate(table), file, line);
}

void SQLCache::ReleaseTable(ISQLCacheTable*& table) {
    SQLCacheTable::ReleaseSQLCacheTable((SQLCacheTable*&)table);
}

