#include "RedisOperator.h"

static XPool<RedisOperator> s_RedisOperatorPool;

RedisOperator* RedisOperator::Create(const std::string& key, const int logicID, const eOperatorType type, const unsigned_int64 context) {
    return XPOOL_CREATE(s_RedisOperatorPool, key, logicID, type, context);
}

void RedisOperator::Release(RedisOperator* opt) {
    XPOOL_RELEASE(s_RedisOperatorPool, opt);
}
