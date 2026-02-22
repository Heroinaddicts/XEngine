#ifndef __IMovement_h__
#define __IMovement_h__

#include "iModule.h"
UsingXEngine;
#include <Vector2.h>
#include <functional>

class IGameObject;
class IMovement : public Api::iModule {
public:
    virtual ~IMovement() {}
    typedef std::function<void(IGameObject* const, const bool arrived)> fArrived;
    virtual void MoveTo(IGameObject* const entity, const Vector2& position, const fArrived& callback = nullptr) = 0;
    virtual void StopMoveTo(IGameObject* const entity) = 0;
};

#endif //__IMovement_h__
