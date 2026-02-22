#ifndef __IBackpack_h__
#define __IBackpack_h__

#include "iModule.h"
UsingXEngine;

#define INVALID_EQUIP_ID 0

class IBackpack : public Api::iModule {
public:
    virtual ~IBackpack() {}

    virtual bool Equip(const UInt64 characterGuid, const UInt64 equipGuid) = 0;
    virtual bool UnEquip(const UInt64 characterGuid, const UInt64 equipGuid) = 0;
    virtual bool QueryResourceByID(const UInt64 characterGuid, const UInt64 resourceId) = 0;
    virtual void GetRewardItems(const UInt64 characterGuid, const std::vector<Int64>& itemResourceIds) = 0;
};

#endif //__IBackpack_h__


