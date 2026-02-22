#ifndef __Db_h__
#define __Db_h__

#include "Header.h"

class Db : public IDb {
public:
    virtual ~Db() {}

    // 通过 IDb 继承
    IDbGroup* CreateGroup(const int count,
        const std::string& mysqlhost, const int mysqlport, const std::string& mysqldb, const std::string& mysqlusername, const std::string& mysqlpassword,
        const std::string& redishost, const int redisport, const std::string& redispassword) override;
private:
    // 通过 IDb 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;
};

#endif //__Db_h__
