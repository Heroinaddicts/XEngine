#ifndef __RedisOperator_h__
#define __RedisOperator_h__

#include "Header.h"

#define SAFE_FREE_OPERATOR_DATA(opt) {\
    if (opt->_Data) { \
        xfree(opt->_Data); \
        opt->_Data = nullptr; \
    }\
    opt->_Len = 0;\
}

class RedisOperator {
public:
    enum class eOperatorType {
        Read,
        Write,
        Delete
    };
public:
    virtual ~RedisOperator() {
        SAFE_FREE_OPERATOR_DATA(this);
    }

    static RedisOperator* Create(const std::string& key, const int logicID, const eOperatorType type, const unsigned_int64 context);
    static void Release(RedisOperator* opt);

private:
    friend XPool<RedisOperator>;
    RedisOperator(const std::string& key, const int logicID, const eOperatorType type, const unsigned_int64 context)
        :_Key(key), _LogicID(logicID), _Type(type), _Context(context), _Success(false), _Data(nullptr), _Len(0) {}

public:
    const std::string _Key;
    const int _LogicID;
    const eOperatorType _Type;
    const unsigned_int64 _Context;

    bool _Success;
    void* _Data;
    int _Len;
};

#endif //__RedisOperator_h__
