using System;
using UnityEngine;
using XEngine;
using XGame;
using static GameUnitApi;
using System.Collections.Generic;

public interface IShowFXManager : IModule
{
    public void ShowFX(ulong Guid, ulong buffGuid, Int64 fxconfigID, string parentName, float duration = 2f);//显示特效在某个单位的某个节点上
    public void ClearFX(ulong Guid, ulong buffGuid, string parentName);//清除特效
    public void ShowBulletStartFX(GameObject bullet, Int64 bulletConfigID);//显示子弹拖尾特效

    public void ShowSkillChargeFX(UInt64 guid, Int64 skillId, bool loop = false);//显示技能蓄力特效
    public void ShowSkillMoveFX(UInt64 guid, Int64 skillId, bool loop = false);//显示技能移动特效
    public void DeletSkillFX(UInt64 guid);//删除技能特效
}
public class ShowFXManager : IShowFXManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IShowFXManager, ShowFXManager>();
    }
    IClient _Client;

    private Dictionary<string, GameObject> _FXPrefabs = new Dictionary<string, GameObject>(); //缓存特效预制体
    private Dictionary<string, Queue<GameObject>> _FXPools = new Dictionary<string, Queue<GameObject>>(); //按特效类型分类的对象池
    private Dictionary<ulong, List<GameObject>> _BuffFXMapping = new Dictionary<ulong, List<GameObject>>(); //记录每个buffGuid对应的特效对象


    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();

        return true;
    }


    public void Release()
    {
        _FXPrefabs.Clear();
    }

    public void ShowFX(ulong Guid, ulong fxitemGuid, Int64 fxconfigID, string parentName, float duration = 2f)//显示特效在某个单位的某个节点上
    {
        if (!AutoConfig.SpecialEffects.Datas.ContainsKey(fxconfigID))
        {
            XApi.Debug($"ShowFX {fxconfigID} 不存在于SpecialEffects配置中");
            return;
        }
        IUnit unit = GameUnitApi.QueryUnit(Guid);
        if (null == unit)
        {
            XApi.Debug($"ShowFX {Guid}{parentName} ,GUID单位不存在");
            return;
        }

        Transform parentNode = unit.transform.Find(parentName);
        if (null == parentNode)
        {
            XApi.Debug($"ShowFX {Guid} {parentName} ,父节点不存在");
            return;
        }
        Int64 fxid = AutoConfig.SpecialEffects.Get(fxconfigID)._ID;

        GameObject fxObject = CreateFXObject(fxid.ToString(), fxconfigID, parentNode, fxitemGuid);
        if (fxObject != null)
        {
            fxObject.transform.position = parentNode.position;
        }

    }

    GameObject CreateFXObject(string fxname, Int64 fxconfigID, Transform parent, ulong Guid)//加载特效预制体
    {
        GameObject fxPrefab = null;

        if (!_FXPrefabs.ContainsKey(fxname))
        {
            _FXPrefabs[fxname] = Resources.Load<GameObject>(AutoConfig.SpecialEffects.Get(fxconfigID)._Prefab);
        }

        fxPrefab = _FXPrefabs[fxname];

        if (fxPrefab != null)
        {
            GameObject fxObject = GameObject.Instantiate(fxPrefab, parent);
            Renderer renderers = fxObject.GetComponentInChildren<Renderer>();
            renderers.sortingOrder = 15;
            if (!_BuffFXMapping.ContainsKey(Guid))
            {
                _BuffFXMapping[Guid] = new List<GameObject>();
            }
            _BuffFXMapping[Guid].Add(fxObject);

            return fxObject;
        }
        else
        {
            XApi.Debug($"无法加载特效预制体: Effects/{fxname}");
            return null;
        }
    }
    public void ClearFX(ulong Guid, ulong buffGuid, string parentName)//删除buff特效
    {
        IUnit unit = GameUnitApi.QueryUnit(Guid);
        if (null == unit)
        {
            XApi.Debug($"ClearFX {Guid} {parentName} ,GUID单位不存在");
            return;
        }

        Transform parentNode = unit.transform.Find(parentName);
        if (null == parentNode)
        {
            XApi.Debug($"ClearFX {Guid} {parentName} ,父节点不存在");
            Debug.LogError($"ClearFX {Guid} {parentName} ,父节点不存在");
            return;
        }

        if (_BuffFXMapping.ContainsKey(buffGuid))
        {
            foreach (GameObject fxObject in _BuffFXMapping[buffGuid])
            {
                if (fxObject != null)
                {
                    GameObject.Destroy(fxObject);
                }
            }
            _BuffFXMapping.Remove(buffGuid);
        }
        // foreach (Transform child in parentNode)
        // {
        //     GameObject.Destroy(child.gameObject);
        // }
    }

    public void ShowBulletStartFX(GameObject bullet, Int64 bulletConfigID)//子弹拖尾特效
    {
        Int64 fxconfigID = AutoConfig.Bullet.Get(bulletConfigID)._TrailerEffect;
        if (fxconfigID == 0)
        {
            return;
        }
        AddShowFX(bullet, fxconfigID);
    }

    public void ShowMoveSkillFX(GameObject character, Int64 skillId)//位移技能拖尾特效
    {
        Int64 fxconfigID = AutoConfig.Move.Get(skillId)._MoveEffect;
        if (fxconfigID == 0)
        {
            return;
        }
        AddShowFX(character, fxconfigID);
    }

    private void AddShowFX(GameObject obj, Int64 fxconfigID)//显示挂载特效
    {
        GameObject fxObject = null;
        GameObject fxPrefab = Resources.Load<GameObject>(AutoConfig.SpecialEffects.Get(fxconfigID)._Prefab);
        if (fxPrefab != null)
        {
            fxPrefab.GetComponentInChildren<Renderer>().sortingOrder = 15;
            fxObject = GameObject.Instantiate(fxPrefab, obj.transform.position, obj.transform.rotation);
        }
        if (fxObject != null)
        {
            fxObject.transform.parent = obj.transform;
        }
    }

    public void ShowSkillChargeFX(UInt64 guid, Int64 skillId, bool loop = false)//显示技能蓄力特效
    {
        GameObject fxObject = null;
        IUnit skillUnit = GameUnitApi.QueryUnit(guid);
        if (null == skillUnit)
        {
            XApi.Debug($"ShowSkillFX {guid} {skillId} ,GUID单位不存在");
            return;
        }
        Transform parentNode = skillUnit.transform.Find("SkillFX");
        if (null == parentNode)
        {
            XApi.Debug($"ShowSkillChargeFX {guid} {skillId} ,父节点不存在");
            return;
        }
        GameObject fxPrefab = null;
        GameObject skillcharacter = skillUnit.gameObject;
        if (skillUnit.Type == GameUnit.eUnitType.Character)
        {
            if (AutoConfig.Skill.Get(skillId)._SpecialEffects == 0)
            {
                return;
            }

        }
        if (skillUnit.Type == GameUnit.eUnitType.Monster)
        {
            if (AutoConfig.MonsterSkill.Get(skillId)._SpecialEffects == 0)
            {
                return;
            }
        }

        if (skillUnit.Type == GameUnit.eUnitType.Character)
        {
            fxPrefab = Resources.Load<GameObject>(AutoConfig.SpecialEffects.Get(AutoConfig.Skill.Get(skillId)._SpecialEffects)._Prefab);
        }
        else if (skillUnit.Type == GameUnit.eUnitType.Monster)
        {
            fxPrefab = Resources.Load<GameObject>(AutoConfig.SpecialEffects.Get(AutoConfig.MonsterSkill.Get(skillId)._SpecialEffects)._Prefab);
        }
        
        if (fxPrefab != null)
        {
            fxPrefab.GetComponentInChildren<Renderer>().sortingOrder = 15;
            fxObject = GameObject.Instantiate(fxPrefab, parentNode);
            //Debug.LogError($"技能特效: {fxPrefab.name}创建在{parentNode.name}");
            fxObject.transform.localPosition = Vector3.zero;
            if (skillUnit.Angle > 90 && skillUnit.Angle < 270) // 角色面向左侧
            {
                Vector3 localScale = fxObject.transform.localScale;
                localScale.x = Mathf.Abs(localScale.x) * -1; // 水平翻转
                fxObject.transform.localScale = localScale;
            }
            else // 角色面向右侧或其他方向
            {
                Vector3 localScale = fxObject.transform.localScale;
                localScale.x = Mathf.Abs(localScale.x); // 保持正常方向
                fxObject.transform.localScale = localScale;
            }

            if (loop)
            {//循环播放
                ParticleSystem[] particleSystems = fxObject.GetComponentsInChildren<ParticleSystem>();
                foreach (ParticleSystem ps in particleSystems)
                {
                    var main = ps.main;
                    main.loop = true;
                }
            }
            //Debug.LogError($"ShowSkillChargeFX  {skillId} ,特效显示成功");
        }
    }
    public void ShowSkillMoveFX(UInt64 guid, Int64 skillId, bool loop = false)//显示技能移动特效
    {
        GameObject fxObject = null;
        IUnit skillUnit = GameUnitApi.QueryUnit(guid);
        if (null == skillUnit)
        {
            XApi.Debug($"ShowSkillFX {guid} {skillId} ,GUID单位不存在");
            return;
        }
        Transform parentNode = skillUnit.transform.Find("SkillFX");
        if (null == parentNode)
        {
            XApi.Debug($"ShowSkillChargeFX {guid} {skillId} ,父节点不存在");
            return;
        }

        GameObject skillcharacter = skillUnit.gameObject;
        GameObject fxPrefab = Resources.Load<GameObject>(AutoConfig.SpecialEffects.Get(AutoConfig.Move.Get(skillId)._MoveEffect)._Prefab);
        if (fxPrefab != null)
        {
            fxPrefab.GetComponentInChildren<Renderer>().sortingOrder = 15;
            fxObject = GameObject.Instantiate(fxPrefab, parentNode);
            fxObject.transform.localPosition = Vector3.zero;

            if (loop)
            {//循环播放
                ParticleSystem[] particleSystems = fxObject.GetComponentsInChildren<ParticleSystem>();
                foreach (ParticleSystem ps in particleSystems)
                {
                    var main = ps.main;
                    main.loop = true;
                }
            }
        }
    }

    public void DeletSkillFX(UInt64 guid)//删除技能特效
    {
        IUnit skillUnit = GameUnitApi.QueryUnit(guid);
        if (null == skillUnit)
        {
            XApi.Debug($"ShowSkillFX {guid},GUID单位不存在");
            return;
        }
        Transform parentNode = skillUnit.transform.Find("SkillFX");
        if (null == parentNode)
        {
            XApi.Debug($"ShowSkillChargeFX {guid},父节点不存在");
            return;
        }
        foreach (Transform child in parentNode)
        {
            GameObject.Destroy(child.gameObject);
        }
    }
}


