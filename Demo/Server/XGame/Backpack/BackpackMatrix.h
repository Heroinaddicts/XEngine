#ifndef __BackpackMatrix_h__
#define __BackpackMatrix_h__

#include "Header.h"

#define MAX_TABS 4
#define MAX_COUNT AutoConfig::Const::PackRules::MaxCount

class BackpackMatrix2D {
public:
    virtual ~BackpackMatrix2D() {

        for (auto& [type, matrix] : _Matrix) {
            for (int i = 0; i < _Width * _Height; i++)
            {
                g_GameWorld->ReleaseGameObjectByGuid(matrix[i]);
            }
            matrix.clear();
        }

        //
        IGameObject* character = g_GameWorld->QueryGameObject(_Guid);
        XASSERT(character != nullptr, "wtf");

        g_GameWorld->ReleaseGameObjectByGuid(character->GetUInt64(Character::SlotBeltUInt64));
        g_GameWorld->ReleaseGameObjectByGuid(character->GetUInt64(Character::SlotClothsUInt64));
        g_GameWorld->ReleaseGameObjectByGuid(character->GetUInt64(Character::SlotJewelryUInt64));
        g_GameWorld->ReleaseGameObjectByGuid(character->GetUInt64(Character::SlotPantsUInt64));
        g_GameWorld->ReleaseGameObjectByGuid(character->GetUInt64(Character::SlotShoesUInt64));
        g_GameWorld->ReleaseGameObjectByGuid(character->GetUInt64(Character::SlotWeaponUInt64));

        _Matrix.clear();
        _AddItemChange.Clear();
        _DeleteItemChange.Clear();
    }

    const UInt64 _Guid;

    static BackpackMatrix2D* CreateBackpackMatrix2D(const UInt64 guid, const int width, const int height, const float totalWeight = 0.0f) {
        if (s_BackpackMatrix2DMap.find(guid) != s_BackpackMatrix2DMap.end()) {
            ErrorLog(g_Engine, "Guid %llu already has a BackpackMatrix2D", guid);
            return nullptr;
        }

        BackpackMatrix2D* b = XPOOL_CREATE(s_Pool, guid, width, height, totalWeight);
        s_BackpackMatrix2DMap.insert({ guid, b });
        return b;
    }

    static BackpackMatrix2D* QueryBackpackMatrix2D(const UInt64 guid) {
        auto itor = s_BackpackMatrix2DMap.find(guid);
        if (itor == s_BackpackMatrix2DMap.end()) {
            return nullptr;
        }

        return itor->second;
    }

    static void ReleaseBackpackMatrix2D(const UInt64 guid) {
        auto itor = s_BackpackMatrix2DMap.find(guid);
        if (itor == s_BackpackMatrix2DMap.end()) {
            ErrorLog(g_Engine, "Guid %llu has no BackpackMatrix2D", guid);
            return;
        }

        XPOOL_RELEASE(s_Pool, itor->second);
        s_BackpackMatrix2DMap.erase(itor);
    }

    BackpackMatrix2D(const UInt64 guid, const int width, const int height, const float totalWeight = 0.0f)
        : _Guid(guid), _Width(width), _Height(height), _TotalWeight(totalWeight) {

        AutoConfig::Enum::eResources type[MAX_TABS] = { AutoConfig::Enum::eResources::Consume, AutoConfig::Enum::eResources::Material, AutoConfig::Enum::eResources::Special, AutoConfig::Enum::eResources::Equipment };
        for (int tab = 0; tab < MAX_TABS; tab++) {
            std::vector<UInt64> matrix;
            matrix.resize(width * height);

            _Matrix.insert({ type[tab], matrix });
        }
    }

    bool Equip(const UInt64 equipGuid)
    {
        IGameObject* go = g_GameWorld->QueryGameObject(equipGuid);
        XASSERT(go != nullptr, "wtf");
        XASSERT(go->GetBool(Equip::EquipedBool) == false, "wtf");
        for (int i = 0; i < _Width * _Height; i++)
        {
            if (_Matrix[static_cast<AutoConfig::Enum::eResources>(go->GetInt32(Item::ItemTypeInt32))][i] == equipGuid)
            {
                go->SetBool(Equip::EquipedBool, true);
                //从背包里面拿出来
                Pop(equipGuid, false);
                return true;
            }
        }

        TraceLog(g_Engine, "Equip Not Fund %11u", equipGuid);
        return false;
    }

