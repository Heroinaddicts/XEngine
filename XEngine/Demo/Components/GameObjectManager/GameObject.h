#ifndef __GameObject_h__
#define __GameObject_h__

#include "Header.h"
#include "Attribute.h"

class GameObject : public iGameObject {
public:
    virtual ~GameObject() {}
    GameObject() {}


    std::vector<Attribute> _Attributes;
};

#endif //__GameObject_h__
