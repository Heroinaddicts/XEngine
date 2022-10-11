#ifndef __X3DObjFile_h__
#define __X3DObjFile_h__

#include "Vector.h"

namespace XEngine {
    class X3DObjFile {
    public:
        virtual ~X3DObjFile() {}
        X3DObjFile() : _v_count(0), _v(nullptr), _vn_count(0), _vn(nullptr), _vt_count(0), _vt(nullptr), _f_count(0), _f(nullptr) {}

    private:
        int _v_count;
        Vector3* _v;

        int _vn_count;
        Vector3* _vn;

        int _vt_count;
        Vector2* _vt;

        int _f_count;
        Vector3Int* _f;
    };
}

#endif //__X3DObjFile_h__
