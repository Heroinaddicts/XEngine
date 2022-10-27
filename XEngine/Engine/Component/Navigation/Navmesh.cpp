#include "Navmesh.h"

namespace XEngine {
    Navmesh::~Navmesh() {
        dtFreeNavMeshQuery(_Query);
        dtFreeNavMesh(_Mesh);
        dtFreeCrowd(_DtCrowd);
    }

    Navmesh::Navmesh(const std::string& file) : _File(file), _Mesh(nullptr), _Query(nullptr), _DtCrowd(nullptr), _DtTileCache(nullptr), _Talloc(nullptr), _Tcomp(nullptr), _Tmproc(nullptr) {
        _Query = dtAllocNavMeshQuery();
        _DtCrowd = dtAllocCrowd();

        _Filter.setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);
        _Filter.setAreaCost(SAMPLE_POLYAREA_WATER, 10.0f);
        _Filter.setAreaCost(SAMPLE_POLYAREA_ROAD, 1.0f);
        _Filter.setAreaCost(SAMPLE_POLYAREA_DOOR, 1.0f);
        _Filter.setAreaCost(SAMPLE_POLYAREA_GRASS, 2.0f);
        _Filter.setAreaCost(SAMPLE_POLYAREA_JUMP, 1.5f);

        _Talloc = new LinearAllocator(32000);
        _Tcomp = new FastLZCompressor();
        _Tmproc = new MeshProcess();
    }

    bool Navmesh::Load() {
        int64 tick = SafeSystem::Time::GetMilliSecond();
        FILE* fp = fopen(_File.c_str(), "rb");
        if (!fp) return false;

        // Read header.
        TileCacheSetHeader header;
        size_t headerReadReturnCode = fread(&header, sizeof(TileCacheSetHeader), 1, fp);
        if (headerReadReturnCode != 1
            || header.magic != TILECACHESET_MAGIC
            || header.version != TILECACHESET_VERSION) {
            // Error or early EOF
            fclose(fp);
            return false;
        }

        _Mesh = dtAllocNavMesh();
        if (!_Mesh) {
            fclose(fp);
            return false;
        }

        dtStatus status = _Mesh->init(&header.meshParams);
        if (dtStatusFailed(status)) {
            fclose(fp);
            return false;
        }

        _DtTileCache = dtAllocTileCache();
        if (!_DtTileCache) {
            fclose(fp);
            return false;
        }

        status = _DtTileCache->init(&header.cacheParams, _Talloc, _Tcomp, _Tmproc);
        if (dtStatusFailed(status)) {
            fclose(fp);
            return false;
        }

        // Read tiles.
        for (int i = 0; i < header.numTiles; ++i) {
            TileCacheTileHeader tileHeader;
            size_t tileHeaderReadReturnCode = fread(&tileHeader, sizeof(tileHeader), 1, fp);
            if (tileHeaderReadReturnCode != 1) {
                // Error or early EOF
                fclose(fp);
                return false;
            }

            if (!tileHeader.tileRef || !tileHeader.dataSize)
                break;

            unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
            if (!data) break;
            memset(data, 0, tileHeader.dataSize);
            size_t tileDataReadReturnCode = fread(data, tileHeader.dataSize, 1, fp);
            if (tileDataReadReturnCode != 1) {
                // Error or early EOF
                dtFree(data);
                fclose(fp);
                return false;
            }

            dtCompressedTileRef tile = 0;
            dtStatus addTileStatus = _DtTileCache->addTile(data, tileHeader.dataSize, DT_COMPRESSEDTILE_FREE_DATA, &tile);
            if (dtStatusFailed(addTileStatus)) {
                dtFree(data);
            }

            if (tile)
                _DtTileCache->buildNavMeshTile(tile, _Mesh);
        }

        fclose(fp);
        _Query->init(_Mesh, 2048);
        XLOG(g_Engine, "load navmesh %s use %lld", _File.c_str(), SafeSystem::Time::GetMilliSecond() - tick);
        return true;
    }

    bool Navmesh::CalcPath(const Vector3& start_pos, const Vector3& end_pos, OUT std::vector<Vector3>& corners) const {
        XASSERT(_Mesh, "wtf");
        if (!_Mesh)
            return false;

        float sp[3] = { start_pos._X, start_pos._Y, start_pos._Z };
        float ep[3] = { end_pos._X, end_pos._Y, end_pos._Z };

        float polyPickExt[3] = { 2,4,2 };

        dtPolyRef startRef;
        dtPolyRef endRef;
        _Query->findNearestPoly(sp, polyPickExt, &_Filter, &startRef, 0);
        _Query->findNearestPoly(ep, polyPickExt, &_Filter, &endRef, 0);

        dtStatus pathFindStatus = DT_FAILURE;

        static dtPolyRef polys[MAX_POLYS];

        int polyCount = 0;
        _Query->findPath(startRef, endRef, sp, ep, &_Filter, polys, &polyCount, MAX_POLYS);
        if (polyCount) {
            // In case of partial path, make sure the end point is clamped to the last polygon.
            float epos[3];
            dtVcopy(epos, ep);
            if (polys[polyCount - 1] != endRef)
                _Query->closestPointOnPoly(polys[polyCount - 1], ep, epos, 0);

            static float path[MAX_POLYS * 3];
            static unsigned char pathFlags[MAX_POLYS];
            static dtPolyRef pathPolys[MAX_POLYS];
            int count = 0;
            int options = DT_STRAIGHTPATH_AREA_CROSSINGS;

            _Query->findStraightPath(sp, epos, polys, polyCount,
                path, pathFlags,
                pathPolys, &count, MAX_POLYS, options);

            for (int i = 0; i < count; i++) {
                corners.push_back(Vector3(path[3 * i], path[3 * i + 1], path[3 * i + 2]));
            }
        }

        return true;
    }

    bool Navmesh::GetHeight(const Vector3& pos, OUT float& y) const {
        float p[3] = { pos._X, pos._Y, pos._Z };
        float polyPickExt[3] = { 2,4,2 };
        dtPolyRef ref;
        bool isOverPoly;
        _Query->findNearestPoly(p, polyPickExt, &_Filter, &ref, 0, &isOverPoly);
        if (0 == ref) {
            return false;
        }

        return _Query->getPolyHeight(ref, p, &y) == DT_SUCCESS;
    }

    bool Navmesh::RaycastOnMesh(const Vector3& start_pos, const Vector3& end_pos, OUT Vector3& hitpos) const {
        float t = 0;
        float polyPickExt[3] = { 2,4,2 };

        dtPolyRef startRef;
        dtPolyRef endRef;

        float sp[3] = { start_pos._X, start_pos._Y, start_pos._Z };
        _Query->findNearestPoly(sp, polyPickExt, &_Filter, &startRef, 0);

        float ep[3] = { end_pos._X, end_pos._Y, end_pos._Z };
        _Query->findNearestPoly(ep, polyPickExt, &_Filter, &endRef, 0);


        static dtPolyRef polys[MAX_POLYS];
        static float straightPath[MAX_POLYS * 3];
        straightPath[0] = sp[0];
        straightPath[1] = sp[1];
        straightPath[2] = sp[2];

        float hitNormal[3];
        int polyCount = 0;
        _Query->raycast(startRef, sp, ep, &_Filter, &t, hitNormal, polys, &polyCount, MAX_POLYS);

        bool ret = false;
        if (t > 1) {
            dtVcopy(hitNormal, ep);
            ret = false;
        }
        else {
            // Hit
            dtVlerp(hitNormal, sp, ep, t);
            ret = true;
        }
        // Adjust height.
        if (polyCount > 0) {
            _Query->getPolyHeight(polys[polyCount - 1], hitNormal, &hitNormal[1]);
        }
        hitpos = hitNormal;
        return ret;
    }

    bool Navmesh::RandomPositionInRange(const Vector3& pos, const float radius, OUT Vector3& ret) const {
        float start_point[] = { pos._X, pos._Y,  pos._Z };
        float point[3];

        dtPolyRef startRef;
        float polyPickExt[3] = { 2,4,2 };
        _Query->findNearestPoly(start_point, polyPickExt, &_Filter, &startRef, 0);
        dtPolyRef ref;
        dtStatus status = _Query->findRandomPointAroundCircle(startRef, start_point, radius, &_Filter, Frand, &ref, point);
        if (!dtStatusSucceed(status)) {
            return false;
        }

        ret = point;
        return true;
    }

    bool Navmesh::CreateStaticObstacle(const Vector3& pos, const float radius, const float height) {
        float p[] = { pos._X, pos._Y, pos._Z };
        _DtTileCache->addObstacle(p, radius, height, nullptr);
        _DtTileCache->update(0.05, _Mesh);
        return true;
    }
}
