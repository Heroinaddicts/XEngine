#pragma once

#include "MultiSys.h"
#include "AutoConfigData.h"

namespace AutoConfig {
    class AutoConfigData : public IAutoConfig {
    public:
        virtual ~AutoConfigData() {}

        static const AutoConfigData* GetInstance() {
            static AutoConfigData* s_Instance = nullptr;
            if (s_Instance == nullptr) {
                s_Instance = txnew AutoConfigData();
            }

            return s_Instance;
        }

        __forceinline const Struct::Buff* QueryBuffConfig(const Int64 id) const override {
            auto itor = _BuffData.find(id);
            if (_BuffData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Buff*> & GetBuffConfigMap() const override {
            return _BuffData;
        }

        __forceinline const Struct::Bullet* QueryBulletConfig(const Int64 id) const override {
            auto itor = _BulletData.find(id);
            if (_BulletData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Bullet*> & GetBulletConfigMap() const override {
            return _BulletData;
        }

        __forceinline const Struct::Character* QueryCharacterConfig(const Int64 id) const override {
            auto itor = _CharacterData.find(id);
            if (_CharacterData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Character*> & GetCharacterConfigMap() const override {
            return _CharacterData;
        }

        __forceinline const Struct::CharacterLevel* QueryCharacterLevelConfig(const Int64 id) const override {
            auto itor = _CharacterLevelData.find(id);
            if (_CharacterLevelData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::CharacterLevel*> & GetCharacterLevelConfigMap() const override {
            return _CharacterLevelData;
        }

        __forceinline const Struct::CharacterStrengthen* QueryCharacterStrengthenConfig(const Int64 id) const override {
            auto itor = _CharacterStrengthenData.find(id);
            if (_CharacterStrengthenData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::CharacterStrengthen*> & GetCharacterStrengthenConfigMap() const override {
            return _CharacterStrengthenData;
        }

        __forceinline const Struct::Consume* QueryConsumeConfig(const Int64 id) const override {
            auto itor = _ConsumeData.find(id);
            if (_ConsumeData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Consume*> & GetConsumeConfigMap() const override {
            return _ConsumeData;
        }

        __forceinline const Struct::DropResource* QueryDropResourceConfig(const Int64 id) const override {
            auto itor = _DropResourceData.find(id);
            if (_DropResourceData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::DropResource*> & GetDropResourceConfigMap() const override {
            return _DropResourceData;
        }

        __forceinline const Struct::Dynamic* QueryDynamicConfig(const Int64 id) const override {
            auto itor = _DynamicData.find(id);
            if (_DynamicData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Dynamic*> & GetDynamicConfigMap() const override {
            return _DynamicData;
        }

        __forceinline const Struct::Equipment* QueryEquipmentConfig(const Int64 id) const override {
            auto itor = _EquipmentData.find(id);
            if (_EquipmentData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Equipment*> & GetEquipmentConfigMap() const override {
            return _EquipmentData;
        }

        __forceinline const Struct::EquipmentStrengthen* QueryEquipmentStrengthenConfig(const Int64 id) const override {
            auto itor = _EquipmentStrengthenData.find(id);
            if (_EquipmentStrengthenData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::EquipmentStrengthen*> & GetEquipmentStrengthenConfigMap() const override {
            return _EquipmentStrengthenData;
        }

        __forceinline const Struct::Launcher* QueryLauncherConfig(const Int64 id) const override {
            auto itor = _LauncherData.find(id);
            if (_LauncherData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Launcher*> & GetLauncherConfigMap() const override {
            return _LauncherData;
        }

        __forceinline const Struct::Material* QueryMaterialConfig(const Int64 id) const override {
            auto itor = _MaterialData.find(id);
            if (_MaterialData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Material*> & GetMaterialConfigMap() const override {
            return _MaterialData;
        }

        __forceinline const Struct::Monster* QueryMonsterConfig(const Int64 id) const override {
            auto itor = _MonsterData.find(id);
            if (_MonsterData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Monster*> & GetMonsterConfigMap() const override {
            return _MonsterData;
        }

        __forceinline const Struct::MonsterBoss* QueryMonsterBossConfig(const Int64 id) const override {
            auto itor = _MonsterBossData.find(id);
            if (_MonsterBossData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::MonsterBoss*> & GetMonsterBossConfigMap() const override {
            return _MonsterBossData;
        }

        __forceinline const Struct::MonsterQuality* QueryMonsterQualityConfig(const Int64 id) const override {
            auto itor = _MonsterQualityData.find(id);
            if (_MonsterQualityData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::MonsterQuality*> & GetMonsterQualityConfigMap() const override {
            return _MonsterQualityData;
        }

        __forceinline const Struct::MonsterSkill* QueryMonsterSkillConfig(const Int64 id) const override {
            auto itor = _MonsterSkillData.find(id);
            if (_MonsterSkillData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::MonsterSkill*> & GetMonsterSkillConfigMap() const override {
            return _MonsterSkillData;
        }

        __forceinline const Struct::Monsterkill* QueryMonsterkillConfig(const Int64 id) const override {
            auto itor = _MonsterkillData.find(id);
            if (_MonsterkillData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Monsterkill*> & GetMonsterkillConfigMap() const override {
            return _MonsterkillData;
        }

        __forceinline const Struct::Move* QueryMoveConfig(const Int64 id) const override {
            auto itor = _MoveData.find(id);
            if (_MoveData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Move*> & GetMoveConfigMap() const override {
            return _MoveData;
        }

        __forceinline const Struct::NonePerfom* QueryNonePerfomConfig(const Int64 id) const override {
            auto itor = _NonePerfomData.find(id);
            if (_NonePerfomData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::NonePerfom*> & GetNonePerfomConfigMap() const override {
            return _NonePerfomData;
        }

        __forceinline const Struct::Npc* QueryNpcConfig(const Int64 id) const override {
            auto itor = _NpcData.find(id);
            if (_NpcData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Npc*> & GetNpcConfigMap() const override {
            return _NpcData;
        }

        __forceinline const Struct::NpcPositionChange* QueryNpcPositionChangeConfig(const Int64 id) const override {
            auto itor = _NpcPositionChangeData.find(id);
            if (_NpcPositionChangeData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::NpcPositionChange*> & GetNpcPositionChangeConfigMap() const override {
            return _NpcPositionChangeData;
        }

        __forceinline const Struct::NpcTalk* QueryNpcTalkConfig(const Int64 id) const override {
            auto itor = _NpcTalkData.find(id);
            if (_NpcTalkData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::NpcTalk*> & GetNpcTalkConfigMap() const override {
            return _NpcTalkData;
        }

        __forceinline const Struct::OrganType* QueryOrganTypeConfig(const Int64 id) const override {
            auto itor = _OrganTypeData.find(id);
            if (_OrganTypeData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::OrganType*> & GetOrganTypeConfigMap() const override {
            return _OrganTypeData;
        }

        __forceinline const Struct::Prerequisite* QueryPrerequisiteConfig(const Int64 id) const override {
            auto itor = _PrerequisiteData.find(id);
            if (_PrerequisiteData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Prerequisite*> & GetPrerequisiteConfigMap() const override {
            return _PrerequisiteData;
        }

        __forceinline const Struct::Quality* QueryQualityConfig(const Int64 id) const override {
            auto itor = _QualityData.find(id);
            if (_QualityData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Quality*> & GetQualityConfigMap() const override {
            return _QualityData;
        }

        __forceinline const Struct::RefreshRule* QueryRefreshRuleConfig(const Int64 id) const override {
            auto itor = _RefreshRuleData.find(id);
            if (_RefreshRuleData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::RefreshRule*> & GetRefreshRuleConfigMap() const override {
            return _RefreshRuleData;
        }

        __forceinline const Struct::Resource* QueryResourceConfig(const Int64 id) const override {
            auto itor = _ResourceData.find(id);
            if (_ResourceData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Resource*> & GetResourceConfigMap() const override {
            return _ResourceData;
        }

        __forceinline const Struct::ResourceQuality* QueryResourceQualityConfig(const Int64 id) const override {
            auto itor = _ResourceQualityData.find(id);
            if (_ResourceQualityData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::ResourceQuality*> & GetResourceQualityConfigMap() const override {
            return _ResourceQualityData;
        }

        __forceinline const Struct::Scene* QuerySceneConfig(const Int64 id) const override {
            auto itor = _SceneData.find(id);
            if (_SceneData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Scene*> & GetSceneConfigMap() const override {
            return _SceneData;
        }

        __forceinline const Struct::Skill* QuerySkillConfig(const Int64 id) const override {
            auto itor = _SkillData.find(id);
            if (_SkillData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Skill*> & GetSkillConfigMap() const override {
            return _SkillData;
        }

        __forceinline const Struct::Sound* QuerySoundConfig(const Int64 id) const override {
            auto itor = _SoundData.find(id);
            if (_SoundData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Sound*> & GetSoundConfigMap() const override {
            return _SoundData;
        }

        __forceinline const Struct::Special* QuerySpecialConfig(const Int64 id) const override {
            auto itor = _SpecialData.find(id);
            if (_SpecialData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Special*> & GetSpecialConfigMap() const override {
            return _SpecialData;
        }

        __forceinline const Struct::SpecialEffects* QuerySpecialEffectsConfig(const Int64 id) const override {
            auto itor = _SpecialEffectsData.find(id);
            if (_SpecialEffectsData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::SpecialEffects*> & GetSpecialEffectsConfigMap() const override {
            return _SpecialEffectsData;
        }

        __forceinline const Struct::TalkContent* QueryTalkContentConfig(const Int64 id) const override {
            auto itor = _TalkContentData.find(id);
            if (_TalkContentData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::TalkContent*> & GetTalkContentConfigMap() const override {
            return _TalkContentData;
        }

        __forceinline const Struct::Task* QueryTaskConfig(const Int64 id) const override {
            auto itor = _TaskData.find(id);
            if (_TaskData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Task*> & GetTaskConfigMap() const override {
            return _TaskData;
        }

        __forceinline const Struct::TaskCompleteCondition* QueryTaskCompleteConditionConfig(const Int64 id) const override {
            auto itor = _TaskCompleteConditionData.find(id);
            if (_TaskCompleteConditionData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::TaskCompleteCondition*> & GetTaskCompleteConditionConfigMap() const override {
            return _TaskCompleteConditionData;
        }

        __forceinline const Struct::TaskContact* QueryTaskContactConfig(const Int64 id) const override {
            auto itor = _TaskContactData.find(id);
            if (_TaskContactData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::TaskContact*> & GetTaskContactConfigMap() const override {
            return _TaskContactData;
        }

        __forceinline const Struct::TaskEvent* QueryTaskEventConfig(const Int64 id) const override {
            auto itor = _TaskEventData.find(id);
            if (_TaskEventData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::TaskEvent*> & GetTaskEventConfigMap() const override {
            return _TaskEventData;
        }

        __forceinline const Struct::TaskKill* QueryTaskKillConfig(const Int64 id) const override {
            auto itor = _TaskKillData.find(id);
            if (_TaskKillData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::TaskKill*> & GetTaskKillConfigMap() const override {
            return _TaskKillData;
        }

        __forceinline const Struct::TaskLevel* QueryTaskLevelConfig(const Int64 id) const override {
            auto itor = _TaskLevelData.find(id);
            if (_TaskLevelData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::TaskLevel*> & GetTaskLevelConfigMap() const override {
            return _TaskLevelData;
        }

        __forceinline const Struct::TaskMaterail* QueryTaskMaterailConfig(const Int64 id) const override {
            auto itor = _TaskMaterailData.find(id);
            if (_TaskMaterailData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::TaskMaterail*> & GetTaskMaterailConfigMap() const override {
            return _TaskMaterailData;
        }

        __forceinline const Struct::TaskSystem* QueryTaskSystemConfig(const Int64 id) const override {
            auto itor = _TaskSystemData.find(id);
            if (_TaskSystemData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::TaskSystem*> & GetTaskSystemConfigMap() const override {
            return _TaskSystemData;
        }

        __forceinline const Struct::TaskUse* QueryTaskUseConfig(const Int64 id) const override {
            auto itor = _TaskUseData.find(id);
            if (_TaskUseData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::TaskUse*> & GetTaskUseConfigMap() const override {
            return _TaskUseData;
        }

        __forceinline const Struct::Tests* QueryTestsConfig(const Int64 id) const override {
            auto itor = _TestsData.find(id);
            if (_TestsData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::Tests*> & GetTestsConfigMap() const override {
            return _TestsData;
        }

        __forceinline const Struct::TransPort* QueryTransPortConfig(const Int64 id) const override {
            auto itor = _TransPortData.find(id);
            if (_TransPortData.end() != itor) {
                return itor->second;
            }
            return nullptr;
        }

        __forceinline const std::unordered_map<Int64, Struct::TransPort*> & GetTransPortConfigMap() const override {
            return _TransPortData;
        }


    private:
        std::unordered_map<Int64, Struct::Buff*> _BuffData;
        std::unordered_map<Int64, Struct::Bullet*> _BulletData;
        std::unordered_map<Int64, Struct::Character*> _CharacterData;
        std::unordered_map<Int64, Struct::CharacterLevel*> _CharacterLevelData;
        std::unordered_map<Int64, Struct::CharacterStrengthen*> _CharacterStrengthenData;
        std::unordered_map<Int64, Struct::Consume*> _ConsumeData;
        std::unordered_map<Int64, Struct::DropResource*> _DropResourceData;
        std::unordered_map<Int64, Struct::Dynamic*> _DynamicData;
        std::unordered_map<Int64, Struct::Equipment*> _EquipmentData;
        std::unordered_map<Int64, Struct::EquipmentStrengthen*> _EquipmentStrengthenData;
        std::unordered_map<Int64, Struct::Launcher*> _LauncherData;
        std::unordered_map<Int64, Struct::Material*> _MaterialData;
        std::unordered_map<Int64, Struct::Monster*> _MonsterData;
        std::unordered_map<Int64, Struct::MonsterBoss*> _MonsterBossData;
        std::unordered_map<Int64, Struct::MonsterQuality*> _MonsterQualityData;
        std::unordered_map<Int64, Struct::MonsterSkill*> _MonsterSkillData;
        std::unordered_map<Int64, Struct::Monsterkill*> _MonsterkillData;
        std::unordered_map<Int64, Struct::Move*> _MoveData;
        std::unordered_map<Int64, Struct::NonePerfom*> _NonePerfomData;
        std::unordered_map<Int64, Struct::Npc*> _NpcData;
        std::unordered_map<Int64, Struct::NpcPositionChange*> _NpcPositionChangeData;
        std::unordered_map<Int64, Struct::NpcTalk*> _NpcTalkData;
        std::unordered_map<Int64, Struct::OrganType*> _OrganTypeData;
        std::unordered_map<Int64, Struct::Prerequisite*> _PrerequisiteData;
        std::unordered_map<Int64, Struct::Quality*> _QualityData;
        std::unordered_map<Int64, Struct::RefreshRule*> _RefreshRuleData;
        std::unordered_map<Int64, Struct::Resource*> _ResourceData;
        std::unordered_map<Int64, Struct::ResourceQuality*> _ResourceQualityData;
        std::unordered_map<Int64, Struct::Scene*> _SceneData;
        std::unordered_map<Int64, Struct::Skill*> _SkillData;
        std::unordered_map<Int64, Struct::Sound*> _SoundData;
        std::unordered_map<Int64, Struct::Special*> _SpecialData;
        std::unordered_map<Int64, Struct::SpecialEffects*> _SpecialEffectsData;
        std::unordered_map<Int64, Struct::TalkContent*> _TalkContentData;
        std::unordered_map<Int64, Struct::Task*> _TaskData;
        std::unordered_map<Int64, Struct::TaskCompleteCondition*> _TaskCompleteConditionData;
        std::unordered_map<Int64, Struct::TaskContact*> _TaskContactData;
        std::unordered_map<Int64, Struct::TaskEvent*> _TaskEventData;
        std::unordered_map<Int64, Struct::TaskKill*> _TaskKillData;
        std::unordered_map<Int64, Struct::TaskLevel*> _TaskLevelData;
        std::unordered_map<Int64, Struct::TaskMaterail*> _TaskMaterailData;
        std::unordered_map<Int64, Struct::TaskSystem*> _TaskSystemData;
        std::unordered_map<Int64, Struct::TaskUse*> _TaskUseData;
        std::unordered_map<Int64, Struct::Tests*> _TestsData;
        std::unordered_map<Int64, Struct::TransPort*> _TransPortData;

        AutoConfigData() {
            _BuffData.insert({ 1001, txnew Struct::Buff({ 1001 }, {Enum::eAttribute::CantController }, {"眩晕束缚"}, { 1 }, { 10 }, { 7200 }, { 0 }, { 1 }, {"UI/Sprites/Battle/Buff/BundBuff"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1020 }, { true }, {Enum::eAddType::Current }, {Enum::eTargetType::Character }, { true }, {"Entangle"}, { true }, { true }, { false }, { true }, { 5 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1002, txnew Struct::Buff({ 1002 }, {Enum::eAttribute::Invincible }, {"无敌（免疫伤害）"}, { 1 }, { 0 }, { 15000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1002 }, { false }, {Enum::eAddType::Current }, {Enum::eTargetType::PlayerSelf }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1003, txnew Struct::Buff({ 1003 }, {Enum::eAttribute::Hp }, {"加血"}, { 0.1 }, { 0 }, { 2000 }, { 1000 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { false }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1003 }, { false }, {Enum::eAddType::Max }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1004, txnew Struct::Buff({ 1004 }, {Enum::eAttribute::MoveSpeed }, {"减速"}, { -0.45 }, { 0 }, { 2000 }, { 0 }, { 1 }, {"UI/Sprites/Battle/Buff/DesSpeed"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1004 }, { false }, {Enum::eAddType::Current }, {Enum::eTargetType::Monster }, { true }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1005, txnew Struct::Buff({ 1005 }, {Enum::eAttribute::Mockery }, {"嘲讽"}, { 1 }, { 0 }, { 3000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1005 }, { false }, {Enum::eAddType::Current }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1006, txnew Struct::Buff({ 1006 }, {Enum::eAttribute::ExpAdd }, {"加经验"}, { 1.1 }, { 0 }, { 0 }, { 10 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1001 }, { false }, {Enum::eAddType::Current }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1007, txnew Struct::Buff({ 1007 }, {Enum::eAttribute::Critical }, {"眩晕"}, { 0.1 }, { 0 }, { 4500 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1002 }, { true }, {Enum::eAddType::Current }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1008, txnew Struct::Buff({ 1008 }, {Enum::eAttribute::Blood }, {"加吸血"}, { 0.01 }, { 0 }, { 2000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { false }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1003 }, { false }, {Enum::eAddType::Current }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1009, txnew Struct::Buff({ 1009 }, {Enum::eAttribute::AddDefend }, {"加物理防御"}, { 0.01 }, { 0 }, { 2000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { false }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1004 }, { false }, {Enum::eAddType::Current }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1010, txnew Struct::Buff({ 1010 }, {Enum::eAttribute::AddMagicDefend }, {"魔法防御"}, { -0.1 }, { 0 }, { 2000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { false }, {Enum::eCalculationMethod::Normal }, {Enum::eDamageType::Physic }, { 1005 }, { false }, {Enum::eAddType::None }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1011, txnew Struct::Buff({ 1011 }, {Enum::eAttribute::Mp }, {"蓝值"}, { 3 }, { 0 }, { 5000 }, { 1000 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { false }, {Enum::eCalculationMethod::Normal }, {Enum::eDamageType::Physic }, { 1001 }, { false }, {Enum::eAddType::None }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1012, txnew Struct::Buff({ 1012 }, {Enum::eAttribute::ExpandVield }, {"扩大视野范围"}, { 100 }, { 0 }, { 4000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Normal }, {Enum::eDamageType::Physic }, { 1002 }, { false }, {Enum::eAddType::None }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1013, txnew Struct::Buff({ 1013 }, {Enum::eAttribute::SuperArmor }, {"霸体（免疫控制）"}, { 150 }, { 0 }, { 3000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Normal }, {Enum::eDamageType::Physic }, { 1003 }, { false }, {Enum::eAddType::None }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1014, txnew Struct::Buff({ 1014 }, {Enum::eAttribute::AtkSpeed }, {"攻击速度"}, { 150 }, { 0 }, { 1000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Normal }, {Enum::eDamageType::Physic }, { 1004 }, { false }, {Enum::eAddType::None }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1015, txnew Struct::Buff({ 1015 }, {Enum::eAttribute::Restraint }, {"束缚"}, { 0 }, { 0 }, { 2000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Normal }, {Enum::eDamageType::Physic }, { 1005 }, { true }, {Enum::eAddType::None }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1016, txnew Struct::Buff({ 1016 }, {Enum::eAttribute::Silent }, {"沉默"}, { 0 }, { 0 }, { 2000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Normal }, {Enum::eDamageType::Physic }, { 1001 }, { false }, {Enum::eAddType::None }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1017, txnew Struct::Buff({ 1017 }, {Enum::eAttribute::Attack }, {"攻击力"}, { 1.5 }, { 0 }, { 2000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Normal }, {Enum::eDamageType::Physic }, { 1023 }, { false }, {Enum::eAddType::None }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1018, txnew Struct::Buff({ 1018 }, {Enum::eAttribute::MoveSpeed }, {"移动速度"}, { 0 }, { 0 }, { 2000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Normal }, {Enum::eDamageType::Physic }, { 1003 }, { false }, {Enum::eAddType::None }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1019, txnew Struct::Buff({ 1019 }, {Enum::eAttribute::CriticalAtk }, {"暴击伤害"}, { 0.5 }, { 0 }, { 1000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Normal }, {Enum::eDamageType::Physic }, { 1004 }, { false }, {Enum::eAddType::None }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1020, txnew Struct::Buff({ 1020 }, {Enum::eAttribute::CantController }, {"声波眩晕"}, { 1 }, { 10 }, { 1500 }, { 0 }, { 1 }, {"UI/Sprites/Battle/Buff/Dizzy"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1020 }, { true }, {Enum::eAddType::Current }, {Enum::eTargetType::Character }, { false }, {"xx"}, { false }, { true }, { false }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1021, txnew Struct::Buff({ 1021 }, {Enum::eAttribute::MoveSpeed }, {"声波减速"}, { -0.4 }, { 1 }, { 4500 }, { 0 }, { 1 }, {"UI/Sprites/Battle/Buff/DesSpeed"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1021 }, { true }, {Enum::eAddType::Current }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1022, txnew Struct::Buff({ 1022 }, {Enum::eAttribute::CantController }, {"声波眩晕"}, { 1 }, { 10 }, { 3500 }, { 0 }, { 1 }, {"UI/Sprites/Battle/Buff/Dizzy"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1020 }, { true }, {Enum::eAddType::Current }, {Enum::eTargetType::Character }, { false }, {"xx"}, { false }, { true }, { false }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1023, txnew Struct::Buff({ 1023 }, {Enum::eAttribute::Hp }, {"九头蛇柏缠绕扣血"}, { -0.04 }, { 0 }, { 7200 }, { 1200 }, { 1 }, {"UI/Sprites/Battle/Buff/BloodBuff"}, { false }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1024 }, { false }, {Enum::eAddType::Max }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { true }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1024, txnew Struct::Buff({ 1024 }, {Enum::eAttribute::MoveSpeed }, {"减速"}, { -0.35 }, { 0 }, { 2500 }, { 0 }, { 1 }, {"UI/Sprites/Battle/Buff/DesSpeed"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1021 }, { true }, {Enum::eAddType::Current }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { true }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1025, txnew Struct::Buff({ 1025 }, {Enum::eAttribute::Hp }, {"踢腿持续流血"}, { -0.02 }, { 0 }, { 2400 }, { 1800 }, { 1 }, {"UI/Sprites/Battle/Buff/BloodBuff"}, { false }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1024 }, { false }, {Enum::eAddType::Max }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { true }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1026, txnew Struct::Buff({ 1026 }, {Enum::eAttribute::Hp }, {"血尸攻击流血"}, { -0.02 }, { 0 }, { 2000 }, { 1000 }, { 1 }, {"UI/Sprites/Battle/Buff/BloodBuff"}, { false }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1022 }, { false }, {Enum::eAddType::Max }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { true }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1027, txnew Struct::Buff({ 1027 }, {Enum::eAttribute::Attack }, {"潘子加攻击力"}, { 0.4 }, { 0 }, { 6000 }, { 0 }, { 1 }, {"UI/Sprites/Character/TrueHead"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1025 }, { false }, {Enum::eAddType::Current }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BuffData.insert({ 1028, txnew Struct::Buff({ 1028 }, {Enum::eAttribute::MoveSpeed }, {"潘子加移速"}, { 0.3 }, { 0 }, { 6000 }, { 0 }, { 1 }, {"UI/Sprites/Battle/Buff/DesSpeed"}, { true }, {Enum::eCalculationMethod::Percent }, {Enum::eDamageType::Physic }, { 1025 }, { false }, {Enum::eAddType::Current }, {Enum::eTargetType::Monster }, { false }, {"xx"}, { false }, { false }, { true }, { false }, { 12 }, { 1200 }, { 1200 })});
            _BulletData.insert({ 11001, txnew Struct::Bullet({ 11001 }, {"小哥测试血阵"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Ellipse }, { 5 }, { 40 }, {10,10}, { true }, { 0 }, { 375 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {"Spine/VFX/Prefabs/AttackB"}, { 2 }, { 0 }, { 3500 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 0.7 }, { 1009 }, { 0 }, { false }, { 325 }, { true }, {"blood_3"}, {"blood_2"}, {"blood_4"}, { 2000 }, { 3 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11002, txnew Struct::Bullet({ 11002 }, {"小哥测试声波攻击"}, {Enum::eBallistics::Line }, { 3 }, {Enum::eRangeType::Cube }, { 6 }, { 40 }, {4.65,5.5}, { true }, { 0 }, { 220 }, { 10 }, { 0 }, {Enum::eDamageType::Physic }, {1021}, {2}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { -0.85 }, {"Spine/VFX/Prefabs/AttackC"}, { 1 }, { 0 }, { 1700 }, { 500 }, { 1.5 }, {Enum::ePhysicType::None}, { 2.5 }, { 1010 }, { 0 }, { false }, { 425 }, { false }, {}, {}, {}, { 0 }, { 5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11003, txnew Struct::Bullet({ 11003 }, {"跟踪弹"}, {Enum::eBallistics::Track }, { 4 }, {Enum::eRangeType::None }, { 2 }, { 40 }, {10,10}, { true }, { 1 }, { 10 }, { 25 }, { 7 }, {Enum::eDamageType::Physic }, {1001}, {2}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {"Effect/Prefabs/Environment/Fire/Cartoon/PowerupGlow2"}, { 1 }, { 1011 }, { 5000 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 4 }, { 1011 }, { 1014 }, { true }, { 102 }, { false }, {}, {}, {}, { 0 }, { 5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11004, txnew Struct::Bullet({ 11004 }, {"小哥测试普攻踢腿"}, {Enum::eBallistics::Line }, { 9 }, {Enum::eRangeType::Cube }, { 4 }, { 40 }, {0,0}, { true }, { 0 }, { 3000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1025}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { -2 }, {"Spine/VFX/Prefabs/AttackA"}, { 1 }, { 0 }, { 770 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 4 }, { 1009 }, { 0 }, { false }, { 750 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11005, txnew Struct::Bullet({ 11005 }, {"血滴"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::None }, { 0 }, { 40 }, {10,10}, { true }, { 5 }, { 10 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1004}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {"Spine/VFX/Prefabs/AttackB_Blood"}, { 1 }, { 0 }, { 500 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 4 }, { 0 }, { 0 }, { false }, { 103 }, { false }, {}, {}, {}, { 0 }, { 0 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11006, txnew Struct::Bullet({ 11006 }, {"普攻1"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Cube }, { 2 }, { 210 }, {7,6.75}, { true }, { 0 }, { 115 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {}, { 1 }, { 0 }, { 585 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 0.4 }, { 1009 }, { 0 }, { false }, { 265 }, { false }, {}, {}, {}, { 0 }, { 1.65 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11007, txnew Struct::Bullet({ 11007 }, {"普攻2"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Cube }, { 2 }, { 210 }, {7,6.75}, { true }, { 0 }, { 115 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {}, { 1 }, { 0 }, { 400 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1 }, { 1009 }, { 0 }, { false }, { 1201 }, { false }, {}, {}, {}, { 0 }, { 1.65 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11008, txnew Struct::Bullet({ 11008 }, {"普攻3"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Cube }, { 2 }, { 210 }, {7,6.75}, { true }, { 0 }, { 115 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {}, { 1 }, { 0 }, { 400 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.35 }, { 1009 }, { 0 }, { false }, { 1202 }, { false }, {}, {}, {}, { 0 }, { 1.65 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11009, txnew Struct::Bullet({ 11009 }, {"野怪地刺"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Ellipse }, { 1.25 }, { 40 }, {10,10}, { true }, { 0 }, { 350 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1004}, {1}, {Enum::eTargetType::Character}, { 0 }, { 0 }, {"Spine/VFX/Monster/Prefabs/JiuTouSheBai/Caltrop"}, { 2 }, { 0 }, { 900 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1500 }, { true }, {"Snake_Caltrop-stay-A"}, {"Snake_Caltrop-A"}, {"Snake_Caltrop-back-A"}, { 675 }, { 1.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11010, txnew Struct::Bullet({ 11010 }, {"九头扫人B"}, {Enum::eBallistics::Line }, { 1.25 }, {Enum::eRangeType::Ellipse }, { 2.75 }, { 210 }, {10,10}, { true }, { 0 }, { 335 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1024}, {1}, {Enum::eTargetType::Character}, { 0 }, { 0 }, {}, { 1 }, { 0 }, { 1800 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.5 }, { 1009 }, { 0 }, { false }, { 1900 }, { false }, {}, {}, {}, { 0 }, { 13.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11011, txnew Struct::Bullet({ 11011 }, {"九头扫人D"}, {Enum::eBallistics::Line }, { 1.25 }, {Enum::eRangeType::Ellipse }, { 2.75 }, { 210 }, {10,10}, { true }, { 0 }, { 335 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1024}, {1}, {Enum::eTargetType::Character}, { 180 }, { 0 }, {}, { 1 }, { 0 }, { 1800 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.5 }, { 1009 }, { 0 }, { false }, { 1900 }, { false }, {}, {}, {}, { 0 }, { 12 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11012, txnew Struct::Bullet({ 11012 }, {"九头蛇柏抬头纯表现"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Ellipse }, { 0 }, { 210 }, {10,10}, { true }, { 0 }, { 350 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1001}, {1}, {Enum::eTargetType::Character}, { 0 }, { 0 }, {}, { 1 }, { 0 }, { 5000 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 0 }, { 1009 }, { 0 }, { false }, { 1202 }, { false }, {}, {}, {}, { 0 }, { 0 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11013, txnew Struct::Bullet({ 11013 }, {"藤曼触手怪"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Ellipse }, { 1.25 }, { 210 }, {10,10}, { true }, { 1 }, { 350 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1001,1023}, {1}, {Enum::eTargetType::Character}, { 0 }, { 0 }, {"Spine/VFX/Monster/Prefabs/JiuTouSheBai/Bunding"}, { 1 }, { 1017 }, { 1200 }, { 850 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.3 }, { 1009 }, { 0 }, { false }, { 1300 }, { true }, {"tengman-stay"}, {"tengman-in"}, {"tengman-out"}, { 4200 }, { 1.5 }, { true }, {Enum::eLifePanType::HurtLife }, { 6400 }, { false }, { true }, { -7 }, {"BulletGround2"})});
            _BulletData.insert({ 11014, txnew Struct::Bullet({ 11014 }, {"九头蛇柏发出蛇头打人"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Ellipse }, { 2.85 }, { 210 }, {10,10}, { true }, { 0 }, { 850 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1004}, {1}, {Enum::eTargetType::Character}, { 0 }, { 0 }, {"Spine/VFX/Monster/Prefabs/JiuTouSheBai/SnakeDown"}, { 1 }, { 0 }, { 2200 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 3 }, { 1009 }, { 0 }, { false }, { 1300 }, { false }, {}, {}, {}, { 0 }, { 2.25 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { -7 }, {"BulletGround2"})});
            _BulletData.insert({ 11015, txnew Struct::Bullet({ 11015 }, {"九头蛇柏插入地下"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Ellipse }, { 0 }, { 210 }, {10,10}, { true }, { 0 }, { 350 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1001}, {1}, {Enum::eTargetType::Character}, { 0 }, { 0 }, {}, { 1 }, { 0 }, { 500 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 0 }, { 1009 }, { 0 }, { false }, { 1202 }, { false }, {}, {}, {}, { 0 }, { 0 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11016, txnew Struct::Bullet({ 11016 }, {"九头扫人A"}, {Enum::eBallistics::Line }, { 1.25 }, {Enum::eRangeType::Ellipse }, { 3 }, { 210 }, {10,10}, { true }, { 0 }, { 335 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1024}, {1}, {Enum::eTargetType::Character}, { 0 }, { 0 }, {}, { 1 }, { 0 }, { 1200 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.5 }, { 1009 }, { 0 }, { false }, { 1900 }, { false }, {}, {}, {}, { 0 }, { 18 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11017, txnew Struct::Bullet({ 11017 }, {"九头扫人C"}, {Enum::eBallistics::Line }, { 1.25 }, {Enum::eRangeType::Ellipse }, { 3 }, { 210 }, {10,10}, { true }, { 0 }, { 335 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1024}, {1}, {Enum::eTargetType::Character}, { 180 }, { 0 }, {}, { 1 }, { 0 }, { 1200 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.5 }, { 1009 }, { 0 }, { false }, { 1900 }, { false }, {}, {}, {}, { 0 }, { 13 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11018, txnew Struct::Bullet({ 11018 }, {"尸鳖普攻"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Cube }, { 2 }, { 210 }, {2.85,2.85}, { true }, { 0 }, { 20 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Character}, { 0 }, { 0 }, {}, { 1 }, { 0 }, { 250 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1 }, { 0 }, { 0 }, { false }, { 700 }, { false }, {}, {}, {}, { 0 }, { 1.65 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11019, txnew Struct::Bullet({ 11019 }, {"血尸普攻"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Cube }, { 2 }, { 210 }, {6.25,6.25}, { true }, { 0 }, { 100 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1026}, {1}, {Enum::eTargetType::Character}, { 0 }, { 0 }, {}, { 1 }, { 0 }, { 550 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1 }, { 0 }, { 0 }, { false }, { 700 }, { false }, {}, {}, {}, { 0 }, { 1.65 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11020, txnew Struct::Bullet({ 11020 }, {"潘子普攻射箭"}, {Enum::eBallistics::Line }, { 23.5 }, {Enum::eRangeType::Cube }, { 4 }, { 40 }, {3.75,2.2}, { true }, { 1 }, { 30 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 1.5 }, {"Spine/VFX/Bullet/Jian"}, { 1 }, { 0 }, { 675 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1 }, { 1009 }, { 0 }, { false }, { 600 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11021, txnew Struct::Bullet({ 11021 }, {"潘子剑雨"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Ellipse }, { 6 }, { 40 }, {10,10}, { true }, { 0 }, { 200 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1021}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {"Spine/VFX/Prefabs/PanZi/JianYu"}, { 2 }, { 0 }, { 1800 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2.5 }, { 1009 }, { 0 }, { false }, { 235 }, { true }, {"Spine/VFX/Prefabs/PanZi/JianYu"}, {"Spine/VFX/Prefabs/PanZi/JianYu"}, {"Spine/VFX/Prefabs/PanZi/JianYu"}, { 0 }, { 4.05 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11022, txnew Struct::Bullet({ 11022 }, {"表现爆裂箭"}, {Enum::eBallistics::Line }, { 8 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 1000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { -2 }, {}, { 1 }, { 0 }, { 700 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11023, txnew Struct::Bullet({ 11023 }, {"潘子挂buff"}, {Enum::eBallistics::Line }, { 0 }, {Enum::eRangeType::Cube }, { 0 }, { 40 }, {1,1}, { true }, { 1 }, { 10 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1027,1028}, {1}, {Enum::eTargetType::PlayerSelf}, { 0 }, { 0 }, {"Spine/VFX/Prefabs/Buff"}, { 1 }, { 0 }, { 500 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 0 }, { 0 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 0 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround2"})});
            _BulletData.insert({ 11024, txnew Struct::Bullet({ 11024 }, {"表现分爆裂箭"}, {Enum::eBallistics::Line }, { 8 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { -2 }, {}, { 1 }, { 0 }, { 700 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11025, txnew Struct::Bullet({ 11025 }, {"表现分爆裂箭"}, {Enum::eBallistics::Line }, { 8 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 12 }, { -2 }, {}, { 1 }, { 0 }, { 700 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11026, txnew Struct::Bullet({ 11026 }, {"表现分爆裂箭"}, {Enum::eBallistics::Line }, { 8 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { -12 }, { -2 }, {}, { 1 }, { 0 }, { 700 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11027, txnew Struct::Bullet({ 11027 }, {"实际伤害多发爆裂箭"}, {Enum::eBallistics::Line }, { 23 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {"Spine/VFX/Bullet/BaoLieJian_1"}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11028, txnew Struct::Bullet({ 11028 }, {"伤害分爆裂箭1"}, {Enum::eBallistics::Line }, { 23 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {"Spine/VFX/Bullet/BaoLieJian_1"}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.5 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11029, txnew Struct::Bullet({ 11029 }, {"伤害分爆裂箭2"}, {Enum::eBallistics::Line }, { 23 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 12 }, { 0 }, {"Spine/VFX/Bullet/BaoLieJian_1"}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.5 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11030, txnew Struct::Bullet({ 11030 }, {"伤害分爆裂箭3"}, {Enum::eBallistics::Line }, { 23 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { -12 }, { 0 }, {"Spine/VFX/Bullet/BaoLieJian_1"}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.5 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11031, txnew Struct::Bullet({ 11031 }, {"小哥测试普攻踢腿"}, {Enum::eBallistics::Line }, { 17 }, {Enum::eRangeType::Cube }, { 4 }, { 40 }, {4.75,2.7}, { true }, { 0 }, { 95 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {1025}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { -1 }, {}, { 1 }, { 0 }, { 770 }, { 500 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 4 }, { 1009 }, { 0 }, { false }, { 750 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11032, txnew Struct::Bullet({ 11032 }, {"假小哥测试声波攻击"}, {Enum::eBallistics::Line }, { 7.5 }, {Enum::eRangeType::Cube }, { 6 }, { 40 }, {2.25,5}, { true }, { 0 }, { 125 }, { 10 }, { 0 }, {Enum::eDamageType::Physic }, {1021}, {2}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {}, { 1 }, { 0 }, { 750 }, { 500 }, { 1.5 }, {Enum::ePhysicType::None}, { 2.5 }, { 1010 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11033, txnew Struct::Bullet({ 11033 }, {"伤害多发爆裂箭2"}, {Enum::eBallistics::Line }, { 23 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { -2 }, { 0 }, {"Spine/VFX/Bullet/BaoLieJian_1"}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11034, txnew Struct::Bullet({ 11034 }, {"伤害多发爆裂箭3"}, {Enum::eBallistics::Line }, { 23 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 2 }, { 0 }, {"Spine/VFX/Bullet/BaoLieJian_1"}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11035, txnew Struct::Bullet({ 11035 }, {"伤害多发爆裂箭4"}, {Enum::eBallistics::Line }, { 23 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {0,0}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 1 }, { 0 }, {"Spine/VFX/Bullet/BaoLieJian_1"}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11036, txnew Struct::Bullet({ 11036 }, {"伤害多发爆裂箭5"}, {Enum::eBallistics::Line }, { 23 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {4.5,1.25}, { true }, { 0 }, { 2000 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { -1 }, { 0 }, {"Spine/VFX/Bullet/BaoLieJian_1"}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11037, txnew Struct::Bullet({ 11037 }, {"zhen伤害多发爆裂箭1"}, {Enum::eBallistics::Line }, { 28 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {4.5,1.25}, { true }, { 0 }, { 35 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11038, txnew Struct::Bullet({ 11038 }, {"zhen伤害多发爆裂箭2"}, {Enum::eBallistics::Line }, { 28 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {4.5,1.25}, { true }, { 0 }, { 35 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { -2 }, { 0 }, {}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11039, txnew Struct::Bullet({ 11039 }, {"zhen伤害多发爆裂箭3"}, {Enum::eBallistics::Line }, { 28 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {4.5,1.25}, { true }, { 0 }, { 35 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 2 }, { 0 }, {}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11040, txnew Struct::Bullet({ 11040 }, {"zhen伤害多发爆裂箭4"}, {Enum::eBallistics::Line }, { 28 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {4.5,1.25}, { true }, { 0 }, { 35 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 1 }, { 0 }, {}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11041, txnew Struct::Bullet({ 11041 }, {"zhen伤害多发爆裂箭5"}, {Enum::eBallistics::Line }, { 28 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {4.5,1.25}, { true }, { 0 }, { 35 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { -1 }, { 0 }, {}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 2 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11042, txnew Struct::Bullet({ 11042 }, {"zhen伤害分爆裂箭1"}, {Enum::eBallistics::Line }, { 29 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {4.5,1.25}, { true }, { 0 }, { 75 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 0 }, { 0 }, {}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.5 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11043, txnew Struct::Bullet({ 11043 }, {"zhen伤害分爆裂箭2"}, {Enum::eBallistics::Line }, { 29 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {4.5,1.25}, { true }, { 0 }, { 75 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { 12 }, { 0 }, {}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.5 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _BulletData.insert({ 11044, txnew Struct::Bullet({ 11044 }, {"zhen伤害分爆裂箭3"}, {Enum::eBallistics::Line }, { 29 }, {Enum::eRangeType::Cube }, { 10 }, { 40 }, {4.5,1.25}, { true }, { 0 }, { 75 }, { 25 }, { 0 }, {Enum::eDamageType::Physic }, {-1}, {1}, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { -12 }, { 0 }, {}, { 1 }, { 1026 }, { 700 }, { 350 }, { 0 }, {Enum::ePhysicType::StrengthWall}, { 1.5 }, { 1009 }, { 0 }, { false }, { 1000 }, { false }, {}, {}, {}, { 0 }, { 2.5 }, { false }, {Enum::eLifePanType::Normal }, { 0 }, { true }, { false }, { 0 }, {"BulletGround"})});
            _CharacterData.insert({ 100001, txnew Struct::Character({ 100001 }, {"小哥"}, {"Spine/Character/Prefabs/Xiaoge"}, { 7 }, { 5.8 }, { 380 }, { 0.15 }, { 2.25 }, { 10 }, { 9 }, { 0 }, { 10 }, { 0 }, { 0 }, { 50 }, { 0.03 }, { 0.03 }, { 1001 }, { 8 }, { 1 }, { 425 }, { 1 }, { 1 }, { 2002 }, {"Spine/Character/Res/WuXie/wuxie_SkeletonData"}, {11001,11002,11003,11004}, {11001,11002,11003,11004,11006,11007,11008}, {"UI/Sprites/Character/XiaogeHead"}, { 11006 }, { 11007 }, { 11008 }, {"UI/Sprites/Character/img_head_wuxie"}, {"UI/Sprites/Character/img_character_job1"}, {Enum::eResources::Brass }, { 100 }, { 600 }, { 0.1 }, { 0.1 }, { 1200 }, { 9 }, { 9 }, { 455 }, { 980 }, { 1.5 }, {Enum::eDamagePriority::First }, {"die"}, { 4018 }, { 2250 }, { 1450 }, { 1 })});
            _CharacterData.insert({ 100002, txnew Struct::Character({ 100002 }, {"小哥"}, {"Spine/Character/Prefabs/WuXie"}, { 5 }, { 3 }, { 140 }, { 0 }, { 2.25 }, { 17 }, { 14 }, { 0 }, { 8 }, { 0 }, { 0 }, { 40 }, { 0.05 }, { 0.05 }, { 1001 }, { 8 }, { 2 }, { 140 }, { 1 }, { 1 }, { 2002 }, {"Spine/Character/Res/WuXie/wuxie_SkeletonData"}, {11001,11003}, {11001,11003}, {"UI/Sprites/Character/WuXieHead"}, { 11002 }, { 11002 }, { 11002 }, {"UI/Sprites/Character/img_head_xiaoge"}, {"UI/Sprites/Character/img_character_job2"}, {Enum::eResources::Brass }, { 101 }, { 1001 }, { 0.2 }, { 0.2 }, { 1801 }, { 14 }, { 14 }, { 1001 }, { 1001 }, { 1.5 }, {Enum::eDamagePriority::First }, {"die"}, { 4018 }, { 2250 }, { 1450 }, { 1 })});
            _CharacterData.insert({ 100003, txnew Struct::Character({ 100003 }, {"王胖子"}, {"Spine/Character/Prefabs/WuXie"}, { 4 }, { 5 }, { 115 }, { 0 }, { 1.5 }, { 12 }, { 10 }, { 0 }, { 12 }, { 0 }, { 0 }, { 38 }, { 0.01 }, { 0.01 }, { 1001 }, { 8 }, { 3 }, { 115 }, { 1 }, { 1 }, { 2002 }, {"Spine/Character/Res/WuXie/wuxie_SkeletonData"}, {11001,11002}, {11001,11002}, {"UI/Sprites/Character/WpzHead"}, { 11003 }, { 11003 }, { 11003 }, {"UI/Sprites/Character/img_head_wangpangzi"}, {"UI/Sprites/Character/img_character_job3"}, {Enum::eResources::Brass }, { 102 }, { 1002 }, { 0.3 }, { 0.3 }, { 1802 }, { 10 }, { 10 }, { 1002 }, { 1002 }, { 1.5 }, {Enum::eDamagePriority::First }, {"die"}, { 4018 }, { 2250 }, { 1450 }, { 1 })});
            _CharacterData.insert({ 100004, txnew Struct::Character({ 100004 }, {"大奎"}, {"Spine/Character/Prefabs/WuXie"}, { 4 }, { 5 }, { 250 }, { 0 }, { 1.5 }, { 24 }, { 13 }, { 0 }, { 5 }, { 0 }, { 0 }, { 32 }, { 0.01 }, { 0.01 }, { 1001 }, { 8 }, { 4 }, { 180 }, { 1 }, { 1 }, { 2002 }, {"Spine/Character/Res/WuXie/wuxie_SkeletonData"}, {11001,11005}, {11001,11005}, {"UI/Sprites/Character/DkHead"}, { 11004 }, { 11004 }, { 11004 }, {"UI/Sprites/Character/img_head_wangpangzi"}, {"UI/Sprites/Character/img_character_job4"}, {Enum::eResources::Brass }, { 103 }, { 1003 }, { 0.4 }, { 0.4 }, { 1803 }, { 13 }, { 13 }, { 1003 }, { 1003 }, { 1.5 }, {Enum::eDamagePriority::First }, {"die"}, { 4018 }, { 2250 }, { 1450 }, { 1 })});
            _CharacterData.insert({ 100005, txnew Struct::Character({ 100005 }, {"潘子"}, {"Spine/Character/Prefabs/PanZi"}, { 5 }, { 5 }, { 265 }, { 0 }, { 1.8 }, { 10 }, { 9 }, { 0 }, { 7 }, { 0 }, { 0 }, { 36 }, { 0.02 }, { 0.02 }, { 1001 }, { 8 }, { 5 }, { 370 }, { 1 }, { 2 }, { 2002 }, {"Spine/Character/Res/PanZi/panzi_SkeletonData"}, {11009,11010,11011,11012}, {11009,11010,11011,11012,11013,11014,11015}, {"UI/Sprites/Character/WpzHead"}, { 11013 }, { 11014 }, { 11015 }, {"UI/Sprites/Character/img_head_panzi"}, {"UI/Sprites/Character/img_character_job5"}, {Enum::eResources::Brass }, { 104 }, { 1004 }, { 0.5 }, { 0.5 }, { 1804 }, { 9 }, { 9 }, { 1004 }, { 1004 }, { 1.5 }, {Enum::eDamagePriority::First }, {"die"}, { 4018 }, { 2250 }, { 1450 }, { 1 })});
            _CharacterLevelData.insert({ 1, txnew Struct::CharacterLevel({ 1 }, {"Lv1"}, { 100 }, { 0 }, { 1027 }, { 1 })});
            _CharacterLevelData.insert({ 2, txnew Struct::CharacterLevel({ 2 }, {"Lv1"}, { 150 }, { 5 }, { 1027 }, { 1 })});
            _CharacterLevelData.insert({ 3, txnew Struct::CharacterLevel({ 3 }, {"Lv1"}, { 200 }, { 5 }, { 1027 }, { 1 })});
            _CharacterStrengthenData.insert({ 1001, txnew Struct::CharacterStrengthen({ 1001 }, { 0.25 }, { 0.26 }, { 0.27 }, { 0.28 }, { 0.29 }, { 0.30 }, { 0.31 }, { 0.32 }, { 0.33 }, { 0.34 }, { 0.35 }, { 0.36 }, { 0.37 })});
            _ConsumeData.insert({ 5001, txnew Struct::Consume({ 5001 }, {"血瓶"}, {Enum::eTransactionType::None }, {Enum::eSoldType::None }, {"回复玩家200的生命值"}, {Enum::eResources::Coin }, { 100 }, {"UI/Sprites/Bag/Consume/Hp1.png"}, {100001}, { 10001 }, {"药水"}, { 1 }, {"RewardItem/LootItem/LootItem/Res/IMG/TrueHead"}, { 1001 })});
            _DropResourceData.insert({ 4001, txnew Struct::DropResource({ 4001 }, {Enum::eResources::Special }, { 9001 }, { 1 }, {"xx"}, { 0.2 })});
            _DropResourceData.insert({ 4002, txnew Struct::DropResource({ 4002 }, {Enum::eResources::Material }, { 5001 }, { 1 }, {"xx"}, { 0.2 })});
            _DropResourceData.insert({ 4003, txnew Struct::DropResource({ 4003 }, {Enum::eResources::FatigueValue }, { -1 }, { 1 }, {"xx"}, { 0.2 })});
            _DropResourceData.insert({ 4004, txnew Struct::DropResource({ 4004 }, {Enum::eResources::Equipment }, { 2002 }, { 1 }, {"xx"}, { 0.2 })});
            _DropResourceData.insert({ 4005, txnew Struct::DropResource({ 4005 }, {Enum::eResources::Special }, { 9001 }, { 1 }, {"xx"}, { 0.2 })});
            _DynamicData.insert({ 1001, txnew Struct::Dynamic({ 1001 }, {1,3}, {1,4}, {1,5}, {1,6}, {1,7}, {1,8}, {1,9}, {1,10}, {1,11}, {1,12}, {1,13}, {1,14})});
            _EquipmentData.insert({ 2001, txnew Struct::Equipment({ 2001 }, {"鞋子"}, {Enum::eEquipmentType::Shose }, {Enum::eTransactionType::None }, {Enum::eSoldType::Sold }, {Enum::eResources::Coin }, { 100 }, {"xxxx"}, { 5 }, { 1 }, { 30 }, { 1001 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1001 }, { 5001 }, {"xxx.png"}, { 4001 }, { false }, {"Weapon/Prefabs/t2"}, {Enum::eWeaponType::None }, {"鞋子"}, {100001}, { 1 }, { 0.1 }, { 0.1 }, {"RewardItem/LootItem/LootItem/Res/IMG/TrueHead"}, { 1001 })});
            _EquipmentData.insert({ 2002, txnew Struct::Equipment({ 2002 }, {"武器"}, {Enum::eEquipmentType::Weapon }, {Enum::eTransactionType::None }, {Enum::eSoldType::Sold }, {Enum::eResources::Coin }, { 100 }, {"xxxx"}, { 5 }, { 1 }, { 30 }, { 1001 }, { 1 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 2 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1001 }, { 5001 }, {"xxx.png"}, { 4001 }, { true }, {"Weapon/Prefabs/t1"}, {Enum::eWeaponType::Sword }, {"弓箭"}, {100002}, { 1 }, { 0.2 }, { 0.2 }, {"RewardItem/LootItem/LootItem/Res/IMG/TrueHead"}, { 1001 })});
            _EquipmentData.insert({ 2003, txnew Struct::Equipment({ 2003 }, {"裤子"}, {Enum::eEquipmentType::Pants }, {Enum::eTransactionType::None }, {Enum::eSoldType::Sold }, {Enum::eResources::Coin }, { 100 }, {"xxxx"}, { 5 }, { 1 }, { 30 }, { 1001 }, { 1 }, { 3 }, { 3 }, { 3 }, { 3 }, { 3 }, { 3 }, { 3 }, { 3 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1001 }, { 5001 }, {"xxx.png"}, { 4001 }, { false }, {"xxx"}, {Enum::eWeaponType::None }, {"裤子"}, {100003}, { 1 }, { 0.3 }, { 0.3 }, {"RewardItem/LootItem/LootItem/Res/IMG/TrueHead"}, { 1001 })});
            _EquipmentData.insert({ 2004, txnew Struct::Equipment({ 2004 }, {"衣服"}, {Enum::eEquipmentType::Cloths }, {Enum::eTransactionType::None }, {Enum::eSoldType::Sold }, {Enum::eResources::Coin }, { 100 }, {"xxxx"}, { 5 }, { 1 }, { 30 }, { 1001 }, { 1 }, { 4 }, { 4 }, { 4 }, { 4 }, { 4 }, { 4 }, { 4 }, { 4 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1001 }, { 5001 }, {"xxx.png"}, { 4001 }, { false }, {"xxx"}, {Enum::eWeaponType::None }, {"上衣"}, {100004}, { 1 }, { 0.4 }, { 0.4 }, {"RewardItem/LootItem/LootItem/Res/IMG/TrueHead"}, { 1001 })});
            _EquipmentData.insert({ 2005, txnew Struct::Equipment({ 2005 }, {"腰带"}, {Enum::eEquipmentType::Belt }, {Enum::eTransactionType::None }, {Enum::eSoldType::Sold }, {Enum::eResources::Coin }, { 100 }, {"xxxx"}, { 5 }, { 1 }, { 30 }, { 1001 }, { 1 }, { 5 }, { 5 }, { 5 }, { 5 }, { 5 }, { 5 }, { 5 }, { 5 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1001 }, { 5001 }, {"xxx.png"}, { 4001 }, { false }, {"xxx"}, {Enum::eWeaponType::None }, {"腰带"}, {100005}, { 1 }, { 0.5 }, { 0.5 }, {"RewardItem/LootItem/LootItem/Res/IMG/TrueHead"}, { 1001 })});
            _EquipmentData.insert({ 2006, txnew Struct::Equipment({ 2006 }, {"武器1"}, {Enum::eEquipmentType::Weapon }, {Enum::eTransactionType::None }, {Enum::eSoldType::Sold }, {Enum::eResources::Coin }, { 100 }, {"xxxx"}, { 5 }, { 1 }, { 30 }, { 1001 }, { 1 }, { 6 }, { 6 }, { 6 }, { 6 }, { 6 }, { 6 }, { 6 }, { 6 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1001 }, { 5001 }, {"xxx.png"}, { 4001 }, { true }, {"Weapon/Prefabs/t3"}, {Enum::eWeaponType::Sword }, {"弓箭"}, {100002}, { 1 }, { 0.6 }, { 0.6 }, {"RewardItem/LootItem/LootItem/Res/IMG/TrueHead"}, { 1001 })});
            _EquipmentStrengthenData.insert({ 1001, txnew Struct::EquipmentStrengthen({ 1001 }, { 0.25 }, { 0.26 }, { 0.27 }, { 0.28 }, { 0.29 }, { 0.30 }, { 0.31 }, { 0.32 }, { 0.33 }, { 0.34 }, { 0.35 }, { 0.36 }, { 0.37 })});
            _LauncherData.insert({ 1, txnew Struct::Launcher({ 1 }, {"弓箭1"}, {Enum::ePosition::TraceHost }, {0,1}, { true }, { true }, { 200 }, { 1 }, { 1000 }, {"xxxx"}, {11001})});
            _LauncherData.insert({ 2, txnew Struct::Launcher({ 2 }, {"手雷"}, {Enum::ePosition::TraceHost }, {0,1}, { true }, { true }, { 200 }, { 1 }, { 1000 }, {"xxxx"}, {11002})});
            _LauncherData.insert({ 3, txnew Struct::Launcher({ 3 }, {"跟踪弹"}, {Enum::ePosition::TraceHost }, {0,1}, { true }, { true }, { 200 }, { 1 }, { 1000 }, {"xxxx"}, {11003})});
            _LauncherData.insert({ 4, txnew Struct::Launcher({ 4 }, {"测试小哥踢腿普攻"}, {Enum::ePosition::TraceHost }, {0.35,0.45}, { true }, { true }, { 150 }, { 1 }, { 1100 }, {"xxxx"}, {11004,11031})});
            _LauncherData.insert({ 5, txnew Struct::Launcher({ 5 }, {"测试小哥声波攻击"}, {Enum::ePosition::TraceHost }, {0.85,0.15}, { true }, { true }, { 200 }, { 1 }, { 1000 }, {"xxxx"}, {11002})});
            _LauncherData.insert({ 6, txnew Struct::Launcher({ 6 }, {"测试小哥血阵"}, {Enum::ePosition::WorldPosition }, {0,0}, { true }, { true }, { 335 }, { 1 }, { 1000 }, {"xxxx"}, {11001})});
            _LauncherData.insert({ 7, txnew Struct::Launcher({ 7 }, {"测试小哥血滴"}, {Enum::ePosition::TraceHost }, {-1.5,0.3}, { true }, { true }, { 20 }, { 1 }, { 1000 }, {"xxxx"}, {11005})});
            _LauncherData.insert({ 8, txnew Struct::Launcher({ 8 }, {"测试小哥普攻1"}, {Enum::ePosition::TraceHost }, {2.85,1.75}, { true }, { true }, { 20 }, { 1 }, { 600 }, {"xxxx"}, {11006})});
            _LauncherData.insert({ 9, txnew Struct::Launcher({ 9 }, {"测试小哥普攻2"}, {Enum::ePosition::TraceHost }, {2.85,1.75}, { true }, { true }, { 20 }, { 1 }, { 1000 }, {"xxxx"}, {11007})});
            _LauncherData.insert({ 10, txnew Struct::Launcher({ 10 }, {"测试小哥普攻3"}, {Enum::ePosition::TraceHost }, {2.85,1.75}, { true }, { true }, { 300 }, { 1 }, { 1000 }, {"xxxx"}, {11008})});
            _LauncherData.insert({ 11, txnew Struct::Launcher({ 11 }, {"地刺"}, {Enum::ePosition::WorldPosition }, {0,0}, { true }, { true }, { 10 }, { 1 }, { 1000 }, {"xxxx"}, {11009})});
            _LauncherData.insert({ 12, txnew Struct::Launcher({ 12 }, {"九头蛇柏扫人"}, {Enum::ePosition::TraceHost }, {0,0}, { true }, { true }, { 10 }, { 1 }, { 1000 }, {"xxxx"}, {11010})});
            _LauncherData.insert({ 13, txnew Struct::Launcher({ 13 }, {"九头蛇柏扫人"}, {Enum::ePosition::TraceHost }, {0,0}, { true }, { true }, { 10 }, { 1 }, { 1000 }, {"xxxx"}, {11011})});
            _LauncherData.insert({ 14, txnew Struct::Launcher({ 14 }, {"单纯抬头"}, {Enum::ePosition::TraceHost }, {0,0}, { true }, { true }, { 10 }, { 1 }, { 1000 }, {"xxxx"}, {11012})});
            _LauncherData.insert({ 15, txnew Struct::Launcher({ 15 }, {"蛇头打人"}, {Enum::ePosition::WorldPosition }, {0,1}, { true }, { true }, { 10 }, { 1 }, { 1000 }, {"xxxx"}, {11014})});
            _LauncherData.insert({ 16, txnew Struct::Launcher({ 16 }, {"藤曼抓人"}, {Enum::ePosition::WorldPosition }, {0,0}, { true }, { true }, { 10 }, { 1 }, { 1000 }, {"xxxx"}, {11013})});
            _LauncherData.insert({ 17, txnew Struct::Launcher({ 17 }, {"藤曼下垂"}, {Enum::ePosition::WorldPosition }, {0,0}, { true }, { true }, { 10 }, { 1 }, { 1000 }, {"xxxx"}, {11015})});
            _LauncherData.insert({ 18, txnew Struct::Launcher({ 18 }, {"九头蛇柏扫A"}, {Enum::ePosition::TraceHost }, {-4.5,0}, { true }, { true }, { 1050 }, { 1 }, { 1000 }, {"xxxx"}, {11016})});
            _LauncherData.insert({ 19, txnew Struct::Launcher({ 19 }, {"九头蛇柏扫B"}, {Enum::ePosition::TraceHost }, {-4.5,-2.5}, { true }, { true }, { 1180 }, { 1 }, { 1000 }, {"xxxx"}, {11010})});
            _LauncherData.insert({ 20, txnew Struct::Launcher({ 20 }, {"九头蛇柏扫人C"}, {Enum::ePosition::TraceHost }, {4.5,0}, { true }, { true }, { 1050 }, { 1 }, { 1000 }, {"xxxx"}, {11017})});
            _LauncherData.insert({ 21, txnew Struct::Launcher({ 21 }, {"九头蛇柏扫人D"}, {Enum::ePosition::TraceHost }, {4.5,-2.5}, { true }, { true }, { 1050 }, { 1 }, { 1000 }, {"xxxx"}, {11011})});
            _LauncherData.insert({ 22, txnew Struct::Launcher({ 22 }, {"尸鳖普攻"}, {Enum::ePosition::TraceHost }, {0.35,0.25}, { true }, { true }, { 225 }, { 1 }, { 600 }, {"xxxx"}, {11018})});
            _LauncherData.insert({ 23, txnew Struct::Launcher({ 23 }, {"血尸普攻"}, {Enum::ePosition::TraceHost }, {1.35,0.35}, { true }, { true }, { 1000 }, { 1 }, { 1000 }, {"xxxx"}, {11019})});
            _LauncherData.insert({ 24, txnew Struct::Launcher({ 24 }, {"潘子普攻"}, {Enum::ePosition::TraceHost }, {0.75,2.56}, { true }, { true }, { 600 }, { 1 }, { 500 }, {"xxxx"}, {11020})});
            _LauncherData.insert({ 25, txnew Struct::Launcher({ 25 }, {"潘子剑雨"}, {Enum::ePosition::WorldPosition }, {0,0}, { true }, { true }, { 1280 }, { 1 }, { 1000 }, {"xxxx"}, {11021})});
            _LauncherData.insert({ 26, txnew Struct::Launcher({ 26 }, {"爆裂箭"}, {Enum::ePosition::TraceHost }, {1.05,2.25}, { true }, { true }, { 950 }, { 10 }, { 50 }, {"xxxx"}, {11022,11027,11037})});
            _LauncherData.insert({ 27, txnew Struct::Launcher({ 27 }, {"加buff潘子"}, {Enum::ePosition::TraceHost }, {0,0}, { true }, { true }, { 10 }, { 1 }, { 100 }, {"xxxx"}, {11023})});
            _LauncherData.insert({ 28, txnew Struct::Launcher({ 28 }, {"潘子后撤普攻"}, {Enum::ePosition::TraceHost }, {0.4,1}, { true }, { true }, { 450 }, { 3 }, { 50 }, {"xxxx"}, {11020})});
            _LauncherData.insert({ 29, txnew Struct::Launcher({ 29 }, {"爆裂箭"}, {Enum::ePosition::TraceHost }, {0,2.25}, { true }, { true }, { 400 }, { 3 }, { 150 }, {"xxxx"}, {11024,11025,11026,11028,11029,11030,11042,11043,11044})});
            _LauncherData.insert({ 30, txnew Struct::Launcher({ 30 }, {"实际小哥声波攻击"}, {Enum::ePosition::TraceHost }, {0.85,0.15}, { true }, { true }, { 200 }, { 3 }, { 300 }, {"xxxx"}, {11032})});
            _LauncherData.insert({ 31, txnew Struct::Launcher({ 31 }, {"爆裂箭2"}, {Enum::ePosition::TraceHost }, {1.05,2.25}, { true }, { true }, { 1000 }, { 2 }, { 300 }, {"xxxx"}, {11033,11038})});
            _LauncherData.insert({ 32, txnew Struct::Launcher({ 32 }, {"爆裂箭3"}, {Enum::ePosition::TraceHost }, {1.05,2.25}, { true }, { true }, { 1050 }, { 2 }, { 300 }, {"xxxx"}, {11034,11039})});
            _LauncherData.insert({ 33, txnew Struct::Launcher({ 33 }, {"爆裂箭4"}, {Enum::ePosition::TraceHost }, {1.05,2.25}, { true }, { true }, { 1100 }, { 2 }, { 300 }, {"xxxx"}, {11035,11040})});
            _LauncherData.insert({ 34, txnew Struct::Launcher({ 34 }, {"爆裂箭5"}, {Enum::ePosition::TraceHost }, {1.05,2.25}, { true }, { true }, { 1150 }, { 2 }, { 300 }, {"xxxx"}, {11036,11041})});
            _MaterialData.insert({ 5001, txnew Struct::Material({ 5001 }, {}, {Enum::eTransactionType::None }, {Enum::eSoldType::None }, {"战国帛书拓本是进入鲁王公墓的特别物件"}, {Enum::eResources::Coin }, { 100 }, {"RewardItem/LootItem/LootItem/Res/IMG/TrueHead"}, {"晶石"}, { 1 }, {"RewardItem/LootItem/LootItem/Res/IMG/TrueHead"}, { 1001 })});
            _MonsterData.insert({ 4001, txnew Struct::Monster({ 4001 }, {"尸鳖"}, { 2 }, { 5 }, { 30 }, { 0.1 }, { 2 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 5 }, {4002}, { 10 }, {"Spine/Monster/Prefabs/ShiBie"}, { 18 }, { 6 }, { 35 }, { 2 }, { 10 }, { 16 }, { 0.15 }, { 400 }, { 10 }, { 0.1 }, { 0.1 }, { 91001 }, { 1 }, { 1 }, {"die"}, { 4012 }, { 4013 }, { 1.25 }, {Enum::eDamagePriority::Fourth }, { false }, { 0 }, { 0 }, { 11070 }, { 1018 }, { 21 }, { 0.25 }, { 0.85 }, { 1000 }, { 11070 }, { 450 }, { 500 })});
            _MonsterData.insert({ 4002, txnew Struct::Monster({ 4002 }, {"血尸"}, { 9 }, { 6.5 }, { 720 }, { 0.25 }, { 1.5 }, { 15 }, { 10 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 6.5 }, {4002}, { 50 }, {"Spine/Monster/Prefabs/XueShi"}, { 18 }, { 6 }, { 35 }, { 4 }, { 10 }, { 25 }, { 0.25 }, { 400 }, { 10 }, { 0.2 }, { 0.2 }, { 91002 }, { 1 }, { 1 }, {"die"}, { 4012 }, { 4013 }, { 1.75 }, {Enum::eDamagePriority::Fourth }, { false }, { 0 }, { 0 }, { 11071 }, { 1018 }, { 21 }, { 0.25 }, { 4 }, { 1000 }, { 11078 }, { 450 }, { 500 })});
            _MonsterData.insert({ 4003, txnew Struct::Monster({ 4003 }, {"藤曼"}, { 1 }, { 0 }, { 20 }, { 0 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 0 }, {4002}, { 10 }, {"Spine/Monster/Prefabs/NonePre"}, { 35 }, { 1 }, { 20 }, { 2 }, { 500 }, { 20 }, { 0.25 }, { 400 }, { 10 }, { 0.2 }, { 0.2 }, { 91002 }, { 1 }, { 1 }, {"die"}, { 4012 }, { 4013 }, { 1.5 }, {Enum::eDamagePriority::Fourth }, { true }, { 1200 }, { 6000 }, { 11001 }, { 1018 }, { 16 }, { 0.25 }, { 1 }, { 1000 }, { 0 }, { 450 }, { 500 })});
            _MonsterData.insert({ 4004, txnew Struct::Monster({ 4004 }, {"藤曼"}, { 1 }, { 0 }, { 20 }, { 0 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 0 }, {4002}, { 10 }, {"Spine/Monster/Prefabs/NonePre"}, { 35 }, { 1 }, { 20 }, { 2 }, { 500 }, { 20 }, { 0.25 }, { 400 }, { 10 }, { 0.2 }, { 0.2 }, { 91002 }, { 1 }, { 1 }, {"die"}, { 4012 }, { 4013 }, { 15 }, {Enum::eDamagePriority::Fourth }, { true }, { 1200 }, { 6000 }, { 11001 }, { 1018 }, { 16 }, { 0.25 }, { 1 }, { 1000 }, { 0 }, { 450 }, { 500 })});
            _MonsterBossData.insert({ 4001, txnew Struct::MonsterBoss({ 4001 }, {"九头蛇柏"}, { 6 }, { 0 }, { 5427 }, { 0.08 }, { 1.85 }, { 20 }, { 25 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 1 }, { 3 }, {4002}, { 10 }, {"Spine/Monster/Prefabs/JiuTouSheBai"}, { 30 }, { 7 }, { 0 }, { 11002 }, { 11003 }, { 11004 }, { 11005 }, { 11006 }, { 11007 }, { 15 }, { 0.001 }, { 200 }, { 12000 }, { 0.6 }, { 0.2 }, {11002,11003,11004,11005,11006,11007,11008,11009,11010,11011,11012,11013,11014,11015,11016,11017,11018,11019,11020,11021,11022,11023,11024,11025,11026,11027,11028,11029,11030,11031,11032,11033,11034,11035,11036,11037,11038,11039,11040,11041,11042,11043,11044,11045,11046,11047,11048,11049,11050,11051,11052,11053,11054,11055,11056,11057,11058,11059,11060,11061,11062,11063,11064,11065,11067,11068,11069}, {11002,11003,11004,11005,11006,11007,11008,11009,11010,11011,11012,11013,11014,11015,11016,11017,11018,11019,11020,11021,11022,11023,11024,11025,11026,11027,11028,11029,11030,11031,11032,11033,11034,11035,11036,11037,11038,11039,11040,11041,11042,11043,11044,11045,11046,11047,11048,11049,11050,11051,11052,11053,11054,11055,11056,11057,11058,11059,11060,11061,11062,11063,11064,11065,11067,11068,11069}, {11002,11003,11004,11005,11006,11007,11008,11009,11010,11011,11012,11013,11014,11015,11016,11017,11018,11019,11020,11021,11022,11023,11024,11025,11026,11027,11028,11029,11030,11031,11032,11033,11034,11035,11036,11037,11038,11039,11040,11041,11042,11043,11044,11045,11046,11047,11048,11049,11050,11051,11052,11053,11054,11055,11056,11057,11058,11059,11060,11061,11062,11063,11064,11065,11067,11068,11069}, { 0.1 }, { 0.1 }, { 91001 }, { 1 }, { 1 }, {"die"}, { 4012 }, { 4013 }, { 2.65 }, {Enum::eDamagePriority::First }, {4003}, {Enum::eBoss::HydraCypress }, {"Snake_Idle"}, {"UI/Sprites/TopUI/MonsterIcon/Boss_1"}, { 1019 }, { 30 }, { 3 }, {"SnakeA", "SnakeB", "SnakeC", "SnakeD", "Tree"}, {"Snake_Die-A", "Snake_Die-B", "Snake_Die-C", "Snake_Die-D", "Snake_Tree_Die"}, { 5200 }, { 450 }, { 500 })});
            _MonsterQualityData.insert({ 91001, txnew Struct::MonsterQuality({ 91001 }, {"普通"}, {0.87,1,0.8}, {Enum::eDamagePriority::Third })});
            _MonsterQualityData.insert({ 91002, txnew Struct::MonsterQuality({ 91002 }, {"精英"}, {0.35,0.92,1}, {Enum::eDamagePriority::Third })});
            _MonsterQualityData.insert({ 91003, txnew Struct::MonsterQuality({ 91003 }, {"Boss"}, {0,0,2}, {Enum::eDamagePriority::First })});
            _MonsterSkillData.insert({ 11001, txnew Struct::MonsterSkill({ 11001 }, {"九头蛇柏的触手技能抓人"}, {"抓人"}, {Enum::eSkillEffect::Launcher}, {16}, { 5000 }, { 12000 }, { true }, { true }, { 15 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1500 }, {}, { 4016 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 600 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11002, txnew Struct::MonsterSkill({ 11002 }, {"九头蛇柏发动技能A扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {18}, { 8500 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_A"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-A"}, { 1 }, { 1000 }, { true }, {"SnakeA"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11003, txnew Struct::MonsterSkill({ 11003 }, {"九头蛇柏发动技能B扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {19}, { 12000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_B"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-B"}, { 1 }, { 602 }, { true }, {"SnakeB"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11004, txnew Struct::MonsterSkill({ 11004 }, {"九头蛇柏发动技能C扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {20}, { 15500 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_C"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-C"}, { 1 }, { 603 }, { true }, {"SnakeC"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11005, txnew Struct::MonsterSkill({ 11005 }, {"九头蛇柏发动技能D扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {21}, { 19000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_D"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-D"}, { 1 }, { 604 }, { true }, {"SnakeD"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11006, txnew Struct::MonsterSkill({ 11006 }, {"九头蛇柏发动技能C扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {20}, { 22500 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_C"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {5,7.5}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-C"}, { 1 }, { 605 }, { true }, {"SnakeC"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,7.5}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11007, txnew Struct::MonsterSkill({ 11007 }, {"九头蛇柏发动技能D扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {21}, { 26000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_D"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-D"}, { 1 }, { 606 }, { true }, {"SnakeD"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11008, txnew Struct::MonsterSkill({ 11008 }, {"插入地底下a"}, {"插入地底"}, {Enum::eSkillEffect::Launcher}, {17}, { 29000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Insert-A"}, { 4019 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {"Snake_Idle-A"}, { 1 }, { 900 }, { true }, {"SnakeA"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11009, txnew Struct::MonsterSkill({ 11009 }, {"插入地底下b"}, {"插入地底"}, {Enum::eSkillEffect::Launcher}, {17}, { 29000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Insert-B"}, { 4019 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {"Snake_Idle-B"}, { 1 }, { 2500 }, { true }, {"SnakeB"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11010, txnew Struct::MonsterSkill({ 11010 }, {"插入地底下c"}, {"插入地底"}, {Enum::eSkillEffect::Launcher}, {17}, { 38000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Insert-C"}, { 4019 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {"Snake_Idle-C"}, { 1 }, { 2500 }, { true }, {"SnakeC"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11011, txnew Struct::MonsterSkill({ 11011 }, {"插入地底下d"}, {"插入地底"}, {Enum::eSkillEffect::Launcher}, {17}, { 38000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Insert-D"}, { 4019 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {"Snake_Idle-D"}, { 1 }, { 2500 }, { true }, {"SnakeD"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11012, txnew Struct::MonsterSkill({ 11012 }, {"地刺1"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 31000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11013, txnew Struct::MonsterSkill({ 11013 }, {"地刺2"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 32000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11014, txnew Struct::MonsterSkill({ 11014 }, {"地刺3"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 33000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11015, txnew Struct::MonsterSkill({ 11015 }, {"地刺4"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 34000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11016, txnew Struct::MonsterSkill({ 11016 }, {"地刺5"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 35000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11017, txnew Struct::MonsterSkill({ 11017 }, {"地刺6"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 36000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11018, txnew Struct::MonsterSkill({ 11018 }, {"地刺7"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 37000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11019, txnew Struct::MonsterSkill({ 11019 }, {"地刺8"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 40000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11020, txnew Struct::MonsterSkill({ 11020 }, {"地刺9"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 41000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11021, txnew Struct::MonsterSkill({ 11021 }, {"地刺10"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 42000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11022, txnew Struct::MonsterSkill({ 11022 }, {"地刺11"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 43500 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11023, txnew Struct::MonsterSkill({ 11023 }, {"地刺12"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 45000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11024, txnew Struct::MonsterSkill({ 11024 }, {"九头蛇柏发动技能A扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {18}, { 48000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_A"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-A"}, { 1 }, { 1000 }, { true }, {"SnakeA"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11025, txnew Struct::MonsterSkill({ 11025 }, {"九头蛇柏发动技能B扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {19}, { 51000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_B"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-B"}, { 1 }, { 602 }, { true }, {"SnakeB"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11026, txnew Struct::MonsterSkill({ 11026 }, {"九头蛇柏发动技能C扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {20}, { 54000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_C"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {1}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-C"}, { 1 }, { 603 }, { true }, {"SnakeC"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {1}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11027, txnew Struct::MonsterSkill({ 11027 }, {"九头蛇柏发动技能D扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {21}, { 57000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_D"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {1,7.5}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-D"}, { 1 }, { 604 }, { true }, {"SnakeD"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {1,7.5}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11028, txnew Struct::MonsterSkill({ 11028 }, {"九头蛇柏发动技能B扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {19}, { 60000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_B"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-B"}, { 1 }, { 602 }, { true }, {"SnakeB"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11029, txnew Struct::MonsterSkill({ 11029 }, {"九头蛇柏发动技能D扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {21}, { 63000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_D"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-D"}, { 1 }, { 606 }, { true }, {"SnakeD"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11030, txnew Struct::MonsterSkill({ 11030 }, {"A抬头动作"}, {"抬头动作1"}, {Enum::eSkillEffect::Launcher}, {14}, { 67000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Fly-A"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { true }, {"Snake_Fly-idle-A"}, {"Snake_Fly-A"}, {"Snake_Fly-back-A"}, { 13000 }, { 1 }, {"Snake_Idle-A"}, { 1 }, { 5000 }, { true }, {"SnakeA"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11031, txnew Struct::MonsterSkill({ 11031 }, {"A抬头和伸头打人"}, {"伸头打人"}, {Enum::eSkillEffect::Launcher}, {15}, { 70500 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 2000 }, {}, { 4015 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::Circle }, {2.45,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {}, { 1 }, { 700 }, { false }, {"xx"}, { true }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11032, txnew Struct::MonsterSkill({ 11032 }, {"B抬头动作"}, {"抬头动作1"}, {Enum::eSkillEffect::Launcher}, {14}, { 67000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Fly-B"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { true }, {"Snake_Fly-idle-B"}, {"Snake_Fly-B"}, {"Snake_Fly-back-B"}, { 13000 }, { 1 }, {"Snake_Idle-B"}, { 1 }, { 5000 }, { true }, {"SnakeB"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11033, txnew Struct::MonsterSkill({ 11033 }, {"B抬头和伸头打人"}, {"伸头打人"}, {Enum::eSkillEffect::Launcher}, {15}, { 73500 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 2000 }, {}, { 4015 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::Circle }, {2.45,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {}, { 1 }, { 700 }, { false }, {"xx"}, { true }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11034, txnew Struct::MonsterSkill({ 11034 }, {"C抬头动作"}, {"抬头动作1"}, {Enum::eSkillEffect::Launcher}, {14}, { 67000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Fly-C"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { true }, {"Snake_Fly-idle-C"}, {"Snake_Fly-C"}, {"Snake_Fly-back-C"}, { 13000 }, { 1 }, {"Snake_Idle-C"}, { 1 }, { 5000 }, { true }, {"SnakeC"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11035, txnew Struct::MonsterSkill({ 11035 }, {"C抬头和伸头打人"}, {"伸头打人"}, {Enum::eSkillEffect::Launcher}, {15}, { 76500 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 2000 }, {}, { 4015 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::Circle }, {2.45,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {}, { 1 }, { 700 }, { false }, {"xx"}, { true }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11036, txnew Struct::MonsterSkill({ 11036 }, {"D抬头动作"}, {"抬头动作1"}, {Enum::eSkillEffect::Launcher}, {14}, { 67000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Fly-D"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { true }, {"Snake_Fly-idle-D"}, {"Snake_Fly-D"}, {"Snake_Fly-back-D"}, { 13000 }, { 1 }, {"Snake_Idle-D"}, { 1 }, { 5000 }, { true }, {"SnakeD"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11037, txnew Struct::MonsterSkill({ 11037 }, {"D抬头和伸头打人"}, {"伸头打人"}, {Enum::eSkillEffect::Launcher}, {15}, { 79000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 2000 }, {}, { 4015 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::Circle }, {2.45,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {}, { 1 }, { 700 }, { false }, {"xx"}, { true }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11038, txnew Struct::MonsterSkill({ 11038 }, {"九头蛇柏发动技能A扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {18}, { 81500 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_A"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-A"}, { 1 }, { 1000 }, { true }, {"SnakeA"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11039, txnew Struct::MonsterSkill({ 11039 }, {"九头蛇柏发动技能B扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {19}, { 83750 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_B"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-B"}, { 1 }, { 602 }, { true }, {"SnakeB"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11040, txnew Struct::MonsterSkill({ 11040 }, {"九头蛇柏发动技能C扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {20}, { 86000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_C"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {1}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-C"}, { 1 }, { 603 }, { true }, {"SnakeC"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11041, txnew Struct::MonsterSkill({ 11041 }, {"九头蛇柏发动技能D扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {21}, { 88500 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_D"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {1,7.5}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-D"}, { 1 }, { 604 }, { true }, {"SnakeD"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {1,7.5}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11042, txnew Struct::MonsterSkill({ 11042 }, {"插入地底下a"}, {"插入地底"}, {Enum::eSkillEffect::Launcher}, {17}, { 90000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Insert-A"}, { 4019 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {"Snake_Idle-A"}, { 1 }, { 900 }, { true }, {"SnakeA"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11043, txnew Struct::MonsterSkill({ 11043 }, {"插入地底下b"}, {"插入地底"}, {Enum::eSkillEffect::Launcher}, {17}, { 90000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Insert-B"}, { 4019 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {"Snake_Idle-B"}, { 1 }, { 2500 }, { true }, {"SnakeB"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11044, txnew Struct::MonsterSkill({ 11044 }, {"插入地底下c"}, {"插入地底"}, {Enum::eSkillEffect::Launcher}, {17}, { 100000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Insert-C"}, { 4019 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {"Snake_Idle-C"}, { 1 }, { 2500 }, { true }, {"SnakeC"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11045, txnew Struct::MonsterSkill({ 11045 }, {"插入地底下d"}, {"插入地底"}, {Enum::eSkillEffect::Launcher}, {17}, { 100000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Insert-D"}, { 4019 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {"Snake_Idle-D"}, { 1 }, { 2500 }, { true }, {"SnakeD"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11046, txnew Struct::MonsterSkill({ 11046 }, {"地刺1"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 92500 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11047, txnew Struct::MonsterSkill({ 11047 }, {"地刺2"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 95000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11048, txnew Struct::MonsterSkill({ 11048 }, {"地刺3"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 97500 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11049, txnew Struct::MonsterSkill({ 11049 }, {"地刺4"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 102500 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11050, txnew Struct::MonsterSkill({ 11050 }, {"地刺5"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 105000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11051, txnew Struct::MonsterSkill({ 11051 }, {"地刺6"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 107500 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11052, txnew Struct::MonsterSkill({ 11052 }, {"九头蛇柏发动技能A扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {18}, { 110000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_A"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-A"}, { 1 }, { 1000 }, { true }, {"SnakeA"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11053, txnew Struct::MonsterSkill({ 11053 }, {"九头蛇柏发动技能C扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {20}, { 110000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_C"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {1}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-C"}, { 1 }, { 603 }, { true }, {"SnakeC"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11054, txnew Struct::MonsterSkill({ 11054 }, {"九头蛇柏发动技能B扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {19}, { 113500 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_B"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-B"}, { 1 }, { 602 }, { true }, {"SnakeB"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11055, txnew Struct::MonsterSkill({ 11055 }, {"九头蛇柏发动技能D扫人"}, {"扫人"}, {Enum::eSkillEffect::Launcher}, {21}, { 113500 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Atk_D"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {1,7.5}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 2 }, {"Snake_Idle-D"}, { 1 }, { 604 }, { true }, {"SnakeD"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {1,7.5}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11056, txnew Struct::MonsterSkill({ 11056 }, {"A抬头动作（组合技能开始)"}, {"抬头动作1"}, {Enum::eSkillEffect::Launcher}, {14}, { 115500 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Fly-A"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { true }, {"Snake_Fly-idle-A"}, {"Snake_Fly-A"}, {"Snake_Fly-back-A"}, { 13000 }, { 1 }, {"Snake_Idle-A"}, { 1 }, { 5000 }, { true }, {"SnakeA"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11057, txnew Struct::MonsterSkill({ 11057 }, {"C抬头动作"}, {"抬头动作1"}, {Enum::eSkillEffect::Launcher}, {14}, { 115500 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Fly-C"}, { 4014 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { true }, {"Snake_Fly-idle-C"}, {"Snake_Fly-C"}, {"Snake_Fly-back-C"}, { 13000 }, { 1 }, {"Snake_Idle-C"}, { 1 }, { 5000 }, { true }, {"SnakeC"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11058, txnew Struct::MonsterSkill({ 11058 }, {"A抬头和伸头打人"}, {"伸头打人"}, {Enum::eSkillEffect::Launcher}, {15}, { 118000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 2000 }, {}, { 4015 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::Circle }, {2.45,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {}, { 1 }, { 700 }, { false }, {"xx"}, { true }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11059, txnew Struct::MonsterSkill({ 11059 }, {"A抬头和伸头打人"}, {"伸头打人"}, {Enum::eSkillEffect::Launcher}, {15}, { 121000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 2000 }, {}, { 4015 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::Circle }, {2.45,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {}, { 1 }, { 700 }, { false }, {"xx"}, { true }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11060, txnew Struct::MonsterSkill({ 11060 }, {"A抬头和伸头打人"}, {"伸头打人"}, {Enum::eSkillEffect::Launcher}, {15}, { 124000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 2000 }, {}, { 4015 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::Circle }, {2.45,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {}, { 1 }, { 700 }, { false }, {"xx"}, { true }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11061, txnew Struct::MonsterSkill({ 11061 }, {"A抬头和伸头打人"}, {"伸头打人"}, {Enum::eSkillEffect::Launcher}, {15}, { 127000 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 2000 }, {}, { 4015 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::Circle }, {2.45,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {}, { 1 }, { 700 }, { false }, {"xx"}, { true }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11062, txnew Struct::MonsterSkill({ 11062 }, {"插入地底下b"}, {"插入地底"}, {Enum::eSkillEffect::Launcher}, {17}, { 120000 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Insert-B"}, { 4019 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {"Snake_Idle-B"}, { 1 }, { 2500 }, { true }, {"SnakeB"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11063, txnew Struct::MonsterSkill({ 11063 }, {"插入地底下d"}, {"插入地底"}, {Enum::eSkillEffect::Launcher}, {17}, { 120500 }, { 130000 }, { false }, { false }, { 30 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Snake_Insert-D"}, { 4019 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {4,12}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 0 }, {"Snake_Idle-D"}, { 1 }, { 2500 }, { true }, {"SnakeD"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {4,12}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11064, txnew Struct::MonsterSkill({ 11064 }, {"地刺1"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 121500 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11065, txnew Struct::MonsterSkill({ 11065 }, {"地刺2"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 122700 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11066, txnew Struct::MonsterSkill({ 11066 }, {"地刺3"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 123900 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11067, txnew Struct::MonsterSkill({ 11067 }, {"地刺4"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 124700 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11068, txnew Struct::MonsterSkill({ 11068 }, {"地刺5"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 125900 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11069, txnew Struct::MonsterSkill({ 11069 }, {"地刺6（组合结束）"}, {"地刺"}, {Enum::eSkillEffect::Launcher}, {11}, { 127500 }, { 130000 }, { true }, { false }, { 30 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {}, { 4017 }, { 0 }, { 20 }, { false }, { 1001 }, { false }, {Enum::eSheetsType::Circle }, {1.35,9.6}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {}, { 1 }, { 607 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,9.6}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11070, txnew Struct::MonsterSkill({ 11070 }, {"尸鳖打人"}, {"尸鳖打人"}, {Enum::eSkillEffect::Launcher}, {22}, { 750 }, { 750 }, { false }, { false }, { 3 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"attack"}, { 4004 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 1 }, {"idle"}, { 1 }, { 800 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11071, txnew Struct::MonsterSkill({ 11071 }, {"血尸普攻1"}, {"尸鳖打人"}, {Enum::eSkillEffect::Launcher}, {23}, { 750 }, { 2000 }, { false }, { false }, { 6 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 1000 }, {"Xueshi_atk"}, { 4004 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 1 }, {"idle"}, { 1 }, { 1500 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {0.41,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11072, txnew Struct::MonsterSkill({ 11072 }, {"血尸普攻2"}, {"尸鳖打人"}, {Enum::eSkillEffect::Launcher}, {23}, { 2500 }, { 2000 }, { false }, { false }, { 6 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Xueshi_atk"}, { 4004 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 1 }, {"idle"}, { 1 }, { 1500 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11073, txnew Struct::MonsterSkill({ 11073 }, {"血尸普攻3"}, {"尸鳖打人"}, {Enum::eSkillEffect::Launcher}, {23}, { 4500 }, { 2000 }, { false }, { false }, { 6 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Xueshi_atk"}, { 4004 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 1 }, {"idle"}, { 1 }, { 1500 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11074, txnew Struct::MonsterSkill({ 11074 }, {"血尸普攻4"}, {"尸鳖打人"}, {Enum::eSkillEffect::Launcher}, {23}, { 6500 }, { 2000 }, { false }, { false }, { 6 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Xueshi_atk"}, { 4004 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 1 }, {"idle"}, { 1 }, { 1500 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11075, txnew Struct::MonsterSkill({ 11075 }, {"血尸普攻5"}, {"尸鳖打人"}, {Enum::eSkillEffect::Launcher}, {23}, { 8500 }, { 2000 }, { false }, { false }, { 6 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Xueshi_atk"}, { 4004 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 1 }, {"idle"}, { 1 }, { 1500 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11076, txnew Struct::MonsterSkill({ 11076 }, {"血尸普攻6"}, {"尸鳖打人"}, {Enum::eSkillEffect::Launcher}, {23}, { 10500 }, { 2000 }, { false }, { false }, { 6 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Xueshi_atk"}, { 4004 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 1 }, {"idle"}, { 1 }, { 1500 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11077, txnew Struct::MonsterSkill({ 11077 }, {"血尸普攻7"}, {"尸鳖打人"}, {Enum::eSkillEffect::Launcher}, {23}, { 12500 }, { 2000 }, { false }, { false }, { 6 }, { false }, {"UI/Sprites/Skill/SkillRange"}, { 0 }, {"Xueshi_atk"}, { 4004 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {5,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { true }, { false }, {}, {}, {}, { 0 }, { 1 }, {"idle"}, { 1 }, { 1500 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterSkillData.insert({ 11078, txnew Struct::MonsterSkill({ 11078 }, {"血尸位移级技能"}, {"尸鳖打人"}, {Enum::eSkillEffect::Move}, {1006}, { 500 }, { 2000 }, { false }, { false }, { 6 }, { true }, {"UI/Sprites/Skill/SkillRange"}, { 1700 }, {"Xueshi_Rush"}, { 4004 }, { 0 }, { 20 }, { false }, { 1001 }, { true }, {Enum::eSheetsType::None }, {1,30}, {"Material_SpeTexture/Material/Res/RingIn"}, { false }, { false }, {}, {}, {}, { 0 }, { 1 }, {"idle"}, { 1 }, { 4000 }, { false }, {"xx"}, { false }, { 600 }, { 0.85 }, { 120 }, { 120 }, { 50 }, { true }, {5,30}, { false }, { 350 })});
            _MonsterkillData.insert({ 61001, txnew Struct::Monsterkill({ 61001 }, { 4001 }, { 10 })});
            _MonsterkillData.insert({ 61002, txnew Struct::Monsterkill({ 61002 }, { 4001 }, { 20 })});
            _MoveData.insert({ 1001, txnew Struct::Move({ 1001 }, {"小哥跟踪位移位移"}, {Enum::eMoveType::TrackMove }, { 2 }, { 1 }, { 5000 }, { false }, { 10 }, { 1 }, {1005}, {Enum::ePhysicType::Wall}, {0,0}, { 1000 }, {Enum::eDamageType::Physic }, { 1 }, { 100 }, { 100 }, {Enum::eTargetType::EnemyCharacter }, { true }, { 50 }, { 10 }, { 1012 }, { 1010 }, { 0 })});
            _MoveData.insert({ 1002, txnew Struct::Move({ 1002 }, {"直线位移"}, {Enum::eMoveType::LineMove }, { 22 }, { 1 }, { 565 }, { false }, { 100 }, { 1 }, {1005}, {Enum::ePhysicType::Wall}, {0,0}, { 10 }, {Enum::eDamageType::Physic }, { 1 }, { 100 }, { 10 }, {Enum::eTargetType::EnemyCharacter }, { true }, { 50 }, { 10 }, { 0 }, { 0 }, { 0 })});
            _MoveData.insert({ 1003, txnew Struct::Move({ 1003 }, {"瞬间移动"}, {Enum::eMoveType::Immediate }, { 0 }, { 0 }, { 3000 }, { false }, { 100 }, { 120 }, {1005}, {Enum::ePhysicType::None}, {0,0}, { 20 }, {Enum::eDamageType::Physic }, { 1 }, { 100 }, { 100 }, {Enum::eTargetType::EnemyCharacter }, { true }, { 50 }, { 10 }, { 1013 }, { 1011 }, { 0 })});
            _MoveData.insert({ 1004, txnew Struct::Move({ 1004 }, {"抛物线位移"}, {Enum::eMoveType::ParabolaMove }, { 0 }, { 0 }, { 3000 }, { false }, { 100 }, { 120 }, {1005}, {Enum::ePhysicType::None}, {0,0}, { 20 }, {Enum::eDamageType::Physic }, { 1 }, { 100 }, { 100 }, {Enum::eTargetType::EnemyCharacter }, { true }, { 50 }, { 10 }, { 1013 }, { 1011 }, { 1.5 })});
            _MoveData.insert({ 1005, txnew Struct::Move({ 1005 }, {"藤曼自由选择目的地直线位移"}, {Enum::eMoveType::EverLineMove }, { 15 }, { 1 }, { 0 }, { false }, { 100 }, { 1 }, {1005}, {Enum::ePhysicType::Wall}, {0,0}, { 10 }, {Enum::eDamageType::Physic }, { 1 }, { 100 }, { 10 }, {Enum::eTargetType::EnemyCharacter }, { true }, { 50 }, { 10 }, { 0 }, { 0 }, { 0 })});
            _MoveData.insert({ 1006, txnew Struct::Move({ 1006 }, {"血尸位移打人"}, {Enum::eMoveType::LineMove }, { 12 }, { 0.5 }, { 1600 }, { true }, { 5 }, { 2 }, {1001}, {Enum::ePhysicType::Wall}, {0,0}, { 2550 }, {Enum::eDamageType::Physic }, { 0 }, { 600 }, { 10 }, {Enum::eTargetType::EnemyCharacter }, { false }, { 50 }, { 10 }, { 0 }, { 0 }, { 0 })});
            _MoveData.insert({ 1007, txnew Struct::Move({ 1007 }, {"潘子向后位移"}, {Enum::eMoveType::LineMove }, { -10 }, { 0 }, { 475 }, { false }, { 100 }, { 1 }, {1005}, {Enum::ePhysicType::Wall}, {0,0}, { 285 }, {Enum::eDamageType::Physic }, { 1 }, { 100 }, { 10 }, {Enum::eTargetType::EnemyCharacter }, { true }, { 50 }, { 10 }, { 0 }, { 0 }, { 0 })});
            _NonePerfomData.insert({ 1001, txnew Struct::NonePerfom({ 1001 }, {"小哥被动加buff技能"}, {1017,1022})});
            _NpcData.insert({ 1001, txnew Struct::Npc({ 1001 }, {"王胖子"}, {Enum::eNpcType::Normal }, {11002}, {"xx"}, {"Spine/Character/Prefabs/TestCharacter"}, {-12,15.2,0}, {10001,10002}, { -1 }, { -1 }, {"xxx"})});
            _NpcData.insert({ 1002, txnew Struct::Npc({ 1002 }, {"王涛"}, {Enum::eNpcType::Normal }, {11001}, {"xx"}, {"Spine/Character/Prefabs/TestCharacter"}, {-12,53.71,1}, {10001,10002}, { 31001 }, { -1 }, {"xxx"})});
            _NpcData.insert({ 1003, txnew Struct::Npc({ 1003 }, {"孙悟空1"}, {Enum::eNpcType::Normal }, {11001}, {"xx"}, {"Spine/Character/Prefabs/TestCharacter"}, {29.37,25.29,1}, {10001,10002}, { 31002 }, { 10001 }, {"xxx"})});
            _NpcData.insert({ 1004, txnew Struct::Npc({ 1004 }, {"王胖子1"}, {Enum::eNpcType::Normal }, {11001}, {"xx"}, {"Spine/Character/Prefabs/TestCharacter"}, {-107.1,37.6,0}, {10001,10002}, { -1 }, { -1 }, {"xxx"})});
            _NpcData.insert({ 1005, txnew Struct::Npc({ 1005 }, {"王涛1"}, {Enum::eNpcType::Normal }, {11001}, {"xx"}, {"Spine/Character/Prefabs/TestCharacter"}, {-181.71,44.8,1}, {10001,10002}, { 31001 }, { -1 }, {"xxx"})});
            _NpcData.insert({ 1006, txnew Struct::Npc({ 1006 }, {"孙悟空2"}, {Enum::eNpcType::Normal }, {11001}, {"xx"}, {"Spine/Character/Prefabs/TestCharacter"}, {-131.2,52.5,1}, {10001,10002}, { 31002 }, { 10001 }, {"xxx"})});
            _NpcData.insert({ 1007, txnew Struct::Npc({ 1007 }, {"王胖子3"}, {Enum::eNpcType::Normal }, {11001}, {"xx"}, {"Spine/Character/Prefabs/TestCharacter"}, {-109.5,73.2,0}, {10001,10002}, { -1 }, { -1 }, {"xxx"})});
            _NpcData.insert({ 1008, txnew Struct::Npc({ 1008 }, {"王涛4"}, {Enum::eNpcType::Normal }, {11001}, {"xx"}, {"Spine/Character/Prefabs/TestCharacter"}, {-65.01,79.25,1}, {10001,10002}, { 31001 }, { -1 }, {"xxx"})});
            _NpcData.insert({ 1009, txnew Struct::Npc({ 1009 }, {"孙悟空5"}, {Enum::eNpcType::Normal }, {11001}, {"xx"}, {"Spine/Character/Prefabs/TestCharacter"}, {-58.2,109,1}, {10001,10002}, { 31002 }, { 10001 }, {"xxx"})});
            _NpcData.insert({ 1010, txnew Struct::Npc({ 1010 }, {"孙悟空6"}, {Enum::eNpcType::Normal }, {11001}, {"xx"}, {"Spine/Character/Prefabs/TestCharacter"}, {-24.3,92.91,1}, {10001,10002}, { 31002 }, { 10001 }, {"xxx"})});
            _NpcPositionChangeData.insert({ 10001, txnew Struct::NpcPositionChange({ 10001 }, { 31001 }, {1,1,1})});
            _NpcPositionChangeData.insert({ 10002, txnew Struct::NpcPositionChange({ 10002 }, { 31002 }, {1,1,2})});
            _NpcTalkData.insert({ 11001, txnew Struct::NpcTalk({ 11001 }, {41002}, { 4001 })});
            _NpcTalkData.insert({ 11002, txnew Struct::NpcTalk({ 11002 }, {41001}, { 4002 })});
            _OrganTypeData.insert({ 4001, txnew Struct::OrganType({ 4001 }, {"碎石机关"}, {1005}, {Enum::eSkillType::Launch }, { 21001 }, {44,100}, {"xxx"}, {Enum::eTriggleType::Near }, { true }, { 100 })});
            _PrerequisiteData.insert({ 6001, txnew Struct::Prerequisite({ 6001 }, { 0 }, { 4004 }, {0})});
            _PrerequisiteData.insert({ 6002, txnew Struct::Prerequisite({ 6002 }, { 0 }, { 4005 }, {0})});
            _QualityData.insert({ 1001, txnew Struct::Quality({ 1001 }, {"杀死xxx野怪"}, {1,1,1}, {"xxx"})});
            _RefreshRuleData.insert({ 5001, txnew Struct::RefreshRule({ 5001 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 1 }, { 5000 }, { 1000 }, {-14,14}, { 3 })});
            _RefreshRuleData.insert({ 5002, txnew Struct::RefreshRule({ 5002 }, {Enum::eRefreshThing::OrganType }, { 21001 }, { 6 }, { 5000 }, { 1000 }, {2,10}, { 5 })});
            _RefreshRuleData.insert({ 5003, txnew Struct::RefreshRule({ 5003 }, {Enum::eRefreshThing::Monster }, { 4002 }, { 5 }, { 5000 }, { 1000 }, {1.42,1.6}, { 3 })});
            _RefreshRuleData.insert({ 5004, txnew Struct::RefreshRule({ 5004 }, {Enum::eRefreshThing::Monster }, { 4002 }, { 5 }, { 5000 }, { 1000 }, {12.2,15.6}, { 4 })});
            _RefreshRuleData.insert({ 5005, txnew Struct::RefreshRule({ 5005 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 6 }, { 5000 }, { 1000 }, {-1.4,21.33}, { 4 })});
            _RefreshRuleData.insert({ 5006, txnew Struct::RefreshRule({ 5006 }, {Enum::eRefreshThing::Monster }, { 4002 }, { 7 }, { 5000 }, { 1000 }, {-8.8,17.6}, { 4 })});
            _RefreshRuleData.insert({ 5007, txnew Struct::RefreshRule({ 5007 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 7 }, { 5000 }, { 1000 }, {20.11,12.78}, { 4 })});
            _RefreshRuleData.insert({ 5008, txnew Struct::RefreshRule({ 5008 }, {Enum::eRefreshThing::MonsterBoss }, { 4001 }, { 1 }, { 2 }, { 5000 }, {-44.78,49.56}, { 1 })});
            _RefreshRuleData.insert({ 5009, txnew Struct::RefreshRule({ 5009 }, {Enum::eRefreshThing::Monster }, { 4003 }, { 1 }, { 15 }, { 1000 }, {-45.18,40.56}, { 1 })});
            _RefreshRuleData.insert({ 5010, txnew Struct::RefreshRule({ 5010 }, {Enum::eRefreshThing::Monster }, { 4003 }, { 1 }, { 5 }, { 1000 }, {-43.88,42.56}, { 1 })});
            _RefreshRuleData.insert({ 5011, txnew Struct::RefreshRule({ 5011 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 1 }, { 15 }, { 1000 }, {-48.5,38.2}, { 1 })});
            _RefreshRuleData.insert({ 5012, txnew Struct::RefreshRule({ 5012 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 1 }, { 15 }, { 1000 }, {0.56,9.95}, { 1 })});
            _RefreshRuleData.insert({ 5013, txnew Struct::RefreshRule({ 5013 }, {Enum::eRefreshThing::Monster }, { 4002 }, { 1 }, { 1 }, { 1000 }, {0.56,9.95}, { 1 })});
            _RefreshRuleData.insert({ 5014, txnew Struct::RefreshRule({ 5014 }, {Enum::eRefreshThing::Monster }, { 4003 }, { 1 }, { 1 }, { 1000 }, {1.26,68}, { 2 })});
            _RefreshRuleData.insert({ 5015, txnew Struct::RefreshRule({ 5015 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 2 }, { 3 }, { 10 }, {1.26,68}, { 3 })});
            _RefreshRuleData.insert({ 5016, txnew Struct::RefreshRule({ 5016 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 4 }, { 5 }, { 10 }, {19.58,57.42}, { 3 })});
            _RefreshRuleData.insert({ 5017, txnew Struct::RefreshRule({ 5017 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 2 }, { 3 }, { 10 }, {9,44.6}, { 3 })});
            _RefreshRuleData.insert({ 5018, txnew Struct::RefreshRule({ 5018 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 2 }, { 3 }, { 10 }, {10.4,10.9}, { 2 })});
            _RefreshRuleData.insert({ 5019, txnew Struct::RefreshRule({ 5019 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 4 }, { 5 }, { 10 }, {56.31,41}, { 2 })});
            _RefreshRuleData.insert({ 5020, txnew Struct::RefreshRule({ 5020 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 4 }, { 5 }, { 10 }, {74,31}, { 2 })});
            _RefreshRuleData.insert({ 5021, txnew Struct::RefreshRule({ 5021 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 4 }, { 6 }, { 10 }, {38,8.9}, { 2 })});
            _RefreshRuleData.insert({ 5022, txnew Struct::RefreshRule({ 5022 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 4 }, { 6 }, { 10 }, {33.52,3.19}, { 2 })});
            _RefreshRuleData.insert({ 5023, txnew Struct::RefreshRule({ 5023 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 3 }, { 4 }, { 10 }, {-86.12,8.86}, { 2 })});
            _RefreshRuleData.insert({ 5024, txnew Struct::RefreshRule({ 5024 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 3 }, { 4 }, { 10 }, {-84,30}, { 2 })});
            _RefreshRuleData.insert({ 5025, txnew Struct::RefreshRule({ 5025 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 3 }, { 4 }, { 10 }, {-84,30}, { 2 })});
            _RefreshRuleData.insert({ 5026, txnew Struct::RefreshRule({ 5026 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 4 }, { 12 }, { 1000 }, {-41.1,38.6}, { 2 })});
            _RefreshRuleData.insert({ 5027, txnew Struct::RefreshRule({ 5027 }, {Enum::eRefreshThing::Monster }, { 4002 }, { 1 }, { 1 }, { 10 }, {27.9,27.6}, { 1 })});
            _RefreshRuleData.insert({ 5028, txnew Struct::RefreshRule({ 5028 }, {Enum::eRefreshThing::Monster }, { 4001 }, { 7 }, { 7 }, { 10 }, {-25.7,0.8}, { 1 })});
            _RefreshRuleData.insert({ 5029, txnew Struct::RefreshRule({ 5029 }, {Enum::eRefreshThing::Monster }, { 4003 }, { 1 }, { 2 }, { 10 }, {32.8,32.5}, { 1 })});
            _RefreshRuleData.insert({ 5030, txnew Struct::RefreshRule({ 5030 }, {Enum::eRefreshThing::Monster }, { 4003 }, { 1 }, { 2 }, { 10 }, {-9.9,49.5}, { 1 })});
            _RefreshRuleData.insert({ 5031, txnew Struct::RefreshRule({ 5031 }, {Enum::eRefreshThing::Monster }, { 4003 }, { 1 }, { 2 }, { 10 }, {-86.6,11.5}, { 1 })});
            _ResourceData.insert({ 4001, txnew Struct::Resource({ 4001 }, {Enum::eResources::Special }, { 9001 }, { 1 }, {"xx"}, {"UI/Sprites/Icon/Hp1"})});
            _ResourceData.insert({ 4002, txnew Struct::Resource({ 4002 }, {Enum::eResources::Material }, { 5001 }, { 1 }, {"xx"}, {"UI/Sprites/Icon/Hp1"})});
            _ResourceData.insert({ 4003, txnew Struct::Resource({ 4003 }, {Enum::eResources::FatigueValue }, { -1 }, { 1 }, {"xx"}, {"UI/Sprites/Icon/Hp1"})});
            _ResourceData.insert({ 4004, txnew Struct::Resource({ 4004 }, {Enum::eResources::Equipment }, { 2002 }, { 1 }, {"xx"}, {"UI/Sprites/Icon/Hp1"})});
            _ResourceData.insert({ 4005, txnew Struct::Resource({ 4005 }, {Enum::eResources::Special }, { 9001 }, { 1 }, {"xx"}, {"UI/Sprites/Icon/Hp1"})});
            _ResourceQualityData.insert({ 4001, txnew Struct::ResourceQuality({ 4001 }, {"白"}, {"xxx"}, {"xxx"})});
            _ResourceQualityData.insert({ 4002, txnew Struct::ResourceQuality({ 4002 }, {"篮"}, {"xxx"}, {"xxx"})});
            _ResourceQualityData.insert({ 4003, txnew Struct::ResourceQuality({ 4003 }, {"紫"}, {"xxx"}, {"xxx"})});
            _ResourceQualityData.insert({ 4004, txnew Struct::ResourceQuality({ 4004 }, {"金"}, {"xxx"}, {"xxx"})});
            _ResourceQualityData.insert({ 4005, txnew Struct::ResourceQuality({ 4005 }, {"红"}, {"xxx"}, {"xxx"})});
            _ResourceQualityData.insert({ 4006, txnew Struct::ResourceQuality({ 4006 }, {"彩"}, {"xxx"}, {"xxx"})});
            _SceneData.insert({ 20001, txnew Struct::Scene({ 20001 }, {Enum::eSceneType::Public }, {"潘家园"}, {0,10}, {"PanJiaYuan"}, {1001,1002,1003,1004,1005,1006,1007,1008,1009,1010}, { 999 }, { 999 }, { 6001 }, {4003}, {}, { 4009 }, {10001}, {Enum::eVisualSceneType::Public }, { 0 }, { false })});
            _SceneData.insert({ 20002, txnew Struct::Scene({ 20002 }, {Enum::eSceneType::BattleCopy }, {"测试地图"}, {-17.8,-23.9,8.43,67,40.1,49.5,83,26,28,53.3,46.3,7.5,5.2,-10}, {"TestBattle"}, {}, { 4 }, { 16 }, { 6001 }, {4003}, {5009,5008,5012,5013,5015,5016,5017,5018,5019,5020,5021,5022,5023,5024,5025,5026,5027,5028,5029,5030,5031}, { 4010 }, {10002}, {Enum::eVisualSceneType::Entity }, { 1500000 }, { true })});
            _SkillData.insert({ 11001, txnew Struct::Skill({ 11001 }, {"踢腿"}, {"向前快速踢腿，对敌人造成伤害同时造成流血效果"}, {Enum::eSkillEffect::Launcher}, {4}, {"UI/Sprites/Skill/Skill9_Wx"}, { true }, {"UISkill1"}, { 9500 }, { 0 }, {Enum::eSelectedType::UnAim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {0}, {Enum::eConsumeType::Mp }, { 20 }, { 10 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {4.7,11}, { false }, {"xiaoge-attack-A"}, { 850 }, { 254 }, { false }, { false }, { false }, {}, { false }, { 880 }, { 4007 }, { 20 }, { 100 }, { false }, { 765 }, { 0.2 }, { 1 }, { false })});
            _SkillData.insert({ 11002, txnew Struct::Skill({ 11002 }, {"血阵"}, {"释放圆形血阵，对敌人造成大量伤害"}, {Enum::eSkillEffect::Launcher}, {6}, {"UI/Sprites/Skill/Skill1_Wx"}, { true }, {"UISkill2"}, { 15000 }, { 0 }, {Enum::eSelectedType::UnAim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {0}, {Enum::eConsumeType::Mp }, { 30 }, { 12 }, {"UI/Sprites/Character/TrueHead"}, { 23.5 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Circle }, {5.65}, { true }, {"xiaoge-attack-B"}, { 750 }, { 251 }, { false }, { false }, { false }, {}, { false }, { 790 }, { 4005 }, { 21 }, { 101 }, { false }, { 675 }, { 13 }, { 1 }, { false })});
            _SkillData.insert({ 11003, txnew Struct::Skill({ 11003 }, {"疾步"}, {"向前快速滑行一段距离"}, {Enum::eSkillEffect::Move}, {1002}, {"UI/Sprites/Skill/Skill6_Wx"}, { true }, {"UISkill3"}, { 7000 }, { 0 }, {Enum::eSelectedType::UnAim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {0}, {Enum::eConsumeType::Mp }, { 25 }, { 10 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {3,13.5}, { false }, {"chongci"}, { 545 }, { 252 }, { false }, { false }, { false }, {}, { false }, { 665 }, { 4008 }, { 22 }, { 102 }, { false }, { 570 }, { 0.6 }, { 1 }, { false })});
            _SkillData.insert({ 11004, txnew Struct::Skill({ 11004 }, {"鬼哨"}, {"对敌人造成伤害同时减速敌人"}, {Enum::eSkillEffect::Launcher, Enum::eSkillEffect::Launcher}, {5,30}, {"UI/Sprites/Skill/Skill8_Wx"}, { true }, {"UISkill4"}, { 18500 }, { 0 }, {Enum::eSelectedType::UnAim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {11004}, {Enum::eConsumeType::Mp }, { 35 }, { 10 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {6.5,13}, { false }, {"xiaoge-attack-C"}, { 1450 }, { 253 }, { false }, { false }, { false }, {}, { false }, { 1650 }, { 4006 }, { 24 }, { 104 }, { false }, { 1480 }, { 1 }, { 1 }, { false })});
            _SkillData.insert({ 11005, txnew Struct::Skill({ 11005 }, {"小哥跟踪技能测试"}, {"技能描述测试"}, {Enum::eSkillEffect::Move}, {1001}, {"UI/Sprites/Skill/Skill7_Wx"}, { true }, {}, { 1503 }, { 0 }, {Enum::eSelectedType::Aim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {11003}, {Enum::eConsumeType::Mp }, { 18 }, { 10 }, {"UI/Sprites/Character/TrueHead"}, { 20 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Circle }, {1}, { true }, {"x"}, { 103 }, { 253 }, { false }, { false }, { false }, {}, { false }, { 103 }, { 4008 }, { 23 }, { 103 }, { false }, { 75 }, { 0.8 }, { 1 }, { false })});
            _SkillData.insert({ 11006, txnew Struct::Skill({ 11006 }, {"技能测试普攻1"}, {"技能描述测试"}, {Enum::eSkillEffect::Launcher}, {8}, {"UI/Sprites/Bag/WeaponSkill1"}, { false }, {}, { 775 }, { 1016 }, {Enum::eSelectedType::Liberty }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {11005}, {Enum::eConsumeType::Mp }, { 0 }, { 10 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {2,13}, { false }, {"xiaoge-attack3"}, { 735 }, { 250 }, { false }, { true }, { false }, {}, { false }, { 735 }, { 4001 }, { 25 }, { 300 }, { false }, { 675 }, { 0.5 }, { 1 }, { true })});
            _SkillData.insert({ 11007, txnew Struct::Skill({ 11007 }, {"技能测试普攻2"}, {"技能描述测试"}, {Enum::eSkillEffect::Launcher}, {9}, {"UI/Sprites/Bag/WeaponSkill1"}, { false }, {}, { 705 }, { 1016 }, {Enum::eSelectedType::Liberty }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {11006}, {Enum::eConsumeType::Mp }, { 0 }, { 10 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {2,13}, { false }, {"xiaoge-attack1"}, { 500 }, { 250 }, { false }, { true }, { false }, {}, { false }, { 705 }, { 4002 }, { 26 }, { 300 }, { false }, { 475 }, { 0.5 }, { 2 }, { false })});
            _SkillData.insert({ 11008, txnew Struct::Skill({ 11008 }, {"技能测试普攻3"}, {"技能描述测试"}, {Enum::eSkillEffect::Launcher}, {10}, {"UI/Sprites/Bag/WeaponSkill1"}, { false }, {}, { 1090 }, { 1015 }, {Enum::eSelectedType::Liberty }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {11007}, {Enum::eConsumeType::Mp }, { 0 }, { 10 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {2,13}, { false }, {"xiaoge-attack5"}, { 975 }, { 250 }, { false }, { true }, { false }, {}, { false }, { 1090 }, { 4003 }, { 550 }, { 300 }, { true }, { 965 }, { 0.5 }, { 2 }, { false })});
            _SkillData.insert({ 11009, txnew Struct::Skill({ 11009 }, {"潘子剑雨"}, {"潘子剑雨"}, {Enum::eSkillEffect::Launcher}, {25}, {"UI/Sprites/Skill/Skill5_Wx"}, { true }, {"UISkill2"}, { 15000 }, { 0 }, {Enum::eSelectedType::UnAim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {0}, {Enum::eConsumeType::Mp }, { 30 }, { 11 }, {"UI/Sprites/Character/TrueHead"}, { 22 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Circle }, {5.5}, { true }, {"attack1"}, { 1200 }, { 250 }, { false }, { false }, { false }, {}, { false }, { 1220 }, { 4022 }, { 20 }, { 100 }, { false }, { 1205 }, { 13 }, { 1 }, { true })});
            _SkillData.insert({ 11010, txnew Struct::Skill({ 11010 }, {"潘子爆裂箭"}, {"潘子爆裂箭"}, {Enum::eSkillEffect::Launcher, Enum::eSkillEffect::Launcher, Enum::eSkillEffect::Launcher, Enum::eSkillEffect::Launcher, Enum::eSkillEffect::Launcher}, {26,31,32,33,34}, {"UI/Sprites/Skill/Skill8_Wx8"}, { true }, {"UISkill3"}, { 20000 }, { 0 }, {Enum::eSelectedType::UnAim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {0}, {Enum::eConsumeType::Mp }, { 40 }, { 16 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {3.5,15}, { false }, {"attack3"}, { 1150 }, { 250 }, { false }, { false }, { false }, {}, { false }, { 1205 }, { 4021 }, { 20 }, { 100 }, { false }, { 1150 }, { 0.8 }, { 1 }, { true })});
            _SkillData.insert({ 11011, txnew Struct::Skill({ 11011 }, {"潘子向后位移"}, {"潘子向后位移向前射箭"}, {Enum::eSkillEffect::Move, Enum::eSkillEffect::Launcher}, {1007,29}, {"UI/Sprites/Skill/Skill4_Wx"}, { true }, {"UISkill1"}, { 8500 }, { 0 }, {Enum::eSelectedType::UnAim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {0}, {Enum::eConsumeType::Mp }, { 25 }, { 16 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {6.5,16}, { false }, {"attack2"}, { 750 }, { 252 }, { false }, { false }, { false }, {}, { false }, { 775 }, { 4023 }, { 22 }, { 102 }, { false }, { 750 }, { 0.6 }, { 1 }, { false })});
            _SkillData.insert({ 11012, txnew Struct::Skill({ 11012 }, {"潘子加buff"}, {"潘子加buff"}, {Enum::eSkillEffect::Launcher}, {27}, {"UI/Sprites/Skill/Skill10_Wx"}, { true }, {"UISkill4"}, { 15000 }, { 0 }, {Enum::eSelectedType::Liberty }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {0}, {Enum::eConsumeType::Mp }, { 20 }, { 10 }, {"UI/Sprites/Character/TrueHead"}, { 2 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {2.5,19}, { false }, {"attack1"}, { 10 }, { 250 }, { false }, { false }, { false }, {}, { false }, { 20 }, { 4024 }, { 20 }, { 100 }, { false }, { 15 }, { 0.8 }, { 1 }, { false })});
            _SkillData.insert({ 11013, txnew Struct::Skill({ 11013 }, {"潘子普攻"}, {"测试潘子普攻"}, {Enum::eSkillEffect::Launcher}, {24}, {"UI/Sprites/Skill/Skill9_Wx"}, { false }, {}, { 1000 }, { 0 }, {Enum::eSelectedType::UnAim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {0}, {Enum::eConsumeType::Mp }, { 0 }, { 18 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {2.5,19}, { false }, {"attack"}, { 650 }, { 250 }, { false }, { false }, { false }, {}, { false }, { 680 }, { 4020 }, { 20 }, { 100 }, { false }, { 625 }, { 0.8 }, { 1 }, { true })});
            _SkillData.insert({ 11014, txnew Struct::Skill({ 11014 }, {"潘子普攻"}, {"测试潘子普攻"}, {Enum::eSkillEffect::Launcher}, {24}, {"UI/Sprites/Skill/Skill9_Wx"}, { false }, {}, { 1000 }, { 0 }, {Enum::eSelectedType::UnAim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {0}, {Enum::eConsumeType::Mp }, { 0 }, { 18 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {2.5,19}, { false }, {"attack"}, { 650 }, { 250 }, { false }, { false }, { false }, {}, { false }, { 680 }, { 4020 }, { 20 }, { 100 }, { false }, { 625 }, { 0.8 }, { 1 }, { true })});
            _SkillData.insert({ 11015, txnew Struct::Skill({ 11015 }, {"潘子普攻"}, {"测试潘子普攻"}, {Enum::eSkillEffect::Launcher}, {24}, {"UI/Sprites/Skill/Skill9_Wx"}, { false }, {}, { 1000 }, { 0 }, {Enum::eSelectedType::UnAim }, {Enum::eTargetType::Monster, Enum::eTargetType::Boss, Enum::eTargetType::EnemyCharacter}, { false }, { 1 }, { 3 }, {0}, {Enum::eConsumeType::Mp }, { 0 }, { 10 }, {"UI/Sprites/Character/TrueHead"}, { 0 }, {"UI/Sprites/Character/TrueHead"}, {Enum::eSheetsType::Rectangle }, {2,13}, { false }, {"attack"}, { 650 }, { 250 }, { false }, { false }, { false }, {}, { false }, { 680 }, { 4020 }, { 20 }, { 100 }, { false }, { 625 }, { 0.8 }, { 1 }, { true })});
            _SoundData.insert({ 4001, txnew Struct::Sound({ 4001 }, {"小哥普攻1"}, {Enum::eSoundType::EffectAll }, {"Sound/Effect/xiaoge_atk1"}, { false })});
            _SoundData.insert({ 4002, txnew Struct::Sound({ 4002 }, {"小哥普攻2"}, {Enum::eSoundType::EffectAll }, {"Sound/Effect/xiaoge_atk2"}, { false })});
            _SoundData.insert({ 4003, txnew Struct::Sound({ 4003 }, {"小哥普攻3"}, {Enum::eSoundType::EffectAll }, {"Sound/Effect/xiaoge_atk3"}, { false })});
            _SoundData.insert({ 4004, txnew Struct::Sound({ 4004 }, {"小哥移动"}, {Enum::eSoundType::EffectAll }, {"Sound/Effect/yidong"}, { false })});
            _SoundData.insert({ 4005, txnew Struct::Sound({ 4005 }, {"小哥血阵"}, {Enum::eSoundType::EffectAll }, {"Sound/Effect/Blood"}, { false })});
            _SoundData.insert({ 4006, txnew Struct::Sound({ 4006 }, {"小哥声波"}, {Enum::eSoundType::EffectAll }, {"Sound/Effect/Wistle"}, { false })});
            _SoundData.insert({ 4007, txnew Struct::Sound({ 4007 }, {"小哥踢腿"}, {Enum::eSoundType::EffectAll }, {"Sound/Effect/xiaoge_atk3"}, { false })});
            _SoundData.insert({ 4008, txnew Struct::Sound({ 4008 }, {"小哥位移"}, {Enum::eSoundType::EffectAll }, {"Sound/Effect/Move"}, { false })});
            _SoundData.insert({ 4009, txnew Struct::Sound({ 4009 }, {"背景音乐"}, {Enum::eSoundType::EffectSelf }, {"Sound/Bgm/dating"}, { true })});
            _SoundData.insert({ 4010, txnew Struct::Sound({ 4010 }, {"战斗场景音乐"}, {Enum::eSoundType::EffectSelf }, {"Sound/Bgm/fuben"}, { true })});
            _SoundData.insert({ 4011, txnew Struct::Sound({ 4011 }, {"交互"}, {Enum::eSoundType::EffectSelf }, {"Sound/Bgm/jiaohu"}, { false })});
            _SoundData.insert({ 4012, txnew Struct::Sound({ 4012 }, {"普通怪物挨打"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/Hurt"}, { false })});
            _SoundData.insert({ 4013, txnew Struct::Sound({ 4013 }, {"普通怪物死亡"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/Die"}, { false })});
            _SoundData.insert({ 4014, txnew Struct::Sound({ 4014 }, {"九头蛇柏普攻"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/Snake_Atk"}, { false })});
            _SoundData.insert({ 4015, txnew Struct::Sound({ 4015 }, {"九头蛇柏飞出头"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/Snake_Fly"}, { false })});
            _SoundData.insert({ 4016, txnew Struct::Sound({ 4016 }, {"藤曼飞出来"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/Snake_Tengman"}, { false })});
            _SoundData.insert({ 4017, txnew Struct::Sound({ 4017 }, {"九头蛇柏地刺"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/Snake_Dici"}, { false })});
            _SoundData.insert({ 4018, txnew Struct::Sound({ 4018 }, {"小哥死亡"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/XiaogeDie"}, { false })});
            _SoundData.insert({ 4019, txnew Struct::Sound({ 4019 }, {"蛇柏插入地下"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/Snake_Insert"}, { false })});
            _SoundData.insert({ 4020, txnew Struct::Sound({ 4020 }, {"潘子普攻"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/PanAtk"}, { false })});
            _SoundData.insert({ 4021, txnew Struct::Sound({ 4021 }, {"潘子火箭"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/PanHuoJian"}, { false })});
            _SoundData.insert({ 4022, txnew Struct::Sound({ 4022 }, {"潘子剑雨"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/PanJianyu"}, { false })});
            _SoundData.insert({ 4023, txnew Struct::Sound({ 4023 }, {"潘子后撤"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/PanHouche"}, { false })});
            _SoundData.insert({ 4024, txnew Struct::Sound({ 4024 }, {"潘子buff"}, {Enum::eSoundType::EffectSelf }, {"Sound/Effect/PanBuff"}, { false })});
            _SpecialData.insert({ 9001, txnew Struct::Special({ 9001 }, {"战国帛书"}, {Enum::eTransactionType::None }, {Enum::eSoldType::None }, {"战国帛书拓本是进入鲁王公墓的特别物件"}, {Enum::eResources::Coin }, { 100 }, {"xxx.png"}, {"任务材料"}, { 1 }, {"RewardItem/LootItem/LootItem/Res/IMG/TrueHead"}, { 1001 })});
            _SpecialEffectsData.insert({ 1001, txnew Struct::SpecialEffects({ 1001 }, {"某某特效"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Pz_Skill1Xuli"}, {Enum::ePosition::MountBone }, { 45 })});
            _SpecialEffectsData.insert({ 1002, txnew Struct::SpecialEffects({ 1002 }, {"某某特效"}, {"Effect/Prefabs/Environment/Fire/Cartoon/StarFogFlowRed"}, {Enum::ePosition::MountBone }, { 46 })});
            _SpecialEffectsData.insert({ 1003, txnew Struct::SpecialEffects({ 1003 }, {"某某特效"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill2"}, {Enum::ePosition::MountBone }, { 47 })});
            _SpecialEffectsData.insert({ 1004, txnew Struct::SpecialEffects({ 1004 }, {"某某特效"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill2"}, {Enum::ePosition::MountBone }, { 48 })});
            _SpecialEffectsData.insert({ 1005, txnew Struct::SpecialEffects({ 1005 }, {"某某特效"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill3"}, {Enum::ePosition::None }, { 49 })});
            _SpecialEffectsData.insert({ 1006, txnew Struct::SpecialEffects({ 1006 }, {"直线子弹特效"}, {"Effect/Prefabs/Environment/Fire/Cartoon/PowerupGlow2"}, {Enum::ePosition::None }, { 0 })});
            _SpecialEffectsData.insert({ 1007, txnew Struct::SpecialEffects({ 1007 }, {"抛物线子弹"}, {"Effect/Prefabs/Environment/Fire/Cartoon/SpinPortalGold"}, {Enum::ePosition::None }, { 0 })});
            _SpecialEffectsData.insert({ 1008, txnew Struct::SpecialEffects({ 1008 }, {"跟踪弹"}, {"Effect/Prefabs/Environment/Fire/Cartoon/PowerupGlow2"}, {Enum::ePosition::None }, { 0 })});
            _SpecialEffectsData.insert({ 1009, txnew Struct::SpecialEffects({ 1009 }, {"直线子弹爆炸"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Dead1"}, {Enum::ePosition::None }, { 500 })});
            _SpecialEffectsData.insert({ 1010, txnew Struct::SpecialEffects({ 1010 }, {"抛物线爆炸"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill1_2"}, {Enum::ePosition::None }, { 750 })});
            _SpecialEffectsData.insert({ 1011, txnew Struct::SpecialEffects({ 1011 }, {"跟踪弹爆炸"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Dead1"}, {Enum::ePosition::None }, { 500 })});
            _SpecialEffectsData.insert({ 1012, txnew Struct::SpecialEffects({ 1012 }, {"直线弹道拖尾特效"}, {"Effect/Prefabs/Environment/Fire/Cartoon/SoftFireAdditiveBlue"}, {Enum::ePosition::None }, { 0 })});
            _SpecialEffectsData.insert({ 1013, txnew Struct::SpecialEffects({ 1013 }, {"抛物线拖尾"}, {"Effect/Prefabs/Environment/Fire/Cartoon/SoftFireAdditiveBlue"}, {Enum::ePosition::None }, { 0 })});
            _SpecialEffectsData.insert({ 1014, txnew Struct::SpecialEffects({ 1014 }, {"跟踪弹拖尾"}, {"Effect/Prefabs/Environment/Fire/Cartoon/SoftFireAdditiveBlue"}, {Enum::ePosition::None }, { 0 })});
            _SpecialEffectsData.insert({ 1015, txnew Struct::SpecialEffects({ 1015 }, {"普攻特效3时长"}, {"Spine/VFX/Prefabs/Atk11"}, {Enum::ePosition::WorldPosition }, { 800 })});
            _SpecialEffectsData.insert({ 1016, txnew Struct::SpecialEffects({ 1016 }, {"普攻特效1时长"}, {"Spine/VFX/Prefabs/Atk22"}, {Enum::ePosition::WorldPosition }, { 700 })});
            _SpecialEffectsData.insert({ 1017, txnew Struct::SpecialEffects({ 1017 }, {"藤曼消失特效"}, {"Spine/VFX/Monster/Prefabs/JiuTouSheBai/BundingBack"}, {Enum::ePosition::WorldPosition }, { 830 })});
            _SpecialEffectsData.insert({ 1018, txnew Struct::SpecialEffects({ 1018 }, {"藤曼受击特效1"}, {"Spine/VFX/Monster/Prefabs/JiuTouSheBai/BeAttack1"}, {Enum::ePosition::MountBone }, { 285 })});
            _SpecialEffectsData.insert({ 1019, txnew Struct::SpecialEffects({ 1019 }, {"Boss受击特效1"}, {"Spine/VFX/Monster/Prefabs/JiuTouSheBai/BeAttack2"}, {Enum::ePosition::MountBone }, { 285 })});
            _SpecialEffectsData.insert({ 1020, txnew Struct::SpecialEffects({ 1020 }, {"声波眩晕"}, {"Effect/Prefabs/Environment/Fire/Cartoon/SpinPortalGold1"}, {Enum::ePosition::MountBone }, { 300 })});
            _SpecialEffectsData.insert({ 1021, txnew Struct::SpecialEffects({ 1021 }, {"减速"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Pz_Skill2Xuli"}, {Enum::ePosition::MountBone }, { 300 })});
            _SpecialEffectsData.insert({ 1022, txnew Struct::SpecialEffects({ 1022 }, {"持续流血"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill2_1"}, {Enum::ePosition::MountBone }, { 300 })});
            _SpecialEffectsData.insert({ 1023, txnew Struct::SpecialEffects({ 1023 }, {"测试刀光"}, {"Spine/VFX/Monster/Prefabs/XueShi/XueShiAtk"}, {Enum::ePosition::MountBone }, { 350 })});
            _SpecialEffectsData.insert({ 1024, txnew Struct::SpecialEffects({ 1024 }, {"持续流血大量"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill2_2"}, {Enum::ePosition::MountBone }, { 300 })});
            _SpecialEffectsData.insert({ 1025, txnew Struct::SpecialEffects({ 1025 }, {"潘子加移速"}, {"Spine/VFX/Prefabs/PanZi/Buff"}, {Enum::ePosition::MountBone }, { 300 })});
            _SpecialEffectsData.insert({ 1026, txnew Struct::SpecialEffects({ 1026 }, {"爆裂箭爆炸"}, {"Spine/VFX/Bullet/Boom1"}, {Enum::ePosition::MountBone }, { 350 })});
            _SpecialEffectsData.insert({ 1027, txnew Struct::SpecialEffects({ 1027 }, {"低级升级特效"}, {"Effect/Prefabs/Environment/Fire/Cartoon/Wpz_Skill5"}, {Enum::ePosition::MountBone }, { 650 })});
            _TalkContentData.insert({ 41001, txnew Struct::TalkContent({ 41001 }, {"王涛，你站在这等我给你买橘子"}, {"张茂青"}, {"xxx"})});
            _TalkContentData.insert({ 41002, txnew Struct::TalkContent({ 41002 }, {"好的"}, {"王涛"}, {"xxx"})});
            _TaskData.insert({ 10001, txnew Struct::Task({ 10001 }, {Enum::eTaskReceiveType::TriggleNpc }, {4004}, {"默认的初始任务，找npc1接触"}, { 0 }, {11006}, { 1001 }, { 1001 }, {"xxx"}, {"xxx"}, { 20001 }, {"我是王涛"}, {})});
            _TaskData.insert({ 10002, txnew Struct::Task({ 10002 }, {Enum::eTaskReceiveType::NoTriggle }, {4002}, {"完成上个后直接进入的任务。接触任务npc1"}, { 6001 }, {11002}, { 0 }, { 1002 }, {"xxx"}, {"xxx"}, { 20001 }, {"xxxx"}, {})});
            _TaskData.insert({ 10003, txnew Struct::Task({ 10003 }, {Enum::eTaskReceiveType::TriggleNpc }, {4003}, {"3穿戴装备任务"}, { 6002 }, {11005}, { 1001 }, { 1001 }, {"xxx"}, {"xxx"}, { 20001 }, {"xxxx"}, {})});
            _TaskCompleteConditionData.insert({ 11001, txnew Struct::TaskCompleteCondition({ 11001 }, {Enum::eTaskType::TaskLevel }, { 1001 })});
            _TaskCompleteConditionData.insert({ 11002, txnew Struct::TaskCompleteCondition({ 11002 }, {Enum::eTaskType::TaskContact }, { 51002 })});
            _TaskCompleteConditionData.insert({ 11003, txnew Struct::TaskCompleteCondition({ 11003 }, {Enum::eTaskType::TaskMaterail }, { 1001 })});
            _TaskCompleteConditionData.insert({ 11004, txnew Struct::TaskCompleteCondition({ 11004 }, {Enum::eTaskType::TaskKill }, { 1001 })});
            _TaskCompleteConditionData.insert({ 11005, txnew Struct::TaskCompleteCondition({ 11005 }, {Enum::eTaskType::TaskUse }, { 1001 })});
            _TaskCompleteConditionData.insert({ 11006, txnew Struct::TaskCompleteCondition({ 11006 }, {Enum::eTaskType::TaskContact }, { 51001 })});
            _TaskContactData.insert({ 51001, txnew Struct::TaskContact({ 51001 }, {"和Npc1交谈"}, { 1001 })});
            _TaskContactData.insert({ 51002, txnew Struct::TaskContact({ 51002 }, {"和张爹交流"}, { 1002 })});
            _TaskEventData.insert({ 4001, txnew Struct::TaskEvent({ 4001 }, {Enum::eTaskEvent::PreRecive }, { 10001 })});
            _TaskEventData.insert({ 4002, txnew Struct::TaskEvent({ 4002 }, {Enum::eTaskEvent::Received }, { 10001 })});
            _TaskEventData.insert({ 4003, txnew Struct::TaskEvent({ 4003 }, {Enum::eTaskEvent::Complete }, { 10001 })});
            _TaskEventData.insert({ 4004, txnew Struct::TaskEvent({ 4004 }, {Enum::eTaskEvent::Completed }, { 10001 })});
            _TaskEventData.insert({ 4005, txnew Struct::TaskEvent({ 4005 }, {Enum::eTaskEvent::Completed }, { 10002 })});
            _TaskEventData.insert({ 4006, txnew Struct::TaskEvent({ 4006 }, {Enum::eTaskEvent::None }, { 10001 })});
            _TaskKillData.insert({ 1001, txnew Struct::TaskKill({ 1001 }, {"杀死xxx野怪"}, {61001,61002})});
            _TaskLevelData.insert({ 1001, txnew Struct::TaskLevel({ 1001 }, {"通关鲁王宫副本"}, { 20001 })});
            _TaskMaterailData.insert({ 1001, txnew Struct::TaskMaterail({ 1001 }, {"收集材料xxx材料"}, {4004})});
            _TaskSystemData.insert({ 20001, txnew Struct::TaskSystem({ 20001 }, {Enum::eTaskVariety::MainMission }, {"秦岭神树1"}, { 6001 }, { 6001 }, {10001,10002,10003})});
            _TaskSystemData.insert({ 20002, txnew Struct::TaskSystem({ 20002 }, {Enum::eTaskVariety::SideMission }, {"秦岭神树2"}, { 6001 }, { 6001 }, {10001})});
            _TaskSystemData.insert({ 20003, txnew Struct::TaskSystem({ 20003 }, {Enum::eTaskVariety::SpecialMission }, {"秦岭神树3"}, { 6001 }, { 6001 }, {10001})});
            _TaskSystemData.insert({ 20004, txnew Struct::TaskSystem({ 20004 }, {Enum::eTaskVariety::MainMission }, {"秦岭神树4"}, { 6001 }, { 6001 }, {10001})});
            _TaskSystemData.insert({ 20005, txnew Struct::TaskSystem({ 20005 }, {Enum::eTaskVariety::SideMission }, {"秦岭神树5"}, { 6001 }, { 6001 }, {10001})});
            _TaskUseData.insert({ 1001, txnew Struct::TaskUse({ 1001 }, {"穿戴衣服"}, { 4004 })});
            _TestsData.insert({ 100001, txnew Struct::Tests({ 100001 }, { 112388 }, { 10.012312 }, {"Hello Test String1"}, { false }, {Enum::eTransactionType::Tradable }, {100,12333,9987}, {1.678,8.9756}, {"HelloWorld1", "Helloword2", "Hellosowed1"}, {}, {Enum::eTransactionType::Tradable, Enum::eTransactionType::None, Enum::eTransactionType::Tradable})});
            _TestsData.insert({ 100002, txnew Struct::Tests({ 100002 }, { 1123123 }, { 120.065123 }, {"Hello Test String2"}, { false }, {Enum::eTransactionType::Tradable }, {9987}, {8.9756}, {}, {false,true,false}, {})});
            _TestsData.insert({ 100003, txnew Struct::Tests({ 100003 }, { 567 }, { 3510.0123 }, {"Hello Test String3"}, { false }, {Enum::eTransactionType::Tradable }, {}, {}, {"HelloWorld1", ""}, {false,}, {Enum::eTransactionType::Tradable})});
            _TransPortData.insert({ 10001, txnew Struct::TransPort({ 10001 }, {"潘家园主城传送到副本"}, { 20002 }, {54,36}, {Enum::eTransPortType::Normal }, { 10 }, { 0 }, { 1000 }, { 2 }, {"Effect/Prefabs/Environment/Fire/Cartoon/SpinPortalGold"}, { true }, {"blood_3"})});
            _TransPortData.insert({ 10002, txnew Struct::TransPort({ 10002 }, {"战斗地点撤离点传送回到潘家园"}, { 20001 }, {-2.32,51.93}, {Enum::eTransPortType::Evacuate }, { 900000 }, { 4 }, { 15000 }, { 2 }, {"Effect/Prefabs/Environment/Fire/Cartoon/SpinPortalGold2"}, { true }, {"blood_3"})});

        }

    };

    const IAutoConfig* GetAutoConfig() {
        return AutoConfigData::GetInstance();
    }
}
