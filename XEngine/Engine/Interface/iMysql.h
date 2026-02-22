#ifndef __iMysql_h__
#define __iMysql_h__

#include "iEngineComponent.h"
#include "iMysqlApi.h"

namespace XEngine {
    class iMysql : public iEngineComponent, public Api::iMysqlApi {
    public:
        virtual ~iMysql() {}
    };
}

#endif //__iLogic_h__
