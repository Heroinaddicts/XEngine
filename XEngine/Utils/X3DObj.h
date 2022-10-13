#ifndef __X3DObj_h__
#define __X3DObj_h__

#include "Vector.h"
#include "Geometry.h"
#include <string>
#include <vector>

namespace XEngine {
    class X3DObj {
    public:
        virtual ~X3DObj() {}
        X3DObj() {}

        bool Load(const std::string& path);

        __forceinline const std::vector<Vector3>& GetV() const { return _vs; }
        __forceinline const std::vector<Vector3>& GetVN() const { return _vns; }
        __forceinline const std::vector<Vector2>& GetVT() const { return _vts; }
        __forceinline const std::vector<std::vector<Facet>>& GetF() const { return _fs; }

    private:
        std::vector<Vector3> _vs;
        std::vector<Vector3> _vns;
        std::vector<Vector2> _vts;
        std::vector<std::vector<Facet>> _fs;
    };
}

#endif //__X3DObj_h__
