#include "TaskManager.h"

Api::iEngine* g_Engine = nullptr;
ICharacterManager* g_CharacterManager = nullptr;
ICacheClient* g_CacheClient = nullptr;
IPlayerManager* g_PlayerManager = nullptr;
IConfig* g_Config = nullptr;
IGameWorld* g_GameWorld = nullptr;
IBackpack* g_Backpack = nullptr;

static TaskManager* s_Self = nullptr;
static std::unordered_map<UInt64, std::unordered_map<Int64, oTask>> s_PlayerTaskDataMap;

bool TaskManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool TaskManager::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_Config, Config);
    GetModule(g_Engine, g_CharacterManager, CharacterManager);
    GetModule(g_Engine, g_CacheClient, CacheClient);
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_GameWorld, GameWorld);
    GetModule(g_Engine, g_Backpack, Backpack);

    g_PlayerManager->RegistPlayerProtobuf(eClientID::AcceptTaskReq, TaskManager::OnAcceptTaskReq);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::SubmitTaskReq, TaskManager::OnSubmitTaskReq);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::CompleteTask, TaskManager::OnCompleteTask);
    g_PlayerManager->RegistPlayerProtobuf(eClientID::NpcContact, TaskManager::OnNpcContact);

    g_PlayerManager->RegistPlayerEvent(IPlayerManager::ePlayerEvent::NewPlayerCreated, OnNewPlayerCreated);
    g_PlayerManager->RegistPlayerEvent(IPlayerManager::ePlayerEvent::PlayerLoaded, OnPlayerCreated);
    g_PlayerManager->RegistPlayerEvent(IPlayerManager::ePlayerEvent::PlayerRelease, OnPlayerRelease);

    return true;
}

void TaskManager::OnAcceptTaskReq(IPlayer* const player, const UInt64 account, const oAcceptTaskreq& req)
{
    if (s_PlayerTaskDataMap.find(account) == s_PlayerTaskDataMap.end())
    {
        ErrorLog(g_Engine, "Not Find Task Data Account %11u", account);
        return;
    }

    if (s_PlayerTaskDataMap[account].find(req.taskid()) == s_PlayerTaskDataMap[account].end())
    {
        ErrorLog(g_Engine, "Not Find Task Data Task ID %11d", req.taskid());
        return;
    }

    if (static_cast<AutoConfig::Enum::eTaskEvent>(s_PlayerTaskDataMap[account].find(req.taskid())->second.taskstatus()) != AutoConfig::Enum::eTaskEvent::PreRecive)
    {
        ErrorLog(g_Engine, "Not Find Task Data %d", s_PlayerTaskDataMap[account].find(req.taskid())->second.taskstatus());
        return;
    }

    s_Self->TaskStatusChange(account, req.taskid(), AutoConfig::Enum::eTaskEvent::Received);
}

void TaskManager::OnSubmitTaskReq(IPlayer* const player, const UInt64 account, const oSubmitTaskReq& req)
{
    if (s_PlayerTaskDataMap.find(account) == s_PlayerTaskDataMap.end())
    {
        ErrorLog(g_Engine, "Not Find Task Data Account %11u", account);
        return;
    }

    if (s_PlayerTaskDataMap[account].find(req.taskid()) == s_PlayerTaskDataMap[account].end())
    {
        ErrorLog(g_Engine, "Not Find Task Data Task ID %11d", req.taskid());
        return;
    }

    if (static_cast<AutoConfig::Enum::eTaskEvent>(s_PlayerTaskDataMap[account].find(req.taskid())->second.taskstatus()) != AutoConfig::Enum::eTaskEvent::Complete)
    {
        ErrorLog(g_Engine, "Not Find Task Data %d", s_PlayerTaskDataMap[account].find(req.taskid())->second.taskstatus());
        return;
    }

    s_Self->TaskStatusChange(account, req.taskid(), AutoConfig::Enum::eTaskEvent::Completed);

    //发放奖励
    const AutoConfig::Struct::Task* taskConfig = g_Config->GetAutoConfig()->QueryTaskConfig(req.taskid());
    std::vector<Int64> resourceIds;
    for (int i = 0; i < taskConfig->Resource.size(); i++)
    {
        resourceIds.push_back(taskConfig->Resource[i]);
    }
    g_Backpack->GetRewardItems(player->GetDeployCharacterID(), resourceIds);
}

