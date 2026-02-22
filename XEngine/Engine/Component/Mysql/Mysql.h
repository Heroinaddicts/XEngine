#ifndef __Mysql_h__
#define __Mysql_h__ 

#include "Header.h"

namespace XEngine {
    class Mysql : public iMysql {
    public:
        virtual ~Mysql() {}

        static Mysql* GetInstance();

        // Inherited via iMysql
        virtual void ConnectMysql(const std::string& host, const int port, const std::string& db, const std::string& username, const std::string& password, Api::fConnectMysqlCallback callback) override;
        virtual void FreeQueryResult(const std::vector<std::vector<std::string>>* const result) override;

        // Í¨¹ý iMysql ¼Ì³Ð
        bool Initialize(Api::iEngine* const engine) override;
        bool Launch(Api::iEngine* const engine) override;
        void Release(Api::iEngine* const engine) override;
        void EarlyUpdate(Api::iEngine* const engine) override;
        void Update(Api::iEngine* const engine) override;
        void LaterUpdate(Api::iEngine* const engine) override;
    };

}
#endif //__Mysql_h__
