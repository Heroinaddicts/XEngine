#pragma once

#include "Engine.h"
#include "iNavigation.h"

#include "DetourCommon.h"
#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourCrowd.h"
#include "DetourNavMeshQuery.h"
#include "DetourTileCache.h"
#include "DetourTileCacheBuilder.h"
#include "DetourNavMeshBuilder.h"
#include "fastlz/fastlz.h"

#include "SafeSystem.h"
#include "SafeTools.h"

#include <map>

namespace XEngine {
    const int MAX_POLYS = 256;
    const int MAX_SMOOTH = 2048;

    enum SamplePolyAreas {
        SAMPLE_POLYAREA_GROUND,
        SAMPLE_POLYAREA_WATER,
        SAMPLE_POLYAREA_ROAD,
        SAMPLE_POLYAREA_DOOR,
        SAMPLE_POLYAREA_GRASS,
        SAMPLE_POLYAREA_JUMP,
    };

    enum SamplePolyFlags
    {
        SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
        SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
        SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
        SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
        SAMPLE_POLYFLAGS_DISABLED = 0x10,		// Disabled polygon
        SAMPLE_POLYFLAGS_ALL = 0xffff	// All abilities.
    };

    static const int TILECACHESET_MAGIC = 'T' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
    static const int TILECACHESET_VERSION = 1;

    inline float Frand() {
        return SafeTools::Rand(1000) / (float)(1000);
    }

    inline bool InRange(const float* v1, const float* v2, const float r, const float h) {
        const float dx = v2[0] - v1[0];
        const float dy = v2[1] - v1[1];
        const float dz = v2[2] - v1[2];
        return (dx * dx + dz * dz) < r * r && fabsf(dy) < h;
    }

    inline int FixupCorridor(dtPolyRef* path, const int npath, const int maxPath, const dtPolyRef* visited, const int nvisited) {
        int furthestPath = -1;
        int furthestVisited = -1;

        // Find furthest common polygon.
        for (int i = npath - 1; i >= 0; --i) {
            bool found = false;
            for (int j = nvisited - 1; j >= 0; --j) {
                if (path[i] == visited[j]) {
                    furthestPath = i;
                    furthestVisited = j;
                    found = true;
                }
            }
            if (found)
                break;
        }

        // If no intersection found just return current path. 
        if (furthestPath == -1 || furthestVisited == -1)
            return npath;

        // Concatenate paths.	

        // Adjust beginning of the buffer to include the visited.
        const int req = nvisited - furthestVisited;
        const int orig = rcMin(furthestPath + 1, npath);
        int size = rcMax(0, npath - orig);
        if (req + size > maxPath)
            size = maxPath - req;
        if (size)
            memmove(path + req, path + orig, size * sizeof(dtPolyRef));

        // Store visited
        for (int i = 0; i < req; ++i)
            path[i] = visited[(nvisited - 1) - i];

        return req + size;
    }

