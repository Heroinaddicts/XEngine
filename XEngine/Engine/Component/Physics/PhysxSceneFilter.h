#ifndef __PhysxSceneFilter_h__
#define __PhysxSceneFilter_h__

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


    class PhysxSceneRaycastFilter : public physx::PxQueryFilterCallback {
    public:
        PhysxSceneRaycastFilter(physx::PxActor* filterActor, bool blocking, unsigned int mask, eQueryTriggerInteraction interaction);
        __forceinline bool IsShapeIgnored(const physx::PxShape* shape, const physx::PxRigidActor* actor) const {
            if (actor == _FilterActor) {
                return true;
            }

            if (shape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE && !_HitTriggers) {
                return true;
            }

            return false;
        }

        virtual physx::PxQueryHitType::Enum preFilter(
            const physx::PxFilterData& filterData,
            const physx::PxShape* shape,
            const physx::PxRigidActor* actor,
            physx::PxHitFlags& filterFlags);

        virtual physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit);

    private:
        const physx::PxQueryHitType::Enum _PostFilterHitType;
        physx::PxActor* const _FilterActor;
        const unsigned int _Mask;
        const bool _HitTriggers;
    };
}

#endif //__PhysxSceneFilter_h__
