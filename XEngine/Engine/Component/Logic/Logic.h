#ifndef __Logic_h__
#define __Logic_h__

#include "Header.h"

namespace XEngine {
    class Logic : public iLogic {
    public:
        virtual ~Logic() {}

        static iLogic* GetInstance();

        virtual Api::iModule* FindModule(const std::string& name) override;
        // 通过 iLogic 继承
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;
        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

    private:
        Logic() {}

    private:
        std::string _ModulePath;
        std::map<std::string, Api::iModule*> _ModuleMap;
    };
}

#endif //__Logic_h__
