#include "Mysql.h"
#include "MysqlSession.h"
#include "Engine.h"

namespace XEngine {
    static std::set<MysqlSession*> s_Sessions;

    Mysql* Mysql::GetInstance() {
        static Mysql s_Mysql;
        return &s_Mysql;
    }

    bool Mysql::Initialize(Api::iEngine* const engine) {
        return true;
    }

    bool Mysql::Launch(Api::iEngine* const engine) {
        return true;
    }

    void Mysql::Release(Api::iEngine* const engine) {
    }

    void Mysql::EarlyUpdate(Api::iEngine* const engine) {
    }

    void Mysql::Update(Api::iEngine* const engine) {
    }

    void Mysql::LaterUpdate(Api::iEngine* const engine) {
        static std::list<MysqlSession*> s_RecycleBin;
        for (auto itor = s_Sessions.begin(); itor != s_Sessions.end(); itor++) {
            if (!(*itor)->Closed())
                (*itor)->Update();
            else
                s_RecycleBin.push_back(*itor);
        }

        for (auto itor = s_RecycleBin.begin(); itor != s_RecycleBin.end(); itor++) {
            MysqlSession::Release(*itor);
            s_Sessions.erase(*itor);
        }

        s_RecycleBin.clear();
    }

    void Mysql::ConnectMysql(const std::string& host, const int port, const std::string& db, const std::string& username, const std::string& password, Api::fConnectMysqlCallback callback) {
        g_Engine->GetTaskApi()->PushTask(Api::eTaskProcessorType::OrderedQueue,
            [=](Api::iTaskContext& context)->UInt64 {
                MYSQL* con = mysql_init(nullptr);
                XASSERT(con, "wtf");
                if (nullptr == con) {
                    SYSERR(g_Engine, "mysql_init error");
                    return -1;
                }

                my_bool enforce = 0; // false
                mysql_options(con, MYSQL_OPT_SSL_ENFORCE, &enforce);

                my_bool verify = 0; // 不校验证书
                mysql_options(con, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, &verify);
                
                if (mysql_real_connect(con, host.c_str(), username.c_str(), password.c_str(), db.c_str(), port, nullptr, CLIENT_MULTI_STATEMENTS) == nullptr) {
                    SYSERR(g_Engine, "mysql_real_connect error %u: %s", mysql_errno(con), mysql_error(con));
                    return -1;
                }

                bool reconnect = true;
                mysql_optionsv(con, MYSQL_OPT_RECONNECT, (void*)&reconnect);

                context.Set(con);
                return MYSQL_CODE_SUCCESS;
            },
            [=](const UInt64 code, const Api::iTaskContext& context) {
                if (code == MYSQL_CODE_SUCCESS) {
                    MysqlSession* session = MysqlSession::Create(host, port, db, username, password, context.Get<MYSQL>());
                    if (nullptr == session) {
                        callback(nullptr);
                    }
                    else {
                        s_Sessions.insert(session);
                        callback(session);
                    }
                }
                else {
                    callback(nullptr);
                }
            }
        );
    }

    void Mysql::FreeQueryResult(const std::vector<std::vector<std::string>>* const result) {
        if (result) {
            txdel result;
        }
    }
}
