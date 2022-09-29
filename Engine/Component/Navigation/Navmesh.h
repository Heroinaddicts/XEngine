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
        const std::string _file;

        dtNavMesh* _mesh;
        dtNavMeshQuery* _query;
        dtCrowd* _crowd;
        dtTileCache* _tile_cache;

        LinearAllocator* _talloc;
        FastLZCompressor* _tcomp;
        MeshProcess* _tmproc;

        dtQueryFilter _filter;
    };
}

#endif //__Navmesh_h__
