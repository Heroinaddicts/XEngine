#ifndef __iGameObjectApi_h__
#define __iGameObjectApi_h__

#include "MultiSys.h"
#include "Vector.h"
#include <string>
#include <set>
#include <vector>

namespace XEngine {
    namespace Api {
        class iObject {
        public:
            virtual ~iObject() {}
            iObject() : _File(nullptr), _Line(0), _Type(typeid(*this).name()) {}

            virtual void OnCreate() {}
            virtual void OnUpdate() {}
            virtual void OnFixedUpdate() {}
            virtual void OnDestroy() {} //do not destroy other object at OnDestroy

            const std::string _Type;
            const char* const _File;
            const int _Line;
        };

        class iGameObject;
        class iComponent : public iObject {
        public:
            virtual ~iComponent() {}
            iComponent() : _GameObject(nullptr) {}

            virtual void OnTrigger() {}
            virtual void OnCollisionEnter() {}

            iGameObject* const _GameObject;
        };

        class iPhysxComponent;
        class iGameObject : public iObject {
        public:
            virtual ~iGameObject() {}
            virtual void SetPosition(const Vector3& pos) = 0;
            virtual const Vector3& GetPosition() const = 0;

            template<typename T>
            T* AddComponent() {
                T* component = xnew T();
                if (!AddComponent(component)) {
                    xdel component;
                    return nullptr;
                }
                return component;
            }

            template<typename T>
            T* GetComponent() const {
                return dynamic_cast<T*>(GetComponent(typeid(T).name()));
            }

            template<typename T>
            void RemoveComponent(T* component) {
                RemoveIComponent(typeid(T).name(), component);
            }

            template<typename T>
            void GetComponents(std::vector<T*>& components) const {
                const std::set<iComponent*>* temp = GetComponents(typeid(T).name());
                for (auto i = temp->cbegin(); i != temp->cend(); i++) {
                    components.push_back(dynamic_cast<T*>(*i));
                }
            }

        protected:
            iGameObject() {}

            virtual bool AddComponent(iComponent* component) = 0;
            virtual void RemoveIComponent(const char* type, iComponent* component) = 0;
            virtual iComponent* GetComponent(const std::string& type) const = 0;
            virtual const std::set<iComponent*>* GetComponents(const std::string& type) const = 0;
        };

        class iGameObjectApi {
        public:
            virtual ~iGameObjectApi() {}
            virtual iGameObject* CreateGameObject(const char* file, const int line) = 0;
            virtual void DestroyGameObject(iGameObject* gameObject) = 0;
        };
#       define CREATE_GAMEOBJECT(gameObjectApi) gameObjectApi->CreateGameObject(__FILE__, __LINE__)
    }
}

#endif //__iGameObjectApi_h__