    // This function checks if the path has a small U-turn, that is,
    // a polygon further in the path is adjacent to the first polygon
    // in the path. If that happens, a shortcut is taken.
    // This can happen if the target (T) location is at tile boundary,
    // and we're (S) approaching it parallel to the tile edge.
    // The choice at the vertex can be arbitrary, 
    //  +---+---+
    //  |:::|:::|
    //  +-S-+-T-+
    //  |:::|   | <-- the step can end up in here, resulting U-turn path.
    //  +---+---+
    inline int FixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery) {
        if (npath < 3)
            return npath;

        // Get connected polygons
        static const int maxNeis = 16;
        dtPolyRef neis[maxNeis];
        int nneis = 0;

        const dtMeshTile* tile = 0;
        const dtPoly* poly = 0;
        if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
            return npath;

        for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next) {
            const dtLink* link = &tile->links[k];
            if (link->ref != 0) {
                if (nneis < maxNeis)
                    neis[nneis++] = link->ref;
            }
        }

        // If any of the neighbour polygons is within the next few polygons
        // in the path, short cut to that polygon directly.
        static const int maxLookAhead = 6;
        int cut = 0;
        for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
            for (int j = 0; j < nneis; j++) {
                if (path[i] == neis[j]) {
                    cut = i;
                    break;
                }
            }
        }

        if (cut > 1) {
            int offset = cut - 1;
            npath -= offset;
            for (int i = 1; i < npath; i++)
                path[i] = path[i + offset];
        }

        return npath;
    }

    inline bool GetSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
        const float minTargetDist,
        const dtPolyRef* path, const int pathSize,
        float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
        float* outPoints = 0, int* outPointCount = 0) {

        // Find steer target.
        static const int MAX_STEER_POINTS = 3;
        float steerPath[MAX_STEER_POINTS * 3];
        unsigned char steerPathFlags[MAX_STEER_POINTS];
        dtPolyRef steerPathPolys[MAX_STEER_POINTS];
        int nsteerPath = 0;
        navQuery->findStraightPath(startPos, endPos, path, pathSize,
            steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
        if (!nsteerPath)
            return false;

        if (outPoints && outPointCount)
        {
            *outPointCount = nsteerPath;
            for (int i = 0; i < nsteerPath; ++i)
                dtVcopy(&outPoints[i * 3], &steerPath[i * 3]);
        }


        // Find vertex far enough to steer to.
        int ns = 0;
        while (ns < nsteerPath)
        {
            // Stop at Off-Mesh link or when point is further than slop away.
            if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
                !InRange(&steerPath[ns * 3], startPos, minTargetDist, 1000.0f))
                break;
            ns++;
        }
        // Failed to find good point to steer to.
        if (ns >= nsteerPath)
            return false;

        dtVcopy(steerPos, &steerPath[ns * 3]);
        steerPos[1] = startPos[1];
        steerPosFlag = steerPathFlags[ns];
        steerPosRef = steerPathPolys[ns];

        return true;
    }

    struct TileCacheSetHeader {
        int _Magic;
        int _Version;
        int _NumTiles;
        dtNavMeshParams _MeshParams;
        dtTileCacheParams _CacheParams;
    };

    struct TileCacheTileHeader {
        dtCompressedTileRef _TileRef;
        int _DataSize;
    };

    struct NavMeshSetHeader {
        int _Magic;
        int _Version;
        int _NumTiles;
        dtNavMeshParams _Params;
    };

    struct NavMeshTileHeader {
        dtTileRef _TileRef;
        int _DataSize;
    };

    struct LinearAllocator : public dtTileCacheAlloc {
        unsigned char* _Buffer;
        size_t _Capacity;
        size_t _Top;
        size_t _High;

        LinearAllocator(const size_t cap) : _Buffer(0), _Capacity(0), _Top(0), _High(0) {
            resize(cap);
        }

        ~LinearAllocator() {
            dtFree(_Buffer);
        }

        void resize(const size_t cap) {
            if (_Buffer) dtFree(_Buffer);
            _Buffer = (unsigned char*)dtAlloc(cap, DT_ALLOC_PERM);
            _Capacity = cap;
        }

        virtual void reset() {
            _High = dtMax(_High, _Top);
            _Top = 0;
        }

        virtual void* alloc(const size_t size) {
            if (!_Buffer)
                return 0;
            if (_Top + size > _Capacity)
                return 0;
            unsigned char* mem = &_Buffer[_Top];
            _Top += size;
            return mem;
        }

        virtual void free(void* /*ptr*/) {
            // Empty
        }
    };

    struct MeshProcess : public dtTileCacheMeshProcess {
        inline MeshProcess() {}

        virtual void process(dtNavMeshCreateParams* params,
            unsigned char* polyAreas, unsigned short* polyFlags) {
            // Update poly flags from areas.
            for (int i = 0; i < params->polyCount; ++i) {
                if (polyAreas[i] == DT_TILECACHE_WALKABLE_AREA)
                    polyAreas[i] = SAMPLE_POLYAREA_GROUND;
                if (polyAreas[i] == SAMPLE_POLYAREA_GROUND ||
                    polyAreas[i] == SAMPLE_POLYAREA_GRASS ||
                    polyAreas[i] == SAMPLE_POLYAREA_ROAD) {
                    polyFlags[i] = SAMPLE_POLYFLAGS_WALK;
                }
                else if (polyAreas[i] == SAMPLE_POLYAREA_WATER) {
                    polyFlags[i] = SAMPLE_POLYFLAGS_SWIM;
                }
                else if (polyAreas[i] == SAMPLE_POLYAREA_DOOR) {
                    polyFlags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
                }
            }
        }
    };

    struct FastLZCompressor : public dtTileCacheCompressor {
        virtual ~FastLZCompressor() {}

        virtual int maxCompressedSize(const int bufferSize) {
            return (int)(bufferSize * 1.05f);
        }

        virtual dtStatus compress(const unsigned char* buffer, const int bufferSize,
            unsigned char* compressed, const int /*maxCompressedSize*/, int* compressedSize) override {
            *compressedSize = fastlz_compress((const void* const)buffer, bufferSize, compressed);
            return DT_SUCCESS;
        }

        virtual dtStatus decompress(const unsigned char* compressed, const int compressedSize,
            unsigned char* buffer, const int maxBufferSize, int* bufferSize) override {
            *bufferSize = fastlz_decompress(compressed, compressedSize, buffer, maxBufferSize);
            return *bufferSize < 0 ? DT_FAILURE : DT_SUCCESS;
        }
    };
}
