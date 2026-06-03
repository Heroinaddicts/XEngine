#ifndef __NavMeshValidatorHeader_h__
#define __NavMeshValidatorHeader_h__

#include "MultiSys.h"
#include "SafeString.h"
#include "SafeSystem.h"
#include "Vector3.h"
#include "json.h"
#include "NavMesh.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <vector>

UsingXEngine;

struct ValidationSummary {
    int _TotalCount = 0;
    int _PassedCount = 0;
    int _FailedCount = 0;
};

#endif //__NavMeshValidatorHeader_h__
