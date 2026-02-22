#ifndef __SQLCacheTemplate_h__
#define __SQLCacheTemplate_h__

#include <string>
#include <vector>

namespace SQLCacheTemplate {
#define UsingSQLCacheTemplate using namespace SQLCacheTemplate;

    enum class eColumnType {
        __Int32__ = 0,
        __UInt32__ = 1,
        __Int64__ = 2,
        __UInt64__ = 3,
        __Float__ = 4,
        __String__ = 5,
        __Bool__ = 6,

        __Unknown__ = 7
    };

    enum class eTable {
t_account = 0,
t_backpack = 1,
t_character = 2,
t_equip = 3,
t_player = 4,
t_skill = 5,
t_task = 6,


        __Count__ = 7
    };

    constexpr const char* TableNames[] = { "t_account", "t_backpack", "t_character", "t_equip", "t_player", "t_skill", "t_task" };

    namespace t_account {
        enum eColumn {
        Platform__Int32__ = 0,
                    Guid__UInt64__PrimaryUnique = 1,
                    UnionID__String__ = 2,
                    LastLoginTick__Int64__ = 3,
                    RegistTick__Int64__ = 4,
            

            __Count__ = 5
        };
    }
    namespace t_backpack {
        enum eColumn {
        Account__UInt64__ = 0,
                    CharacterId__UInt64__PrimaryUnique = 1,
                    BackPackData__String__ = 2,
            

            __Count__ = 3
        };
    }
    namespace t_character {
        enum eColumn {
        Account__UInt64__ = 0,
                    Guid__UInt64__PrimaryUnique = 1,
                    ConfigID__Int64__ = 2,
                    Level__Int64__ = 3,
                    Exp__Int64__ = 4,
                    LastPublicSceneConfigID__Int64__ = 5,
                    SceneGUID__UInt64__ = 6,
                    X__Float__ = 7,
                    Y__Float__ = 8,
                    Angle__Float__ = 9,
                    SlotCloths__UInt64__ = 10,
                    ClothsConfigID__Int64__ = 11,
                    SlotPants__UInt64__ = 12,
                    PantsConfigID__Int64__ = 13,
                    SlotBelt__UInt64__ = 14,
                    BeltConfigID__Int64__ = 15,
                    SlotShoes__UInt64__ = 16,
                    ShoesConfigID__Int64__ = 17,
                    SlotJewelry__UInt64__ = 18,
                    JewelryConfigID__Int64__ = 19,
                    SlotWeapon__UInt64__ = 20,
                    WeaponConfigID__Int64__ = 21,
            

            __Count__ = 22
        };
    }
    namespace t_equip {
        enum eColumn {
        Guid__UInt64__PrimaryUnique = 0,
                    ConfigID__Int64__ = 1,
                    Equiped__Bool__ = 2,
                    EquipType__Int32__ = 3,
                    Patience__Int32__ = 4,
                    StrengthLv__Int32__ = 5,
                    DynamicAtk__Float__ = 6,
                    DynamicSpeed__Float__ = 7,
                    DynamicHp__Float__ = 8,
                    DynamicCritical__Float__ = 9,
                    DynamicCriticalDamage__Float__ = 10,
                    DynamicDefend__Float__ = 11,
                    DynamicMagicDefend__Float__ = 12,
                    DynamicBlood__Float__ = 13,
                    DynamicMagicAttack__Float__ = 14,
                    DynamicPhysicalPierce__Float__ = 15,
                    DynamicMagicPierce__Float__ = 16,
                    DynamicDodge__Float__ = 17,
            

            __Count__ = 18
        };
    }
    namespace t_player {
        enum eColumn {
        Account__UInt64__PrimaryUnique = 0,
                    Name__String__Unique = 1,
                    Gold__UInt32__ = 2,
                    Diamond__UInt32__ = 3,
                    DeployCharacterID__UInt64__ = 4,
            

            __Count__ = 5
        };
    }
    namespace t_skill {
        enum eColumn {
        Guid__UInt64__PrimaryUnique = 0,
                    Account__UInt64__ = 1,
                    CharacterID__UInt64__ = 2,
                    ConfigID__Int64__ = 3,
                    SkillSlot__String__ = 4,
                    LastCastTick__UInt64__ = 5,
            

