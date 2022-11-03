#ifndef __GameObject_h__
#define __GameObject_h__

#include "Header.h"

namespace XEngine {
    class PhysxComponent;

    class GameObject : public Api::iGameObject {
    public:
        virtual ~GameObject() {}
        GameObject() : _IsReleased(false), _Position(0, 0, 0) {}

        // Í¨¹ý iGameObject ¼Ì³Ð
        virtual void OnCreate() override;
        virtual void OnUpdate() override;
        virtual void OnFixedUpdate() override;
        virtual void OnDestroy() override;

        __forceinline void SetPosition(const Vector3& pos) { _Position = pos; }
        __forceinline const Vector3& GetPosition() const { return _Position; }

        virtual bool AddComponent(Api::iComponent* component) override;
        virtual void RemoveIComponent(const char* type, Api::iComponent* component) override;

    protected:
        virtual Api::iComponent* GetComponent(const std::string& type) const override;
        __forceinline const std::set<Api::iComponent*>* GetComponents(const std::string& type) const {
            auto itor = _ComponentMap.find(type);
            if (itor == _ComponentMap.end()) {
                return nullptr;
            }

            return &(itor->second);
        }

    private:
        bool _IsReleased;

        std::map<std::string, std::set<Api::iComponent*>> _ComponentMap;
        std::map<std::string, std::set<Api::iComponent*>> _NeedAddComponentMap;
        std::map<std::string, std::set<Api::iComponent*>> _NeedRemoveComponentMap;
        Vector3 _Position;

        PhysxComponent* _PhysxComponent;
    };
}

#endif //__GameObject_h__
