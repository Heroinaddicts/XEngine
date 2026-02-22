#ifndef __MysqlSession_h__
#define __MysqlSession_h__

#include "Header.h"

namespace XEngine {
    class MysqlSession : public Api::iMysqlSession, public SafeThread {
    public:
        virtual ~MysqlSession();
        const std::string _Host;
        const int _Port;
        const std::string _Db;
        const std::string _Username;
        const std::string _Password;
        MYSQL* _Con;

        __forceinline bool Closed() {
            return _Closed;
        }

    public:
        static MysqlSession* Create(const std::string& host, const int port, const std::string& db, const std::string& username, const std::string& password, MYSQL* con);
        static void Release(MysqlSession* session);
        void Query(const std::string& sql, const Api::fQueryCallback& fun) override;

        void Update(const bool all = false);

        void Run(void* context) override;
        void Close() override;

    private:
        friend XPool<MysqlSession>;
        MysqlSession(const std::string& host, const std::string& db, const std::string& username, const std::string& password, const int port, MYSQL* con);
        void Deal();

    private:
        static XPool<MysqlSession> s_Pool;
        SafeQueue::SpscQueue<QueryInfo*, 81920> _QueryQueue;
        SafeQueue::SpscQueue<ResultInfo*, 81920> _ResultQueue;
        bool _Closed;
    };
}

#endif //__MysqlSession_h__
