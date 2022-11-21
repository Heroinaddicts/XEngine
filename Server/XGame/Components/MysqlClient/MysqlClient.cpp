#include "MysqlClient.h"

iEngine* g_Engine = nullptr;
MysqlClient* g_MysqlClient = nullptr;

bool MysqlClient::Initialize(iEngine* const engine) {
    g_MysqlClient = this;
    g_Engine = engine;
    return true;
}

bool MysqlClient::Launch(iEngine* const engine) {
    mysql_init(nullptr);
    return true;
}

bool MysqlClient::Destroy(iEngine* const engine) {
    return true;
}

void MysqlClient::ConnectMysql(iMysqlSession* session, const std::string& host, const int port, const std::string& username, const std::string& password) {

}
