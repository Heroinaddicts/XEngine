#ifndef __NodeProto_h__
#define __NodeProto_h__

#include "MultiSys.h"
#include "SafeMemory.h"

namespace NodeProto {
    enum eID {
        NodeReport = 1,
        HeartBeat = 2,

        //以下是开发者自定义协议
        DeveloperDefined = 101,

    };
#pragma pack(push, 1)
    struct MessageHeader {
        unsigned_int16 _Len;
        unsigned_int16 _MsgId;
    };

    struct oNodeReport {
#define oNodeReport_Name_Len 64
        unsigned_int32 _Id;
        char _Name[oNodeReport_Name_Len];

        void SetName(const char* name) {
            SafeMemory::Memcpy(_Name, sizeof(_Name), name, strlen(name) + 1);
            _Name[oNodeReport_Name_Len - 1] = 0;
        }
    };
#pragma pack(pop)


}

#endif //__NodeProto_h__
