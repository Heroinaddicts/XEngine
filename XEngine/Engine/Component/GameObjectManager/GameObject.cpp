#include "GameObject.h"

namespace XEngine {
    void GameObject::OnCreate() {

    }

    void GameObject::OnUpdate() {
        for (auto i = _ComponentMap.begin(); i != _ComponentMap.end(); i++) {
            for (auto j = i->second.begin(); j != i->second.end(); j++) {
                (*j)->OnUpdate();
            }
        }

        for (auto i = _NeedAddComponentMap.begin(); i != _NeedAddComponentMap.end(); i++) {
            const char* type = i->first.c_str();
            auto itor = _ComponentMap.find(type);
            if (itor == _ComponentMap.end()) {
                itor = _ComponentMap.insert(std::make_pair(type, std::set<Api::iComponent*>())).first;
            }

            for (auto j = i->second.begin(); j != i->second.end(); j++) {
                itor->second.insert(*j);
            }
        }
        _NeedAddComponentMap.clear();

        for (auto i = _NeedRemoveComponentMap.begin(); i != _NeedRemoveComponentMap.end(); i++) {
            const char* type = i->first.c_str();
            auto itor = _ComponentMap.find(type);
            if (itor == _ComponentMap.end()) {
                itor = _ComponentMap.insert(std::make_pair(type, std::set<Api::iComponent*>())).first;
            }

            for (auto j = i->second.begin(); j != i->second.end(); j++) {
                itor->second.erase(*j);
                iGameObject* temp = nullptr;
                SAFE_SET_CONST_VALUE((*j)->_GameObject, temp);
                (*j)->OnDestroy();
                xdel(*j);
            }
        }
        _NeedRemoveComponentMap.clear();
    }

    void GameObject::OnFixedUpdate() {
        if (!_IsReleased) {
            for (auto i = _ComponentMap.begin(); i != _ComponentMap.end(); i++) {
                for (auto j = i->second.begin(); j != i->second.end(); j++) {
                    (*j)->OnFixedUpdate();
                }
            }
        }
    }

    void GameObject::OnDestroy() {
        if (!_IsReleased) {
            _IsReleased = true;
            for (auto i = _ComponentMap.begin(); i != _ComponentMap.end(); i++) {
                for (auto j = i->second.begin(); j != i->second.end(); j++) {
                    (*j)->OnDestroy();
                    xdel(*j);
                }
            }
        }
    }

    __forceinline bool IsComponentExistsInMap(const std::string& type, const std::map<std::string, std::set<Api::iComponent*>>& componentMap, Api::iComponent* component) {
        auto itor = componentMap.find(type);
        if (componentMap.end() != itor) {
            if (itor->second.find(component) != itor->second.end()) {
                return true;
            }
        }

        return false;
    }

    bool GameObject::AddComponent(Api::iComponent* component) {
        std::string type = typeid(*component).name();
        if (IsComponentExistsInMap(type, _ComponentMap, component)
            || IsComponentExistsInMap(type, _NeedAddComponentMap, component)
            || IsComponentExistsInMap(type, _NeedRemoveComponentMap, component)) {
            XASSERT(false, "double add component");
            return false;
        }

        auto itor = _NeedAddComponentMap.find(type);
        if (_NeedAddComponentMap.end() == itor) {
            itor = _NeedAddComponentMap.insert(std::make_pair(type, std::set<Api::iComponent*>())).first;
        }

        itor->second.insert(component);
        iGameObject* temp = dynamic_cast<iGameObject*>(this);
        SAFE_SET_CONST_VALUE(component->_GameObject, temp);
        component->OnCreate();
        return true;
    }

    void GameObject::RemoveIComponent(const char* type, Api::iComponent* component) {
        auto itor = _NeedRemoveComponentMap.find(type);
        if (itor == _NeedRemoveComponentMap.end()) {
            itor = _NeedRemoveComponentMap.insert(std::make_pair(type, std::set<Api::iComponent*>())).first;
        }

        itor->second.insert(component);
    }

    Api::iComponent* GameObject::GetComponent(const std::string& type) const {
        auto itor = _ComponentMap.find(type);
        if (itor == _ComponentMap.end() || itor->second.empty()) {
            return nullptr;
        }

        return *(itor->second.begin());
    }
}