void TaskManager::OnCompleteTask(IPlayer* const player, const UInt64 account, const oCompleteTask& req)
{
    if (s_PlayerTaskDataMap.find(account) == s_PlayerTaskDataMap.end())
    {
        ErrorLog(g_Engine, "Not Find Task Data Account %11u", account);
        return;
    }

    if (s_PlayerTaskDataMap[account].find(req.taskid()) == s_PlayerTaskDataMap[account].end())
    {
        ErrorLog(g_Engine, "Not Find Task Data Task ID %11d", req.taskid());
        return;
    }

    if (static_cast<AutoConfig::Enum::eTaskEvent>(s_PlayerTaskDataMap[account].find(req.taskid())->second.taskstatus()) != AutoConfig::Enum::eTaskEvent::Received)
    {
        ErrorLog(g_Engine, "Not Find Task Data %d", s_PlayerTaskDataMap[account].find(req.taskid())->second.taskstatus());
        return;
    }

    s_Self->TaskStatusChange(account, req.taskid(), AutoConfig::Enum::eTaskEvent::Complete);
}

void TaskManager::OnNewPlayerCreated(IPlayer* const player, const UInt64 account)
{
    oTaskDataPush taskData;
    //默认任务
    oTask* task = taskData.add_tasks();
    task->set_configid(AutoConfig::Const::InitialTask::Task);
    task->set_taskstatus(static_cast<int>(AutoConfig::Enum::eInteractType::Wait));
    const AutoConfig::Struct::Task* taskConfig = g_Config->GetAutoConfig()->QueryTaskConfig(AutoConfig::Const::InitialTask::Task);
    for (int i = 0; i < taskConfig->TaskCompleteCondition.size(); i++)
    {
        oTaskCompleteCondition* condition = task->add_conditions();
        condition->set_conditionid(taskConfig->TaskCompleteCondition[i]);
        condition->set_iscomplete(false);
        condition->set_progress(0);
    }
    s_PlayerTaskDataMap.insert({ account, {} });
    s_PlayerTaskDataMap[account].insert({ task->configid(), *task });
    g_CacheClient->Create(account, eTable::t_task,
        [&](ISQLCacheRow* const row) {
            std::string serializedStr;
            bool serializeSuccess = taskData.SerializeToString(&serializedStr);
            std::string encodedStr = XEngine::XBase64::Encode(serializedStr);
            row->GetColumn(t_task::TaskData__String__)->SetString(encodedStr);
            
            row->GetColumn(t_task::Account__UInt64__PrimaryUnique)->SetUInt64(account);
        },
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (succeed) {
                g_PlayerManager->SendProtobufToPlayer(account, eServerID::TaskDataPush, taskData);
            }
            else {
                ErrorLog(g_Engine, "Create Task Data %11u Account Error", account);
            }
        });
}
 
void TaskManager::OnPlayerCreated(IPlayer* const player, const UInt64 account)
{
    auto itor = s_PlayerTaskDataMap.find(account);
    if (itor != s_PlayerTaskDataMap.end())
    {
        oTaskDataPush taskData;
        for (auto& itor2 : itor->second)
        {
            *taskData.add_tasks() = itor2.second;
        }
        g_PlayerManager->SendProtobufToPlayer(account, eServerID::TaskDataPush, taskData);
        return;
    }
    g_CacheClient->Query(account, eTable::t_task, t_task::Account__UInt64__PrimaryUnique, account,
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (!succeed)
            {
                ErrorLog(g_Engine, "Query Task %11u Account Error", account);
                return;
            }

            if (tab != nullptr)
            {
                tab->Query(t_task::Account__UInt64__PrimaryUnique, account,
                    [&](ISQLCacheRow* const row) {
                        oTaskDataPush taskData;
                        std::string decodedStr = XEngine::XBase64::Decode(row->GetColumn(t_task::TaskData__String__)->GetString());
                        taskData.ParseFromString(decodedStr);

                        s_PlayerTaskDataMap.insert({ account, {} });
                        for (auto& task : taskData.tasks())
                        {
                            s_PlayerTaskDataMap[account].insert({task.configid(), task});
                        }

                        g_PlayerManager->SendProtobufToPlayer(account, eServerID::TaskDataPush, taskData);
                    });
            }       
        });
}

void TaskManager::OnPlayerRelease(IPlayer* const player, const UInt64 account)
{
    auto itor = s_PlayerTaskDataMap.find(account);
    if (itor != s_PlayerTaskDataMap.end())
    {
        s_Self->SaveTaskData(account);
        s_PlayerTaskDataMap.erase(itor);
    }
}

