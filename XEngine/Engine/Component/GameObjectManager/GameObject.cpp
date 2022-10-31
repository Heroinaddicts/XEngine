#include "GameObject.h"

namespace XEngine {
    bool GameObject::AddComponent(const std::string& type_name, Api::iComponent* component, const char* file, const int line) {
        if (_ComponentMap.find(type_name) != _ComponentMap.end()) {
            return false;
        }

        SafeMemory::Memcpy((void*)&(component->_File), sizeof(component->_File), &file, sizeof(file));
        SafeMemory::Memcpy((void*)&(component->_Line), sizeof(component->_Line), &line, sizeof(line));

        iGameObject* temp = this;
        SafeMemory::Memcpy((void*)&(component->_GameObject), sizeof(component->_GameObject), &temp, sizeof(temp));

        _ComponentMap.insert(std::make_pair(type_name, component));
        return true;
    }

    Api::iComponent* GameObject::GetComponent(const std::string& type_name) const {
        auto itor = _ComponentMap.find(type_name);
        if (_ComponentMap.end() == itor) {
            return nullptr;
        }

        return itor->second;
    }

    void GameObject::OnCreate() {
        for (auto i = _ComponentMap.begin(); i != _ComponentMap.end(); i++) {
            i->second->OnCreate();
        }
    }

    void GameObject::OnStart() {
        for (auto i = _ComponentMap.begin(); i != _ComponentMap.end(); i++) {
            i->second->OnStart();
        }
    }

    void GameObject::OnUpdate() {
        for (auto i = _ComponentMap.begin(); i != _ComponentMap.end(); i++) {
            if (!_IsInited && !_IsReleased) {
                i->second->OnCreate();
                i->second->OnStart();
            }

            i->second->OnUpdate();
        }

        _IsInited ? void(0) : _IsInited = true;
    }

    void GameObject::OnFixedUpdate() {
        if (_IsInited && !_IsReleased) {
            for (auto i = _ComponentMap.begin(); i != _ComponentMap.end(); i++) {
                i->second->OnFixedUpdate();
            }
        }
    }

    void GameObject::OnDestroy() {
        if (!_IsReleased) {
            _IsReleased = true;
            for (auto i = _ComponentMap.begin(); i != _ComponentMap.end(); i++) {
                i->second->OnDestroy();
            }
        }
    }
}
