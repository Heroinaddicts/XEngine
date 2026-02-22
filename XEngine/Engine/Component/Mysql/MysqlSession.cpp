#include "MysqlSession.h"
#include "Engine.h"
namespace XEngine {
    XPool<MysqlSession> MysqlSession::s_Pool;

    MysqlSession* MysqlSession::Create(const std::string& host, const int port, const std::string& db, const std::string& username, const std::string& password, MYSQL* con) {
        return XPOOL_CREATE(s_Pool, host, db, username, password, port, con);
    }

    void MysqlSession::Release(MysqlSession* session) {
        DEBUG(g_Engine, "Mysql Session Released");
        XPOOL_RELEASE(s_Pool, session);
    }

    void MysqlSession::Query(const std::string& sql, const Api::fQueryCallback& fun) {
        if (_IsClose) {
            fun(-1, nullptr);
            return;
        }

        QueryInfo* info = txnew QueryInfo();
        info->_Session = this;
        info->_Sql = sql;
        info->_Callback = fun;

        _QueryQueue.Push(info);
    }

    MysqlSession::MysqlSession(const std::string& host, const std::string& db, const std::string& username, const std::string& password, const int port, MYSQL* con)
        : _Host(host), _Db(db), _Port(port), _Username(username), _Password(password), _Con(con), _Closed(false) {
        Start();
    }

    MysqlSession::~MysqlSession() {
        
    }

    void MysqlSession::Update(const bool all) {
        ResultInfo* ret;
        UInt64 tick = SafeSystem::Time::GetMicroSecond();
        while (_ResultQueue.Pull(ret)) {
            ret->_QueryInfo->_Callback(ret->_Code, ret->_Data);
            txdel ret->_QueryInfo;
            txdel ret;
            if (SafeSystem::Time::GetMicroSecond() - tick > 1000 && !all) {
                break;
            }
        }
    }

    void MysqlSession::Run(void* context) {
        while (!IsClose()) {
            Deal();
        }
    }

    void MysqlSession::Close() {
        CloseThread();
        while (GetThreadCount() > 0)
            SafeSystem::Time::MillisecondSleep(1);
        Deal();
        Update(true);
        mysql_close(_Con);
        _Con = nullptr;
        _Closed = true;
    }

    void MysqlSession::Deal()
    {
        QueryInfo* query;
        bool needSleep = true;
        while (_QueryQueue.Pull(query)) {
            needSleep = false;
            ResultInfo* ret = txnew ResultInfo();
            ret->_QueryInfo = query;
            ret->_Code = 0;
            do {
                while (mysql_query(query->_Session->_Con, query->_Sql.c_str()) != 0) {
                    ret->_Code = mysql_errno(query->_Session->_Con);
                    if (CR_SERVER_GONE_ERROR == ret->_Code || CR_SERVER_LOST == ret->_Code) {
                        if (nullptr == mysql_real_connect(query->_Session->_Con, query->_Session->_Host.c_str(), query->_Session->_Username.c_str(), query->_Session->_Password.c_str(), nullptr, query->_Session->_Port, nullptr, CLIENT_MULTI_STATEMENTS)) {
                            mysql_close(query->_Session->_Con);
                            query->_Session->_Con = mysql_init(nullptr);
                            if (nullptr == query->_Session->_Con) {
                                SYSERR(g_Engine, "mysql_init error");
                                break;
                            }

                            if (mysql_real_connect(query->_Session->_Con, query->_Session->_Host.c_str(), query->_Session->_Username.c_str(), query->_Session->_Password.c_str(), query->_Session->_Db.c_str(), query->_Session->_Port, nullptr, CLIENT_MULTI_STATEMENTS) == nullptr) {
                                SYSERR(g_Engine, "mysql_real_connect error %u: %s", mysql_errno(query->_Session->_Con), mysql_error(query->_Session->_Con));
                                break;
                            }

                            bool reconnect = true;
                            mysql_optionsv(query->_Session->_Con, MYSQL_OPT_RECONNECT, (void*)&reconnect);
                            break;
                        }
                    }
                    else {
                        break;
                    }
                }
            } while (ret->_Code == CR_SERVER_GONE_ERROR || CR_SERVER_LOST == ret->_Code);

            if (ret->_Code == 0) {
                ret->_Data = txnew std::vector<std::vector<std::string>>();
                MYSQL_RES* result = mysql_store_result(query->_Session->_Con);
                while (result) {
                    if (nullptr != result) {
                        MYSQL_ROW row;
                        while ((row = mysql_fetch_row(result)) != nullptr) {
                            std::vector<std::string> rowData;
                            int fieldCount = mysql_num_fields(result);
                            for (int i = 0; i < fieldCount; i++) {
                                rowData.push_back(row[i] ? row[i] : "nullptr");
                            }
                            ret->_Data->push_back(rowData);
                        }
                        mysql_free_result(result);
                        result = nullptr;
                    }
                    else {
                        ret->_Code = mysql_errno(query->_Session->_Con);
                        if (0 != ret->_Code) {
                            ErrorLog(g_Engine, "mysql_store_result error %u: %s", mysql_errno(query->_Session->_Con), mysql_error(query->_Session->_Con));
                        }
                    }

                    mysql_next_result(query->_Session->_Con);
                    result = mysql_store_result(query->_Session->_Con);
                }
            }
            else {
                ErrorLog(g_Engine, "mysql_query error %s %u: %s", query->_Sql.c_str(), mysql_errno(query->_Session->_Con), mysql_error(query->_Session->_Con));
            }

            _ResultQueue.Push(ret);
        }

        if (needSleep)
            SafeSystem::Time::MillisecondSleep(2);
    }
}