            __Count__ = 6
        };
    }
    namespace t_task {
        enum eColumn {
        Account__UInt64__PrimaryUnique = 0,
                    TaskData__String__ = 1,
            

            __Count__ = 2
        };
    }


    struct ColumnTemplate {
        const std::string _Name;
        const int _Index;
        const eColumnType _Type;
        const bool _Primary;
        const bool _Unique;

        ColumnTemplate(const std::string& name, const int index, const eColumnType type, const bool primary, const bool unique) : _Name(name), _Index(index), _Type(type), _Primary(primary), _Unique(unique) {}
    };

    struct TableTemplate {
        const std::string _Name;
        const eTable _Index;
        const int _ColumnCount;
        const int _PrimaryColumn;
        const std::vector<ColumnTemplate> _ColumnTempates;

        __forceinline const ColumnTemplate& GetKeyColumnTemplate() const {
            return _ColumnTempates[_PrimaryColumn];
        }

        TableTemplate(
            const std::string& name,
            const eTable index,
            const int columnCount,
            const int primaryColumn,
            const std::vector<ColumnTemplate>& columnTemplates
        ) : _Name(name), _Index(index), _ColumnCount(columnCount), _PrimaryColumn(primaryColumn), _ColumnTempates(columnTemplates) {
        }

    };

