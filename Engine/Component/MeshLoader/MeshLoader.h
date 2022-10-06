#ifndef __MeshLoader_h__
#define __MeshLoader_h__

#include "Header.h"

namespace XEngine {
    class MeshLoader : public iMeshLoader {
    public:
        virtual ~MeshLoader() {}

        static iMeshLoader* GetInstance();

        // 通过 iMeshLoader 继承
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;

        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void FixedUpdate(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

        virtual bool LoadObjSync(const std::string& path, OUT Api::MeshData& data) override;
    private:

    };
}

#endif //__MeshLoader_h__
