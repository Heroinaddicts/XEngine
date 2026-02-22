#ifndef __Backpack_h__
#define __Backpack_h__

#include "Header.h"

class Backpack : public IBackpack {
public:
    virtual ~Backpack() {}
private:
    // Í¨¹ý IBackpack ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    static void OnCharacterCreated(ICharacterDocker* const docker, IGameObject* const gameObject);
    static void OnCharacterDataCreated(ICharacterDocker* const docker, IGameObject* const gameObject);
    static void OnCharacterLoaded(ICharacterDocker* const docker, IGameObject* const gameObject);
    static void OnCharacterRelease(ICharacterDocker* const docker, IGameObject* const gameObject);

    void SaveBackpackData(IGameObject* const gameObject);

    bool Equip(const UInt64 characterGuid, const UInt64 equipGuid) override;
    bool UnEquip(const UInt64 characterGuid, const UInt64 equipGuid) override;
    bool QueryResourceByID(const UInt64 characterGuid, const UInt64 resourceId) override;
    void GetRewardItems(const UInt64 characterGuid, const std::vector<Int64>& itemResourceIds) override;
};

#endif //__Backpack_h__