    const static TableTemplate G_TableTemplate[(int)eTable::__Count__] =
    {
        TableTemplate("t_account", eTable::t_account, t_account::__Count__, t_account::Guid__UInt64__PrimaryUnique,
            std::vector<ColumnTemplate>({
                ColumnTemplate("Platform", t_account::Platform__Int32__, eColumnType::__Int32__, false, false),ColumnTemplate("Guid", t_account::Guid__UInt64__PrimaryUnique, eColumnType::__UInt64__, true, true),ColumnTemplate("UnionID", t_account::UnionID__String__, eColumnType::__String__, false, false),ColumnTemplate("LastLoginTick", t_account::LastLoginTick__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("RegistTick", t_account::RegistTick__Int64__, eColumnType::__Int64__, false, false),
            })
        ),
        TableTemplate("t_backpack", eTable::t_backpack, t_backpack::__Count__, t_backpack::CharacterId__UInt64__PrimaryUnique,
            std::vector<ColumnTemplate>({
                ColumnTemplate("Account", t_backpack::Account__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("CharacterId", t_backpack::CharacterId__UInt64__PrimaryUnique, eColumnType::__UInt64__, true, true),ColumnTemplate("BackPackData", t_backpack::BackPackData__String__, eColumnType::__String__, false, false),
            })
        ),
        TableTemplate("t_character", eTable::t_character, t_character::__Count__, t_character::Guid__UInt64__PrimaryUnique,
            std::vector<ColumnTemplate>({
                ColumnTemplate("Account", t_character::Account__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("Guid", t_character::Guid__UInt64__PrimaryUnique, eColumnType::__UInt64__, true, true),ColumnTemplate("ConfigID", t_character::ConfigID__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("Level", t_character::Level__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("Exp", t_character::Exp__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("LastPublicSceneConfigID", t_character::LastPublicSceneConfigID__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("SceneGUID", t_character::SceneGUID__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("X", t_character::X__Float__, eColumnType::__Float__, false, false),ColumnTemplate("Y", t_character::Y__Float__, eColumnType::__Float__, false, false),ColumnTemplate("Angle", t_character::Angle__Float__, eColumnType::__Float__, false, false),ColumnTemplate("SlotCloths", t_character::SlotCloths__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("ClothsConfigID", t_character::ClothsConfigID__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("SlotPants", t_character::SlotPants__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("PantsConfigID", t_character::PantsConfigID__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("SlotBelt", t_character::SlotBelt__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("BeltConfigID", t_character::BeltConfigID__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("SlotShoes", t_character::SlotShoes__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("ShoesConfigID", t_character::ShoesConfigID__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("SlotJewelry", t_character::SlotJewelry__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("JewelryConfigID", t_character::JewelryConfigID__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("SlotWeapon", t_character::SlotWeapon__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("WeaponConfigID", t_character::WeaponConfigID__Int64__, eColumnType::__Int64__, false, false),
            })
        ),
        TableTemplate("t_equip", eTable::t_equip, t_equip::__Count__, t_equip::Guid__UInt64__PrimaryUnique,
            std::vector<ColumnTemplate>({
                ColumnTemplate("Guid", t_equip::Guid__UInt64__PrimaryUnique, eColumnType::__UInt64__, true, true),ColumnTemplate("ConfigID", t_equip::ConfigID__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("Equiped", t_equip::Equiped__Bool__, eColumnType::__Bool__, false, false),ColumnTemplate("EquipType", t_equip::EquipType__Int32__, eColumnType::__Int32__, false, false),ColumnTemplate("Patience", t_equip::Patience__Int32__, eColumnType::__Int32__, false, false),ColumnTemplate("StrengthLv", t_equip::StrengthLv__Int32__, eColumnType::__Int32__, false, false),ColumnTemplate("DynamicAtk", t_equip::DynamicAtk__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicSpeed", t_equip::DynamicSpeed__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicHp", t_equip::DynamicHp__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicCritical", t_equip::DynamicCritical__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicCriticalDamage", t_equip::DynamicCriticalDamage__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicDefend", t_equip::DynamicDefend__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicMagicDefend", t_equip::DynamicMagicDefend__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicBlood", t_equip::DynamicBlood__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicMagicAttack", t_equip::DynamicMagicAttack__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicPhysicalPierce", t_equip::DynamicPhysicalPierce__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicMagicPierce", t_equip::DynamicMagicPierce__Float__, eColumnType::__Float__, false, false),ColumnTemplate("DynamicDodge", t_equip::DynamicDodge__Float__, eColumnType::__Float__, false, false),
            })
        ),
        TableTemplate("t_player", eTable::t_player, t_player::__Count__, t_player::Account__UInt64__PrimaryUnique,
            std::vector<ColumnTemplate>({
                ColumnTemplate("Account", t_player::Account__UInt64__PrimaryUnique, eColumnType::__UInt64__, true, true),ColumnTemplate("Name", t_player::Name__String__Unique, eColumnType::__String__, false, true),ColumnTemplate("Gold", t_player::Gold__UInt32__, eColumnType::__UInt32__, false, false),ColumnTemplate("Diamond", t_player::Diamond__UInt32__, eColumnType::__UInt32__, false, false),ColumnTemplate("DeployCharacterID", t_player::DeployCharacterID__UInt64__, eColumnType::__UInt64__, false, false),
            })
        ),
        TableTemplate("t_skill", eTable::t_skill, t_skill::__Count__, t_skill::Guid__UInt64__PrimaryUnique,
            std::vector<ColumnTemplate>({
                ColumnTemplate("Guid", t_skill::Guid__UInt64__PrimaryUnique, eColumnType::__UInt64__, true, true),ColumnTemplate("Account", t_skill::Account__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("CharacterID", t_skill::CharacterID__UInt64__, eColumnType::__UInt64__, false, false),ColumnTemplate("ConfigID", t_skill::ConfigID__Int64__, eColumnType::__Int64__, false, false),ColumnTemplate("SkillSlot", t_skill::SkillSlot__String__, eColumnType::__String__, false, false),ColumnTemplate("LastCastTick", t_skill::LastCastTick__UInt64__, eColumnType::__UInt64__, false, false),
            })
        ),
        TableTemplate("t_task", eTable::t_task, t_task::__Count__, t_task::Account__UInt64__PrimaryUnique,
            std::vector<ColumnTemplate>({
                ColumnTemplate("Account", t_task::Account__UInt64__PrimaryUnique, eColumnType::__UInt64__, true, true),ColumnTemplate("TaskData", t_task::TaskData__String__, eColumnType::__String__, false, false),
            })
        )

    };

    __forceinline const TableTemplate* GetTableTemplate(const eTable tab) {
        return &(G_TableTemplate[(int)tab]);
    }
}

#endif //__SQLCacheTemplate_h__
