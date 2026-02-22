#ifndef __AccountCache_h__
#define __AccountCache_h__

#include "Header.h"

class AccountCache : public Api::iTimer {
public:
    virtual ~AccountCache() {}


private:
    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;
    void OnEnd(const int id, const UInt64 context, bool nonviolent, const Int64 timestamp) override;
};

#endif //__AccountCache_h__
