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

        __forceinline const std::vector<Vector3>& GetV() const { return _Vs; }
        __forceinline const std::vector<Vector3>& GetVN() const { return _Vns; }
        __forceinline const std::vector<Vector2>& GetVT() const { return _Vts; }
        __forceinline const std::vector<std::vector<Face>>& GetF() const { return _Fs; }

    private:
        std::vector<Vector3> _Vs;
        std::vector<Vector3> _Vns;
        std::vector<Vector2> _Vts;
        std::vector<std::vector<Face>> _Fs;
    };
}

#endif //__X3DObj_h__
