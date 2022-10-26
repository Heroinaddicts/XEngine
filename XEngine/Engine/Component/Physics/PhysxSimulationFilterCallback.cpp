#include "PhysxSimulationFilterCallback.h"

namespace XEngine {
	PxFilterFlags PhysxSimulationFilterCallback::pairFound(
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

	void PhysxSimulationFilterCallback::pairLost(
		PxU32 pairID, 
		PxFilterObjectAttributes attributes0, 
		PxFilterData filterData0, 
		PxFilterObjectAttributes attributes1, 
		PxFilterData filterData1, 
		bool objectRemoved){

	}

	bool PhysxSimulationFilterCallback::statusChange(PxU32& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags) {
		return false;
	}
}
