#include "Db.h"
#include "DbGroup.h"

Api::iEngine* g_Engine;

bool Db::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    return true;
}

bool Db::Launch(Api::iEngine* const engine) {
    return true;
}

bool Db::Destroy(Api::iEngine* const engine) {
    return true;
}

IDbGroup* Db::CreateGroup(const int count,
    const std::string& mysqlhost, const int mysqlport, const std::string& mysqldb, const std::string& mysqlusername, const std::string& mysqlpassword,
    const std::string& redishost, const int redisport, const std::string& redispassword) {
    return DbGroup::Create(count, mysqlhost, mysqlport, mysqldb, mysqlusername, mysqlpassword, redishost, redisport, redispassword);
}
