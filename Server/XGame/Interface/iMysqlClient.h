#ifndef __iMysqlClient_h__
#define __iMysqlClient_h__

#include "iModule.h"
using namespace XEngine;

class iMysqlPipe {
public:
    virtual ~iMysqlPipe() {}
};

class iMysqlSession {
public:
    virtual ~iMysqlSession() {}
    iMysqlSession() : _Pipe(nullptr) {}

    virtual void OnConnected(bool success) = 0;

    iMysqlPipe* const _Pipe;
};

class iMysqlClient : public Api::iModule {
public:
    virtual ~iMysqlClient() {}

    virtual void ConnectMysql(iMysqlSession* session, const std::string& host, const int port, const std::string& username, const std::string& password) = 0;
};

#endif //__iMysqlClient_h__
