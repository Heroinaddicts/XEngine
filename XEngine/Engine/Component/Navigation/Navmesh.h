#ifndef __Navmesh_h__
#define __Navmesh_h__

#include "Header.h"

namespace XEngine {

    class Navmesh : public Api::iNavmesh {
    public:
        virtual ~Navmesh();
        Navmesh(const std::string& file);

        bool Load();

        virtual bool CalcPath(const Vector3& start_pos, const Vector3& end_pos, OUT std::vector<Vector3>& corners) const override;
        virtual bool GetHeight(const Vector3& pos, OUT float& y) const override;
        virtual bool RaycastOnMesh(const Vector3& start_pos, const Vector3& end_pos, OUT Vector3& hitpos) const override;
        virtual bool RandomPositionInRange(const Vector3& pos, const float radius, OUT Vector3& ret) const override;

        virtual bool CreateStaticObstacle(const Vector3& pos, const float radius, const float height) override;


    public:
        const std::string _File;

        dtNavMesh* _Mesh;
        dtNavMeshQuery* _Query;
        dtCrowd* _DtCrowd;
        dtTileCache* _DtTileCache;

        LinearAllocator* _Talloc;
        FastLZCompressor* _Tcomp;
        MeshProcess* _Tmproc;

        dtQueryFilter _Filter;
    };
}

#endif //__Navmesh_h__
