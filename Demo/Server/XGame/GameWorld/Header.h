#pragma once

#include "iTimerApi.h"

#include "IGameWorld.h"
#include "XPool.h"
#include "SafeTools.h"
#include "XEventPool.h"
#include <set>
#include <unordered_map>
UsingXEngine;

extern Api::iEngine* g_Engine;

struct AttributeCallback {
    std::set<IGameWorld::fUInt32AttributeChangeJudge> _UInt32Judge;
    std::set<IGameWorld::fInt32AttributeChangeJudge> _Int32Judge;
    std::set<IGameWorld::fUInt64AttributeChangeJudge> _UInt64Judge;
    std::set<IGameWorld::fInt64AttributeChangeJudge> _Int64Judge;
    std::set<IGameWorld::fFloatAttributeChangeJudge> _FloatJudge;
    std::set<IGameWorld::fStringAttributeChangeJudge> _StringJudge;
    std::set<IGameWorld::fBoolAttributeChangeJudge> _BoolJudge;
    std::set<IGameWorld::fVector2AttributeChangeJudge> _Vector2Judge;
    std::set<IGameWorld::fVector3AttributeChangeJudge> _Vector3Judge;

    std::set<IGameWorld::fUInt32AttributeChanged> _UInt32Changed;
    std::set<IGameWorld::fInt32AttributeChanged> _Int32Changed;
    std::set<IGameWorld::fUInt64AttributeChanged> _UInt64Changed;
    std::set<IGameWorld::fInt64AttributeChanged> _Int64Changed;
    std::set<IGameWorld::fFloatAttributeChanged> _FloatChanged;
    std::set<IGameWorld::fStringAttributeChanged> _StringChanged;
    std::set<IGameWorld::fBoolAttributeChanged> _BoolChanged;
    std::set<IGameWorld::fVector2AttributeChanged> _Vector2Changed;
    std::set<IGameWorld::fVector3AttributeChanged> _Vector3Changed;
};

extern std::unordered_map<eGameObjectType, AttributeCallback> g_AttributeCallback;
