#ifndef __Test001_h__
#define __Test001_h__

#include "Header.h"

class ObjLoader : public iObjLoader {
public:
    virtual ~ObjLoader() {}
    virtual bool Initialize(iEngine* const engine);
    virtual bool Launch(iEngine* const engine);
    virtual bool Destroy(iEngine* const engine);

    virtual const X3DObj* Get3DObj(const std::string& path);
private:
    std::map<std::string, X3DObj*> _obj_map;
};

#endif //__Test001_h__
