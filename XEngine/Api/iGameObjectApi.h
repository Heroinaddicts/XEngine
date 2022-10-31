#ifndef __iGameObjectApi_h__
#define __iGameObjectApi_h__

#include "MultiSys.h"
#include <string>

namespace XEngine {
    namespace Api {
        class iObject {
        public:
            virtual ~iObject() {}
            iObject() : _File(nullptr), _Line(0) {}

            virtual void OnCreate() = 0;
            virtual void OnStart() = 0;
            virtual void OnUpdate() = 0;
            virtual void OnFixedUpdate() = 0;
            virtual void OnDestroy() = 0;

            const char* const _File;
            const int _Line;
        };

        class iGameObject;
        class iComponent : public iObject {
        public:
            virtual ~iComponent() {}
            iComponent() : _GameObject(nullptr) {}
            iGameObject* const _GameObject;
        };

        class iGameObject : public iObject {
        public:
            virtual ~iGameObject() {}
            template<typename T>
            T* AddComponent(const std::string& type_name, const char* file, const int line) {
                T* component = xnew T();
                if (!AddComponent(type_name, component, file, line)) {
                    xdel component;
                    return nullptr;
                }
                return component;
            }

            template<typename T>
            T* GetComponent(const std::string& type_name) {
                return static_cast<T*>(GetComponent(type_name));
            }

        protected:
            virtual bool AddComponent(const std::string& type_name, iComponent* component, const char* file, const int line) = 0;
            virtual iComponent* GetComponent(const std::string& type_name) const = 0;
        };
#       define ADD_COMPONENT(gameObject, ComponentType) gameObject->AddComponent<ComponentType>(#ComponentType, __FILE__, __LINE__)
#       define GET_COMPONENT(gameObject, ComponentType) gameObject->AddComponent<ComponentType>(#ComponentType)

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
