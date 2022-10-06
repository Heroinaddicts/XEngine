#ifndef __iMeshLoaderApi_h__
#define __iMeshLoaderApi_h__

#include "MultiSys.h"
#include "Vector3.h"
#include <vector>
#include <list>
#include <string>

namespace XEngine {
    namespace Api {
        class Face {
        public:
            Face() : u(0), v(0), w(0) {}

            unsigned int u;
            unsigned int v;
            unsigned int w;
        };

        class MeshData {
        public:
            std::vector<Vector3> v;
            std::list<std::vector<Face>> f;
            std::string name;
        };

        class iMeshLoaderApi {
        public:
            virtual ~iMeshLoaderApi() {}
            virtual bool LoadObjSync(const std::string& path, OUT MeshData& data) = 0;
        };
    }
}

#endif //__iMeshLoaderApi_h__
