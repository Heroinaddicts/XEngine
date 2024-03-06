#ifndef __XZip_h__
#define __XZip_h__

#include "Header.h"

namespace XEngine {
    class XZip : public iXZip, public SafeThread {
    public:
        virtual ~XZip() {}

        static XZip* GetInstance();

        bool Initialize(Api::iEngine* const engine) override;
        bool Launch(Api::iEngine* const engine) override;
        void Release(Api::iEngine* const engine) override;
        void EarlyUpdate(Api::iEngine* const engine) override;
        void Update(Api::iEngine* const engine) override;
        void LaterUpdate(Api::iEngine* const engine) override;
        void Unzip(const std::string& file, const std::string& folder, const fUnzipCallback fun) override;

        // Í¨¹ý SafeThread ¼Ì³Ð
        void Run(void* context) override;
    };
}

#endif //__XZip_h__
