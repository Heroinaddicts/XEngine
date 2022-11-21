#ifndef __MysqlClient_h__
#define __MysqlClient_h__ 

#include "Header.h"

class MysqlClient : public iMysqlClient {
public:
    virtual ~MysqlClient() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);

    // Í¨¹ý iMysqlClient ¼Ì³Ð
    virtual void ConnectMysql(iMysqlSession* session, const std::string& host, const int port, const std::string& username, const std::string& password) override;
};

#endif //__MysqlClient_h__
