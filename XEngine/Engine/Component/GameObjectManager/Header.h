#pragma once

#include "iGameObjectManager.h"
#include <list>
#include <set>
#include <map>
#include "SafeMemory.h"
#include "SafeString.h"

namespace XEngine {
    class GameObject;
	extern std::set<GameObject*> g_GameObjectSet;
	extern std::list<GameObject*> g_GameObjectCreated;
	extern std::set<GameObject*> g_GameObjectReleased;

    class iComponent;
}