void TaskManager::SaveTaskData(const UInt64 account)
{
    g_CacheClient->Update(account, eTable::t_task, t_task::Account__UInt64__PrimaryUnique, account,
        [&](ISQLCacheRow* const row) {
            oTaskDataPush taskData;
            for (auto& itor2 : s_PlayerTaskDataMap.find(account)->second)
            {
                *taskData.add_tasks() = itor2.second;
            }
            std::string serializedStr;
            bool serializeSuccess = taskData.SerializeToString(&serializedStr);
            std::string encodedStr = XEngine::XBase64::Encode(serializedStr);
            row->GetColumn(t_task::TaskData__String__)->SetString(encodedStr);
        },
        [=](const bool succeed, const ISQLCacheTable* const tab) {
            if (!succeed)
            {
                ErrorLog(g_Engine, "OnCharacterRelease, TaskManager");
            }
        }
    );
}

void TaskManager::TaskStatusChange(const UInt64 account, const Int64 taskId, const AutoConfig::Enum::eTaskEvent stauts)
{
    s_PlayerTaskDataMap[account].find(taskId)->second.set_taskstatus(static_cast<int>(stauts));

    oTaskStatusChange change;
    change.set_allocated_changetask(&s_PlayerTaskDataMap[account].find(taskId)->second);

    g_PlayerManager->SendProtobufToPlayer(account, eServerID::TaskStatusChange, change);

    AnalyzeNewTask(account);
}

void TaskManager::OnNpcContact(IPlayer* const player, const UInt64 account, const oNpcContact& data)
{
    if (s_PlayerTaskDataMap.find(account) == s_PlayerTaskDataMap.end()) {
        ErrorLog(g_Engine, "Account %11u not found in task data map", account);
        return;
    }

    IGameObject* npc = g_GameWorld->QueryGameObject(data.npcguid());
    if (!npc)
    {
        ErrorLog(g_Engine, "Npc Not Found: %11u", data.npcguid());
        return;
    }

    IGameObject* character = g_GameWorld->QueryGameObject(player->GetDeployCharacterID());
    if ((npc->Position() - character->Position()).Length() <= AutoConfig::Const::Npc::NpcTriggle)
    {
        ErrorLog(g_Engine, "Npc Is Not Near: %11u ", data.npcguid())
        return;
    }

    for (const auto& [id, task] : s_PlayerTaskDataMap[account])
    {
        if (task.taskstatus() == static_cast<int>(AutoConfig::Enum::eTaskEvent::Received))
        {
            bool isComplete = true;
            for (auto condition : task.conditions())
            {
                const AutoConfig::Struct::TaskCompleteCondition* taskCompleteConditionConfig = g_Config->GetAutoConfig()->QueryTaskCompleteConditionConfig(condition.conditionid());
                if (taskCompleteConditionConfig->TaskType == AutoConfig::Enum::eTaskType::TaskContact)
                {
                    const AutoConfig::Struct::TaskUse* taskUseConfig = g_Config->GetAutoConfig()->QueryTaskUseConfig(taskCompleteConditionConfig->TaskTypeID);
                    if (taskUseConfig->Resource == npc->ConfigID())
                    {
                        condition.set_iscomplete(true);
                    }
                }

                isComplete = isComplete && condition.iscomplete();
            }

            if (isComplete)
            {
                s_Self->TaskStatusChange(account, id, AutoConfig::Enum::eTaskEvent::Complete);
            }
        }
    }
}

void TaskManager::OnEquipWear(const UInt64 account, const UInt64 equipGuid)
{
    if (s_PlayerTaskDataMap.find(account) == s_PlayerTaskDataMap.end()) {
        ErrorLog(g_Engine, "Account %11u not found in task data map", account);
        return;
    }

    IGameObject* equip = g_GameWorld->QueryGameObject(equipGuid);
    if (!equip) {
        ErrorLog(g_Engine, "Equip not found: %11u", equipGuid);
        return;
    }

    for (const auto& [id, task] : s_PlayerTaskDataMap[account])
    {
        if (task.taskstatus() == static_cast<int>(AutoConfig::Enum::eTaskEvent::Received))
        {
            bool isComplete = true;
            for (auto condition : task.conditions())
            {
                const AutoConfig::Struct::TaskCompleteCondition* taskCompleteConditionConfig = g_Config->GetAutoConfig()->QueryTaskCompleteConditionConfig(condition.conditionid());
                if (taskCompleteConditionConfig->TaskType == AutoConfig::Enum::eTaskType::TaskUse)
                {
                    const AutoConfig::Struct::TaskUse* taskUseConfig = g_Config->GetAutoConfig()->QueryTaskUseConfig(taskCompleteConditionConfig->TaskTypeID);
                    if (taskUseConfig->Resource == equip->GetInt64(Item::ResourceIdInt64))
                    {
                        condition.set_iscomplete(true);                                                                                                                                                                                         
                    }
                }

                isComplete = isComplete && condition.iscomplete();
            }

            if (isComplete)
            {
                TaskStatusChange(account, id, AutoConfig::Enum::eTaskEvent::Complete);
            }
        }
    }
}

