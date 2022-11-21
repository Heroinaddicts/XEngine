#ifndef __MysqlPipe_h__
#define __MysqlPipe_h__

#include "Header.h"

class MysqlPipe : public iMysqlPipe {
public:
    enum class eErrorCode {
        UNKNOWN,
        CONNECTED,
        CONNECT_FAILD,
        AUTH_FAILD,
        DISCONNECTED
    };
public:
    virtual ~MysqlPipe() {}
    MysqlPipe(iMysqlSession* session, const std::string& host, const int port, const std::string& username, const std::string& password);

    void DoConnect();

private:
    const std::string _Host;
    const int _Port;
    const std::string _Username;
    const std::string _Password;

    MYSQL* _Handle;

    eErrorCode _Code;
    iMysqlSession* const _Session;
};

#endif //__MysqlPipe_h__
