#ifndef __PhysxSceneFilterCallback_h__
#define __PhysxSceneFilterCallback_h__

#include "Header.h"

namespace XEngine {
    class PhysxSceneFilterCallback : public PxSimulationFilterCallback, public PxSceneQueryFilterCallback {
    public:
        virtual ~PhysxSceneFilterCallback() {}

        static PhysxSceneFilterCallback* GetInstance();

        // Í¨¹ý PxSimulationFilterCallback ¼Ì³Ð
        virtual PxFilterFlags pairFound(PxU32 pairID, PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor* a0, const PxShape* s0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor* a1, const PxShape* s1, PxPairFlags& pairFlags) override;
        virtual void pairLost(PxU32 pairID, PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, bool objectRemoved) override;
        virtual bool statusChange(PxU32& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags) override;

        virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags);
        virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit);

    };
}

#endif //__PhysxSceneFilterCallback_h__
