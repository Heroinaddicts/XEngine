#ifndef __SQLCache_h__
#define __SQLCache_h__

#include "Header.h"

class SQLCache : public ISQLCache {
public:
    virtual ~SQLCache() {}

    // 通过 ISQLCache 继承
    ISQLCacheRow* CreateTempRow(const eTable tab, const char* file, const int line) override;
    void ReleaseTempRow(ISQLCacheRow*& row, const char* file, const int line) override;
    ISQLCacheTable* CreateTable(const eTable table, const char* file, const int line) override;
    void ReleaseTable(ISQLCacheTable*& table) override;

private:
    // 通过 ISQLCache 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;



};

#endif //__SQLCache_h__
