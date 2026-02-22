#ifndef __MysqlHeader_h__
#define __MysqlHeader_h__

#include "iMysql.h"
#include "Mysql/mysql.h"
#include "Mysql/errmsg.h"

#include "iTaskApi.h"

#include "SafeString.h"
#include "SafeThread.h"
#include "SafeQueue.h"
#include "XPool.h"
#include "XLock.h"

#include <list>

namespace XEngine {
    class MysqlSession;
    struct QueryInfo {
        MysqlSession* _Session;
        std::string _Sql;
        Api::fQueryCallback _Callback;
    };

    struct ResultInfo {
        QueryInfo* _QueryInfo;
        int _Code;
        std::vector<std::vector<std::string>> * _Data;
        ResultInfo() : _QueryInfo(nullptr), _Code(0xFFFFFFFF), _Data(nullptr) {}
    };
}

#endif //__MysqlHeader_h__