void TaskManager::AnalyzeNewTask(const UInt64 account)
{
    oNewTaskPush push;
    const auto& taskMap = g_Config->GetAutoConfig()->GetTaskConfigMap();

    if (s_PlayerTaskDataMap.find(account) == s_PlayerTaskDataMap.end()) {
        ErrorLog(g_Engine, "Account %11u not found in task data map", account);
        return;
    }
    bool isNewTaskAdd = false;
    for (const auto& [id, task] : taskMap)
    {
        if (s_PlayerTaskDataMap[account].find(id) == s_PlayerTaskDataMap[account].end())
        {
            //判断未解锁的任务是否解锁
            const AutoConfig::Struct::Prerequisite* prerequisiteConfig = g_Config->GetAutoConfig()->QueryPrerequisiteConfig(task->Prerequisite);
            bool unLock = true;
            //先判断人物等级
            if (prerequisiteConfig->CharacterLevel != 0)
            {
                const AutoConfig::Struct::CharacterLevel* characterLevelConfig = g_Config->GetAutoConfig()->QueryCharacterLevelConfig(prerequisiteConfig->CharacterLevel);
                IGameObject* character = g_GameWorld->QueryGameObject(g_PlayerManager->QueryPlayer(account)->GetDeployCharacterID());
                unLock = character->GetInt64(Character::LevelInt64) >= characterLevelConfig->ID;
            }
            //再判断任务要求
            if (prerequisiteConfig->TaskEvent != 0)
            {
                const AutoConfig::Struct::TaskEvent* taskEventConfig = g_Config->GetAutoConfig()->QueryTaskEventConfig(prerequisiteConfig->TaskEvent);
                if (s_PlayerTaskDataMap[account].find(taskEventConfig->Task) != s_PlayerTaskDataMap[account].end())
                {
                    unLock = s_PlayerTaskDataMap[account].find(taskEventConfig->Task)->second.taskstatus() >= static_cast<int>(taskEventConfig->TaskProgress);
                }
                else
                {
                    unLock = false;
                }
            }
            //最后判断资源要求
            if (prerequisiteConfig->Resource[0] != 0)
            {
                for (int i = 0; i < prerequisiteConfig->Resource.size(); i++)
                {
                    if (!g_Backpack->QueryResourceByID(g_PlayerManager->QueryPlayer(account)->GetDeployCharacterID(), prerequisiteConfig->Resource[i]))
                    {
                        unLock = false;
                        break;
                    }
                }
            }

            if (unLock)
            {
                oTask newTask;
                newTask.set_configid(id);
                newTask.set_taskstatus(static_cast<int>(task->TaskReceiveType == AutoConfig::Enum::eTaskReceiveType::TriggleNpc ? AutoConfig::Enum::eTaskEvent::PreRecive : AutoConfig::Enum::eTaskEvent::Received));
                const AutoConfig::Struct::Task* taskConfig = g_Config->GetAutoConfig()->QueryTaskConfig(id);
                for (int i = 0; i < taskConfig->TaskCompleteCondition.size(); i++)
                {
                    oTaskCompleteCondition* condition = newTask.add_conditions();
                    condition->set_conditionid(taskConfig->TaskCompleteCondition[i]);
                    condition->set_iscomplete(false);
                    condition->set_progress(0);
                }

                *push.add_newtasks() = newTask;

                s_PlayerTaskDataMap[account].insert({id, newTask});
                isNewTaskAdd = true;
            }
        }
    }
    
    if (isNewTaskAdd)
    {
        g_PlayerManager->SendProtobufToPlayer(account, eServerID::NewTaskPush, push);
    }

    SaveTaskData(account);
}

bool TaskManager::Destroy(Api::iEngine* const engine) {
    return true;
}