    bool UnEquip(const UInt64 equipGuid)
    {
        IGameObject* go = g_GameWorld->QueryGameObject(equipGuid);
        XASSERT(go != nullptr, "wtf");
        XASSERT(go->GetBool(Equip::EquipedBool) == true, "wtf");

        go->SetBool(Equip::EquipedBool, false);
        go->SetInt32(Equip::CountInt32, 1);
        //装入背包
        Push(go, false);
        return true;
    }

    bool QueryResourceByID(const UInt64 resourceId)
    {
        for (auto const& [type, matrix] : _Matrix)
        {
            for (int i = 0; i < _Width * _Height; i++)
            {
                if (_Matrix[type][i] != INVALID_EQUIP_ID)
                {
                    IGameObject* go = g_GameWorld->QueryGameObject(_Matrix[type][i]);
                    if (go->GetUInt64(Item::ResourceIdInt64) == resourceId)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool Push(IGameObject* const go, const bool isNew) {
        
        XASSERT(go != nullptr, "wtf");
        if (_Matrix.find(static_cast<AutoConfig::Enum::eResources>(go->GetInt32(Item::ItemTypeInt32))) == _Matrix.end())
        {
            return false;
        }

        //判断是否超重
        if (isNew && _TotalWeight + GetItemWeight(go->GetInt32(Item::ItemTypeInt32), go->ConfigID(), go->GetInt32(Item::CountInt32)) > g_GameWorld->QueryGameObject(_Guid)->GetFloat(Character::AffordFloat))
        {
            _AddItemChange.set_code(eError::OverWeight);
            return false;
        }   

        UInt64 remaining = go->GetInt32(Item::CountInt32);
        if (Stackable(go))
        {
            for (int i = 0; i < _Width * _Height; i++)
            {
                IGameObject* slot = g_GameWorld->QueryGameObject(_Matrix[static_cast<AutoConfig::Enum::eResources>(go->GetInt32(Item::ItemTypeInt32))][i]);
                if (slot != nullptr)
                {
                    if (CanMerge(slot, go))
                    {
                        UInt16 space = MAX_COUNT - slot->GetInt32(Item::CountInt32);
                        UInt16 mergeCount = (space < remaining) ? space : remaining;

                        slot->SetInt32(Item::CountInt32, slot->GetInt32(Item::CountInt32) + mergeCount);
                            
                        remaining -= mergeCount;

                        oGameUnit* unit = _AddItemChange.add_changeitem();
                        PackageGameObjectToPB(unit, slot, false);

                        _TotalWeight = isNew ? _TotalWeight + GetItemWeight(slot->GetInt32(Item::ItemTypeInt32), slot->ConfigID(), slot->GetInt32(Item::CountInt32)) : _TotalWeight;
                    }
                }
            }
        }

        if (remaining > 0)
        {
            go->SetInt32(Item::CountInt32, remaining);

            if (!FindAndPlaceItem(go))
            {
                oGameUnit* unit = _AddItemChange.add_remainingitem();
                PackageGameObjectToPB(unit, go, false);
                _AddItemChange.set_code(eError::BackpackFull);
                return false;
            }

            oGameUnit* unit = _AddItemChange.add_changeitem();
            PackageGameObjectToPB(unit, go, false);
            _TotalWeight = isNew ? _TotalWeight + GetItemWeight(go->GetInt32(Item::ItemTypeInt32), go->ConfigID(), go->GetInt32(Item::CountInt32)) : _TotalWeight;
        }
        return true;
    }

    bool Pop(const UInt64 itemGuid, const bool isDelete, const UInt16 count = 1) {
        IGameObject* go = g_GameWorld->QueryGameObject(itemGuid);
        XASSERT(go != nullptr, "wtf");
        for (auto& [type, matrix] : _Matrix)
        {
            for (int i = 0; i < _Width * _Height; i++)
            {
                if (matrix[i] == itemGuid)
                {
                    if (Stackable(go))
                    {
                        go->SetInt32(Item::CountInt32, go->GetInt32(Item::CountInt32) - count);
                        if (go->GetInt32(Item::CountInt32) == 0)
                        {
                            RemoveAndRerange(i, static_cast<AutoConfig::Enum::eResources>(go->GetInt32(Item::ItemTypeInt32)), go->ConfigID());
                        }
                        {
                            AutoStackRerange(i, static_cast<AutoConfig::Enum::eResources>(go->GetInt32(Item::ItemTypeInt32)), go->ConfigID(), count);
                        }
                    }
                    else
                    {
                        go->SetInt32(Item::CountInt32, 0);
                        RemoveAndRerange(i, static_cast<AutoConfig::Enum::eResources>(go->GetInt32(Item::ItemTypeInt32)), go->ConfigID());
                    }
                    oGameUnit* unit = _DeleteItemChange.add_changeitem();
                    PackageGameObjectToPB(unit, go, false);
                    if (isDelete)
                    {
                        _TotalWeight -= GetItemWeight(go->GetInt32(Item::ItemTypeInt32), go->ConfigID(), count);
                        if (go->GetInt32(Item::CountInt32) == 0)
                        {
                            g_GameWorld->ReleaseGameObjectByGuid(go->Guid());
                        }
                    }

                    return true;
                }
            }
        }

        return false;
    }

    void StartCalculateChange()
    {
        _AddItemChange.Clear();
        _DeleteItemChange.Clear();
    }

    void LoadBackpackData(oBackpackItemPush backpackData)
    {
        for (int i = 0; i < backpackData.backpackitem_size(); i++) {
            oGameUnit item = backpackData.backpackitem(i);
            IGameObject* gameObj = g_GameWorld->CreateGameObject(
                static_cast<eGameObjectType>(item.type()),
                item.configid(),
                item.guid(),
                [&](IGameObject* const go) {
                    for (int j = 0; j < item.attributes_size(); j++) {
                        const oAttribute& attr = item.attributes(j);
                        const oValue& value = attr.value();

                        switch (value._type()) {
                        case eAttributeType::__Int32__:
                            go->SetInt32(attr.index(), value._int());
                            break;
                        case eAttributeType::__UInt32__:
                            go->SetUInt32(attr.index(), value._uint());
                            break;
                        case eAttributeType::__Int64__:
                            go->SetInt64(attr.index(), value._int());
                            break;
                        case eAttributeType::__UInt64__:
                            go->SetUInt64(attr.index(), value._uint());
                            break;
                        case eAttributeType::__Float__:
                            go->SetFloat(attr.index(), value._float());
                            break;
                        case eAttributeType::__Bool__:
                            go->SetBool(attr.index(), value._bool());
                            break;
                        case eAttributeType::__String__:
                            go->SetString(attr.index(), value._string());
                            break;
                        case eAttributeType::__Vector2__:
                            if (value.has__vector2()) {
                                Vector2 vec2(value._vector2().x(), value._vector2().y());
                                go->SetVector2(attr.index(), vec2);
                            }
                            break;
                        case eAttributeType::__Vector3__:
                            if (value.has__vector3()) {
                                Vector3 vec3(value._vector3().x(), value._vector3().y(), value._vector3().z());
                                go->SetVector3(attr.index(), vec3);
                            }
                            break;
                        }
                    }
                },
                __FILE__, __LINE__
            );
            if (item.attributes(Equip::EquipedBool).value()._bool() == true)
            {
                continue;
            }
            _Matrix[static_cast<AutoConfig::Enum::eResources>(gameObj->GetInt32(Item::ItemTypeInt32))][i] = gameObj->Guid();
        }
    }

    oBackpackItemPush PackBackpackData()
    {
        oBackpackItemPush backpackData;
        backpackData.set_width(_Width);
        backpackData.set_height(_Height);
        backpackData.set_totalweight(_TotalWeight);

        for (auto const& [type, matrix] : _Matrix)
        {
            for (int i = 0; i < _Width * _Height; i++)
            {
                if (_Matrix[type][i] != INVALID_EQUIP_ID)
                {
                    IGameObject* go = g_GameWorld->QueryGameObject(_Matrix[type][i]);
                    oGameUnit* unit = backpackData.add_backpackitem();
                    PackageGameObjectToPB(unit, go, false);
                }
            }
        }

        IGameObject* character = g_GameWorld->QueryGameObject(_Guid);
        XASSERT(character != nullptr, "wtf");

        //遍历人身上的
        if (character->GetUInt64(Character::SlotBeltUInt64) != INVALID_EQUIP_ID)
        {
            IGameObject* go = g_GameWorld->QueryGameObject(character->GetUInt64(Character::SlotBeltUInt64));
            XASSERT(go != nullptr, "wtf");
            oGameUnit* unit = backpackData.add_backpackitem();
            PackageGameObjectToPB(unit, go, false);
        }

        if (character->GetUInt64(Character::SlotClothsUInt64) != INVALID_EQUIP_ID)
        {
            IGameObject* go = g_GameWorld->QueryGameObject(character->GetUInt64(Character::SlotClothsUInt64));
            XASSERT(go != nullptr, "wtf");
            oGameUnit* unit = backpackData.add_backpackitem();
            PackageGameObjectToPB(unit, go, false);
        }

        if (character->GetUInt64(Character::SlotJewelryUInt64) != INVALID_EQUIP_ID)
        {
            IGameObject* go = g_GameWorld->QueryGameObject(character->GetUInt64(Character::SlotJewelryUInt64));
            XASSERT(go != nullptr, "wtf");
            oGameUnit* unit = backpackData.add_backpackitem();
            PackageGameObjectToPB(unit, go, false);
        }

        if (character->GetUInt64(Character::SlotPantsUInt64) != INVALID_EQUIP_ID)
        {
            IGameObject* go = g_GameWorld->QueryGameObject(character->GetUInt64(Character::SlotPantsUInt64));
            XASSERT(go != nullptr, "wtf");
            oGameUnit* unit = backpackData.add_backpackitem();
            PackageGameObjectToPB(unit, go, false);
        }

        if (character->GetUInt64(Character::SlotShoesUInt64) != INVALID_EQUIP_ID)
        {
            IGameObject* go = g_GameWorld->QueryGameObject(character->GetUInt64(Character::SlotShoesUInt64));
            XASSERT(go != nullptr, "wtf");
            oGameUnit* unit = backpackData.add_backpackitem();
            PackageGameObjectToPB(unit, go, false);
        }

        if (character->GetUInt64(Character::SlotWeaponUInt64) != INVALID_EQUIP_ID)
        {
            IGameObject* go = g_GameWorld->QueryGameObject(character->GetUInt64(Character::SlotWeaponUInt64));
            XASSERT(go != nullptr, "wtf");
            oGameUnit* unit = backpackData.add_backpackitem();
            PackageGameObjectToPB(unit, go, false);
        }

        return backpackData;
    }

    oBackpackAddItemChange GetAddItemChangeData()
    {
        _AddItemChange.set_totalweight(_TotalWeight);
        return _AddItemChange;
    }

    oBackpackDeleteItemChange GetDeleteChangeData()
    {
        _DeleteItemChange.set_totalweight(_TotalWeight);
        return _DeleteItemChange;
    }

private:

    bool Stackable(const IGameObject* go)
    {
        return go->GetInt32(Item::ItemTypeInt32) != static_cast<int>(AutoConfig::Enum::eResources::Equipment);
    }

    bool CanMerge(const IGameObject* slot, const IGameObject* item)
    {
        return slot->ConfigID() == item->ConfigID() && slot->GetInt32(Item::CountInt32) < MAX_COUNT;
    }

    bool FindAndPlaceItem(IGameObject* go) {
        for (int i = 0; i < _Width * _Height; i++)
        {
            if (CanPlace(go->GetInt32(Item::ItemTypeInt32), i))
            {
                PlaceItem(go->GetInt32(Item::ItemTypeInt32), i, go);
                {
                    return true;
                }
            }
        }
        
        return false;
    }

    bool CanPlace(int itemType, int index) const {
        auto it = _Matrix.find(static_cast<AutoConfig::Enum::eResources>(itemType));
        if (it == _Matrix.end()) return false;

        return it->second[index] == INVALID_EQUIP_ID;
    }

    void PlaceItem(int itemType, int index, IGameObject* go) {
        _Matrix[static_cast<AutoConfig::Enum::eResources>(itemType)][index] = go->Guid();
    }

    void AutoStackRerange(int slotIndex, AutoConfig::Enum::eResources type, Int64 itemId, int count)
    {
        int needCount = count;
        auto& matrix = _Matrix[type];

        for (int index = _Width * _Height - 1; index > slotIndex; index--)
        {
            UInt64 currentGuid = matrix[index];
            if (currentGuid == INVALID_EQUIP_ID) {
                continue;
            }

            IGameObject* currentGo = g_GameWorld->QueryGameObject(currentGuid);
            if (!currentGo) {
                continue;
            }

            if (currentGo->ConfigID() == itemId)
            {
                if (needCount <= 0)
                {
                    break;
                }
                int currentCount = currentGo->GetInt32(Item::CountInt32);
                if (currentCount > needCount)
                {
                    currentGo->SetInt32(Item::CountInt32, currentCount - needCount);
                    needCount = 0;
                }
                else
                {
                    needCount -= currentCount;
                    currentGo->SetInt32(Item::CountInt32, 0);

                    RemoveAndRerange(index, type, itemId);
                    g_GameWorld->ReleaseGameObjectByGuid(currentGuid);
                }
                oGameUnit* unit = _DeleteItemChange.add_changeitem();
                PackageGameObjectToPB(unit, currentGo, false);
            }
        }

        if (needCount > 0)
        {
            UInt64 targetGuid = matrix[slotIndex];
            XASSERT(targetGuid != INVALID_EQUIP_ID, "wtf");
            IGameObject* targetGo = g_GameWorld->QueryGameObject(targetGuid);
            int newCount = targetGo->GetInt32(Item::CountInt32) + (count - needCount);
            targetGo->SetInt32(Item::CountInt32, newCount);
            oGameUnit* unit = _DeleteItemChange.add_changeitem();
            PackageGameObjectToPB(unit, targetGo, false);
        }
    }

    float GetItemWeight(const int itemType, const int itemId, const int count)
    {
        switch (itemType)
        {
            case static_cast<int>(AutoConfig::Enum::eResources::Consume):
            {
                return g_Config->GetAutoConfig()->QueryConsumeConfig(itemId)->ID * count;
            }
            case static_cast<int>(AutoConfig::Enum::eResources::Material):
            {
                return g_Config->GetAutoConfig()->QueryMaterialConfig(itemId)->ID * count;
            }
            case static_cast<int>(AutoConfig::Enum::eResources::Equipment):
            {
                return g_Config->GetAutoConfig()->QueryEquipmentConfig(itemId)->Weight * count;
            }
            case static_cast<int>(AutoConfig::Enum::eResources::Special):
            {
                return g_Config->GetAutoConfig()->QuerySpecialConfig(itemId)->ID * count;
            }
        }
    }

    void RemoveAndRerange(int removedIndex, AutoConfig::Enum::eResources type, Int64 itemId) {
        auto& matrix = _Matrix[type];
        matrix[removedIndex] = INVALID_EQUIP_ID;

        int emptySlot = removedIndex;

        for (int currentIndex = removedIndex + 1; currentIndex < _Width * _Height; currentIndex++) {
            if (matrix[currentIndex] != INVALID_EQUIP_ID) {
                matrix[emptySlot] = matrix[currentIndex];
                matrix[currentIndex] = INVALID_EQUIP_ID;
                emptySlot++;
            }
        }
    }

private:
    
    std::unordered_map<AutoConfig::Enum::eResources, std::vector<UInt64>> _Matrix;

    int _Width;
    int _Height;
    float _TotalWeight;
    oBackpackAddItemChange _AddItemChange;
    oBackpackDeleteItemChange _DeleteItemChange;

private:
    inline static std::unordered_map<UInt64, BackpackMatrix2D*> s_BackpackMatrix2DMap;
    inline static XPool<BackpackMatrix2D> s_Pool;
};

#endif //__BackpackMatrix_h__
