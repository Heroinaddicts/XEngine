#ifndef __Relation_h__
#define __Relation_h__

#include "Header.h"

class Relation : public IRelation {
public:
    virtual ~Relation() {}
private:
    // Í¨¹ý IRelation ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

};

#endif //__Relation_h__
