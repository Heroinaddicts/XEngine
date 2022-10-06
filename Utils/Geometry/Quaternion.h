#ifndef __Quaternion_h__
#define __Quaternion_h__

namespace XEngine {
#define kEpsilon (1E-06F)
    class Quaternion {
    public:
        virtual ~Quaternion() {}
        Quaternion() :x(0), y(0), z(0), w(0) {}
        Quaternion(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
        //
        // ժҪ:
        //     X component of the Quaternion. Don't modify this directly unless you know quaternions
        //     inside out.
        float x;
        //
        // ժҪ:
        //     Y component of the Quaternion. Don't modify this directly unless you know quaternions
        //     inside out.
        float y;
        //
        // ժҪ:
        //     Z component of the Quaternion. Don't modify this directly unless you know quaternions
        //     inside out.
        float z;
        //
        // ժҪ:
        //     W component of the Quaternion. Do not directly modify quaternions.
        float w;
    };
}

#endif //__Quaternion_h__
