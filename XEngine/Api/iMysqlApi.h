#ifndef __iMysqlApi_h__
#define __iMysqlApi_h__

#include "MultiSys.h"

namespace XEngine {
    namespace Api {
        class iMysqlSession;
        typedef std::function<void(const int code, const std::vector<std::vector<std::string>>* const result)> fQueryCallback;
        typedef std::function<void(iMysqlSession*)> fConnectMysqlCallback;

        __forceinline std::string EscapeSql(const std::string& input) {
            std::string output;
            for (char c : input) {
                if (c == '\'' || c == '\\')
                    output += '\\';
                output += c;
            }
            return output;
        }

#define MYSQL_CODE_SUCCESS 0
        class iMysqlSession {
        public:
            virtual ~iMysqlSession() {}

            virtual void Query(const std::string& sql, const fQueryCallback& fun) = 0;
            virtual void Close() = 0;
        };

        class iMysqlApi {
        public:
            virtual ~iMysqlApi() {}
            virtual void ConnectMysql(const std::string& host, const int port, const std::string& db, const std::string& username, const std::string& password, fConnectMysqlCallback callback) = 0;
            virtual void FreeQueryResult(const std::vector<std::vector<std::string>>* const result) = 0;
        };
    }
}

#endif //__iMysqlApi_h__
