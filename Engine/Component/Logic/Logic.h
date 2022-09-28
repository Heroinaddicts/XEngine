#ifndef __Logic_h__
#define __Logic_h__

#include "Header.h"

namespace XEngine {
    class Logic : public iLogic {
    public:
        virtual ~Logic() {}

        // Í¨¹ý iLogic ¼Ì³Ð
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;
        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

    private:
        std::string _ComponentPath;
        std::map<std::string, Api::iComponent*> _ComponentMap;
    };
}

#endif //__Logic_h__
