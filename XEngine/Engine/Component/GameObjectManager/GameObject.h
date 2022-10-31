#ifndef __GameObject_h__
#define __GameObject_h__

#include "Header.h"

namespace XEngine {
    class GameObject : public Api::iGameObject {
    public:
        virtual ~GameObject() {}
        GameObject() : _IsInited(false), _IsReleased(false) {}

        // Í¨¹ý iGameObject ¼Ì³Ð
        virtual void OnCreate() override;
        virtual void OnStart() override;
        virtual void OnUpdate() override;
        virtual void OnFixedUpdate() override;
        virtual void OnDestroy() override;

        __forceinline const std::map<std::string, Api::iComponent*>& GetComponents() const { return _ComponentMap; }

    protected:
        virtual bool AddComponent(const std::string& type_name, Api::iComponent* component, const char* file, const int line) override;
        virtual Api::iComponent* GetComponent(const std::string& type_name) const override;

    private:
        bool _IsInited;
        bool _IsReleased;

        std::map<std::string, Api::iComponent*> _ComponentMap;

    };
}

#endif //__GameObject_h__
