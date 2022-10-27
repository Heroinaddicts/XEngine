#include "PhysxSceneFilterCallback.h"

namespace XEngine {
    PhysxSceneFilterCallback* PhysxSceneFilterCallback::GetInstance() {
        static PhysxSceneFilterCallback s_Instance;
        return &s_Instance;
    }

    PxFilterFlags PhysxSceneFilterCallback::pairFound(
        PxU32 pairID,
        PxFilterObjectAttributes attributes0,
        PxFilterData filterData0,
        const PxActor* a0,
        const PxShape* s0,
        PxFilterObjectAttributes attributes1,
        PxFilterData filterData1,
        const PxActor* a1,
        const PxShape* s1,
        PxPairFlags& pairFlags) {

        return PxFilterFlags();
    }

    void PhysxSceneFilterCallback::pairLost(
        PxU32 pairID,
        PxFilterObjectAttributes attributes0,
        PxFilterData filterData0,
        PxFilterObjectAttributes attributes1,
        PxFilterData filterData1,
        bool objectRemoved) {

    }

    bool PhysxSceneFilterCallback::statusChange(PxU32& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags) {
        return false;
    }

    PxQueryHitType::Enum PhysxSceneFilterCallback::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) {
        return PxQueryHitType::Enum();
    }

    PxQueryHitType::Enum PhysxSceneFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit& hit) {
        return PxQueryHitType::Enum();
    }
}
