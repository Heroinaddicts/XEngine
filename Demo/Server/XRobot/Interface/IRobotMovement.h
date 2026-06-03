#ifndef __IRobotMovement_h__
#define __IRobotMovement_h__

#include "iModule.h"
UsingXEngine;

#include "Vector2.h"

class IRobotSession;
class IRobotMovement : public Api::iModule {
public:
    virtual ~IRobotMovement() {}

    typedef std::function<void(const UInt64 characterId, const Vector2& pos, const bool isArrive)> fArrive;
    virtual void MoveTo(const UInt64 characterId, const Vector2& pos, const fArrive& arrive) = 0;
};

#endif //__IRobotMovement_h__
