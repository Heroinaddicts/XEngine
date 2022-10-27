#ifndef __PhysxSimulationFilterCallback_h__
#define __PhysxSimulationFilterCallback_h__

#include "Header.h"

namespace XEngine {
    class PhysxSimulationFilterCallback : public PxSimulationFilterCallback {
    public:
        virtual ~PhysxSimulationFilterCallback() {}

        static PhysxSimulationFilterCallback* GetInstance();

        // Í¨¹ý PxSimulationFilterCallback ¼Ì³Ð
        virtual PxFilterFlags pairFound(PxU32 pairID, PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor* a0, const PxShape* s0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor* a1, const PxShape* s1, PxPairFlags& pairFlags) override;
        virtual void pairLost(PxU32 pairID, PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, bool objectRemoved) override;
        virtual bool statusChange(PxU32& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags) override;
    };
}

#endif //__PhysxSimulationFilterCallback_h__
