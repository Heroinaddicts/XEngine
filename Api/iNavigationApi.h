#ifndef __iNavigationApi_h__
#define __iNavigationApi_h__

#include "MultiSys.h"
#include "Vector.h"

#include <string>
#include <vector>

namespace XEngine {
    namespace Api {
        class iNavmesh {
        public:
            virtual ~iNavmesh() {}

            virtual bool CalcPath(const Vector3& start_pos, const Vector3& end_pos, OUT std::vector<Vector3>& corners) const = 0;
            virtual bool GetHeight(const Vector3& pos, OUT float& y) const = 0;
            virtual bool RaycastOnMesh(const Vector3& start_pos, const Vector3& end_pos, OUT Vector3& hitpos) const = 0;
            virtual bool RandomPositionInRange(const Vector3& pos, const float radius, OUT Vector3& ret) const = 0;

            virtual bool CreateStaticObstacle(const Vector3& pos, const float radius, const float height) = 0;
        };

        class iNavigationApi {
        public:
            virtual ~iNavigationApi() {}

            virtual iNavmesh* LoadNavmeshSync(const std::string& file) = 0;
            virtual void LoadNavmeshAsync(const std::string& file, void(* const func)(iNavmesh* mesh)) = 0;

            virtual void ReleaseNavmesh(iNavmesh* mesh) = 0;
        };
    }
}

#endif //__iNavigationApi_h__

