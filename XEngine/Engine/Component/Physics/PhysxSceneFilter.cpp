#include "PhysxSceneFilter.h"

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

    PhysxSceneRaycastFilter::PhysxSceneRaycastFilter(physx::PxActor* filterActor, bool blocking, unsigned int mask, eQueryTriggerInteraction interaction)
        : _FilterActor(filterActor), _Mask(mask), _PostFilterHitType(PxQueryHitType::eBLOCK), _HitTriggers(true) {

    }

    physx::PxQueryHitType::Enum PhysxSceneRaycastFilter::preFilter(
        const physx::PxFilterData& filterData,
        const physx::PxShape* shape,
        const physx::PxRigidActor* actor,
        physx::PxHitFlags& filterFlags) {
        return physx::PxQueryHitType::Enum();
    }

    physx::PxQueryHitType::Enum PhysxSceneRaycastFilter::postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit) {
        return physx::PxQueryHitType::Enum();
    }
}
