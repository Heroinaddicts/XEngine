using System;
using System.Collections.Generic;
using AutoConfig;
using GameUnit;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.EventSystems;
using UnityEngine.InputSystem;
using UnityEngine.UI;
using XEngine;
using XGame;
using XUtils;
using static AutoConfig.AutoEnum;
using static GameUnitApi;
using static UIJoyStick;

class UINormalAttack : MonoBehaviour, IPointerDownHandler, IPointerUpHandler//,IDragHandler, IBeginDragHandler, IEndDragHandler
{

    public enum eEvent
    {
        BtnSkillClickDown = 0,
        BtnSkillClickDrag = 1,
        BtnSkillClickUp = 2,
        BtnSkillClickCancel = 3
    }
    enum SkillReleaseMode
    {
        None,//无技能
        TargetSelection,//选择技能目标
        DragPlacement,//技能拖放
        Instant//技能立即释放
    }
    SkillReleaseMode _CurrentSkillReleaseMode = SkillReleaseMode.None;
    static XEventPool<eEvent> s_EventPool = new XEventPool<eEvent>();

    IUISkillManager _UISkillManager;
    ISkillManager _SkillManager;
    AutoConfig.Skill _NormalAtkConfig;
    ICharacterManager _CharacterManager;
    IDamageManager _DamageManager;
    SkillVisual.ISkillVisual _SkillPreviewVisual = null;
    float _SkillDragAngle = 0f;//技能拖动角度
    private Vector3 _skillOriginPos;
    bool _IsLongPress = false;
    bool _IsSkillReleasing = false;
    private float _skillDragMaxRange = 5f;
    private EventTrigger _SkillSlotEventTrigger;
    Vector2 _SkillStartPosition;
    bool _IsDragSkill = false;
    float _DragSkillDeadZone = 0f;//拖动技能的死区
    Int64 _ActiveSkillConfigID = 0;

    Vector3 _FinalCastPos = Vector3.zero;//最终释放位置
    [Header("技能取消区域")]
    public SkillCancelArea _SkillCancelArea;

    private string _PressedSkillSlot = null;
    private bool _IsDraggingToCancel = false;


    private Dictionary<UInt64, Int64> _CharacterLastAttackConfigID = new Dictionary<UInt64, Int64>();//角色最后一次攻击的技能配置ID
    private Dictionary<UInt64, Int64> _CharacterLastAttackTime = new Dictionary<UInt64, Int64>();//角色最后一次攻击的时间

    private void Awake()
    {
        _UISkillManager = Launcher.GetModule<IUISkillManager>();
        _SkillManager = Launcher.GetModule<ISkillManager>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _DamageManager = Launcher.GetModule<IDamageManager>();
    }

    float _LastAngle;
    public void OnPointerDown(PointerEventData eventData)
    {
        Debug.LogError("点击了普通攻击按钮");
        _PressedSkillSlot = "NormalAttack";
        _IsLongPress = false;
        _IsDraggingToCancel = false;

        IUnit character = _CharacterManager.GetMainCharacterUnit();
        if (character == null) return;
        Int64 lastAttackConfigID = GetCharacterLastAttackConfigID(character.Guid);
        Int64 lastAttackTime = GetCharacterLastAttackTime(character.Guid);
        Int64 nextAttackconfigID = 0;
        if (lastAttackConfigID == 0 || lastAttackConfigID == AutoConfig.Character.Get(character.ConfigID)._Atk3)
        {
            
        }
        else if (lastAttackConfigID == AutoConfig.Character.Get(character.ConfigID)._Atk1)
        {
            nextAttackconfigID = AutoConfig.Character.Get(character.ConfigID)._Atk2;
        }
        else if (lastAttackConfigID == AutoConfig.Character.Get(character.ConfigID)._Atk2)
        {
            nextAttackconfigID = AutoConfig.Character.Get(character.ConfigID)._Atk3;
        }


        if (lastAttackConfigID == 0 || lastAttackConfigID == AutoConfig.Character.Get(character.ConfigID)._Atk3)//第三段完成和刚开始的时候
        {
            nextAttackconfigID = AutoConfig.Character.Get(character.ConfigID)._Atk1;
        }
        else if (lastAttackConfigID == AutoConfig.Character.Get(character.ConfigID)._Atk1)//完成第一段准备进入第二段的时候
        {
            if (SafeSystem.Time.GetMillisecondTick() - lastAttackTime <= AutoConfig.Character.Get(character.ConfigID)._AtkInteval)
            {
                nextAttackconfigID = AutoConfig.Character.Get(character.ConfigID)._Atk2;
            }
            else
            {
                nextAttackconfigID = AutoConfig.Character.Get(character.ConfigID)._Atk1;
            }
        }
        else if (lastAttackConfigID == AutoConfig.Character.Get(character.ConfigID)._Atk2)//完成第二段准备进入第三段的时候
        {
            if (SafeSystem.Time.GetMillisecondTick() - lastAttackTime <= AutoConfig.Character.Get(character.ConfigID)._AtkInteval)
            {
                nextAttackconfigID = AutoConfig.Character.Get(character.ConfigID)._Atk3;
            }
            else
            {
                nextAttackconfigID = AutoConfig.Character.Get(character.ConfigID)._Atk1;
            }
        }
        _ActiveSkillConfigID = nextAttackconfigID;
        //Debug.LogError("技能释放：" + nextAttackconfigID + "  上一次攻击的技能配置ID：" + lastAttackConfigID);

        if (AutoConfig.Skill.Get(nextAttackconfigID)._SelectedType == AutoEnum.eSelectedType.UnAim)
        {
            Debug.LogError($"普通攻击走面片逻辑，技能配置ID{nextAttackconfigID}");
            OnAttackButtonPress(nextAttackconfigID);
        }
        else if (AutoConfig.Skill.Get(nextAttackconfigID)._SelectedType == AutoEnum.eSelectedType.Liberty)
        {
            //Debug.LogError($"普通攻击立刻释放，技能配置ID{nextAttackconfigID}");
            ClickNormalAttackBtn();
        }
    }
    public void OnAttackButtonPress(Int64 nextAttackconfigID)
    {
        _IsLongPress = false;//重置长按
        if (_CurrentSkillReleaseMode == SkillReleaseMode.TargetSelection)
        {
            IUnit lastDamageUnit = _DamageManager.GetLastDamageUnit();
            IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();

            if (lastDamageUnit != null)
            {
                Vector3 direction = lastDamageUnit.Position - mainCharacter.Position;
                if (direction.magnitude <= AutoConfig.Skill.Get(nextAttackconfigID)._MaxRangeRadius)
                {
                    ExecuteSkillWithTarget(lastDamageUnit.Guid);
                }
                else
                {
                    XApi.Debug($"Skill {nextAttackconfigID} out of range");
                    Debug.LogError($"目标{lastDamageUnit.Guid}超出了技能{nextAttackconfigID}的最大范围{AutoConfig.Skill.Get(nextAttackconfigID)._MaxRangeRadius}");
                }
            }
            else
            {
                return;
            }
            return;
        }
        ClickAttackBtn(nextAttackconfigID);

        XApi.TimerApi.StartTimer(OnLongPressTimer, _CharacterManager.GetMainCharacterUnit(), nextAttackconfigID, (int)AutoConfig.Skill.Get(nextAttackconfigID)._ShowRangeTime, 1, 0);

    }

    private void ClickAttackBtn(long nextAttackconfigID)
    {
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        if (null == mainCharacterUnit)
        {
            Debug.LogWarning($"找不到主角色");
            return;
        }
        Int64 skillConfigId = nextAttackconfigID;//得到技能configID
        AutoConfig.Skill skillConfig = AutoConfig.Skill.Get(skillConfigId);//得到技能

        StartSkillRelease(skillConfigId, skillConfig);
    }

    private void OnLongPressTimer(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
    {
        Int64 skillConfigId = (Int64)context;
        if (state == XApi.ITimer.eState.END && context != null)
        {
            _IsLongPress = true;
            GameObject skillRange = Resources.Load<GameObject>("UI/Sprites/Skill/Ring");//加载技能范围
            IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
            GameObject skillRangeParent = mainCharacterUnit.transform.Find("SkillRange").gameObject;//找到主角色的技能范围父物体
            if (skillRangeParent != null)
            {
                for (int i = 0; i < skillRangeParent.transform.childCount; i++)
                {
                    GameObject.Destroy(skillRangeParent.transform.GetChild(i).gameObject);
                }
            }
            GameObject skillRangeInstance = GameObject.Instantiate(skillRange);
            skillRangeInstance.transform.SetParent(skillRangeParent.transform);//设置技能范围的父物体为主角色的技能范围
            skillRangeInstance.transform.localPosition = Vector3.zero;
            Vector3 scale = skillRangeInstance.transform.localScale;
            scale.x = AutoConfig.Skill.Get(skillConfigId)._MaxRangeRadius;
            scale.y = AutoConfig.Skill.Get(skillConfigId)._MaxRangeRadius;
            skillRangeInstance.transform.localScale = scale;
        }
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
    }
    private void StartSkillRelease(long skillConfigId, AutoConfig.Skill skillConfig)//技能释放开始
    {
        if (_IsSkillReleasing) return;
        _IsSkillReleasing = true;
        if (_CurrentSkillReleaseMode != SkillReleaseMode.None)
        {
            CancelSkillRelease();
        }
        _ActiveSkillConfigID = skillConfigId;
        AutoEnum.eSelectedType currentskilltype = AutoConfig.Skill.Get(skillConfigId)._SelectedType;

        if (_IsSkillReleasing == false)
        {
            Debug.LogWarning("技能释放取消");
            return;
        }
        switch (currentskilltype)
        {
            case AutoEnum.eSelectedType.Aim://指向性（技能点击后必须选择了目标后，才能释放）

                _CurrentSkillReleaseMode = SkillReleaseMode.TargetSelection;
                //StartTargetSelectionMode(skillConfig);
                break;
            case AutoEnum.eSelectedType.Liberty://自由性（技能点击后立刻释放）
                _CurrentSkillReleaseMode = SkillReleaseMode.Instant;
                break;
            case AutoEnum.eSelectedType.UnAim://无指向性（技能点击后，选择面片，才能释放）
                _CurrentSkillReleaseMode = SkillReleaseMode.DragPlacement;
                StartDragPlacementMode(skillConfig);
                break;
            default:
                break;
        }
    }

    public void OnPointerUp(PointerEventData eventData)
    {
        //Debug.LogError("普通攻击抬起");
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        GameObject exitBtn = canvas.transform.Find("ExitSkillBtn").gameObject;
        if (exitBtn != null)
        {
            Debug.LogError("普通攻击抬起按钮关闭");
            exitBtn.SetActive(false);
        }

        // 检查是否滑动到取消区域
        if (_SkillCancelArea._IsDraggingToCancel)
        {
            // 取消技能释放
            CancelCurrentSkill();
        }
        else
        {
            IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
            if (_CurrentSkillReleaseMode == SkillReleaseMode.DragPlacement)
            {
                float endangle = 0;
                Vector2 endPos = Vector2.zero;

                if (mainCharacterUnit == null)
                {
                    CancelSkillRelease();
                    return;
                }

                if (_IsDragSkill == false)
                {
                    // 没有拖拽，使用默认逻辑或最近目标
                    IUnit lastDamageUnit = _DamageManager.GetLastDamageUnit();
                    if (lastDamageUnit == null)
                    {
                        IUnit nowAttackUnit = GetCanAttackUint(_ActiveSkillConfigID);
                        if (nowAttackUnit != null)
                        {
                            endPos = new Vector2(nowAttackUnit.Position.x, nowAttackUnit.Position.y);
                            _SkillDragAngle = Mathf.Atan2(nowAttackUnit.Position.y - mainCharacterUnit.Position.y, nowAttackUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                            Debug.LogWarning($"以技能范围内最近的单位为目标");
                        }
                        else
                        {
                            endPos = new Vector2(mainCharacterUnit.Position.x, mainCharacterUnit.Position.y);
                            _SkillDragAngle = mainCharacterUnit.Angle;
                            Debug.LogWarning("无目标，使用自身位置和角度");
                        }
                    }
                    else
                    {
                        float distance = Vector2.Distance(mainCharacterUnit.Position, lastDamageUnit.Position);
                        if (distance <= AutoConfig.Skill.Get(_ActiveSkillConfigID)._MaxSkillDistacne)
                        {
                            endPos = new Vector2(lastDamageUnit.Position.x, lastDamageUnit.Position.y);
                            _SkillDragAngle = Mathf.Atan2(lastDamageUnit.Position.y - mainCharacterUnit.Position.y, lastDamageUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                            Debug.LogWarning($"技能距离满足，目标位置：{endPos}，角度：{_SkillDragAngle}");
                        }
                        else
                        {
                            IUnit nowAttackUnit = GetCanAttackUint(_ActiveSkillConfigID);
                            if (nowAttackUnit != null)
                            {
                                endPos = new Vector2(nowAttackUnit.Position.x, nowAttackUnit.Position.y);
                                _SkillDragAngle = Mathf.Atan2(nowAttackUnit.Position.y - mainCharacterUnit.Position.y, nowAttackUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                                Debug.LogWarning($"以技能范围内最近的单位为目标");
                            }
                            else
                            {
                                _SkillDragAngle = mainCharacterUnit.Angle;
                                endPos = new Vector2(mainCharacterUnit.Position.x, mainCharacterUnit.Position.y);
                                Debug.LogWarning("技能距离不满足，使用自身位置和角度");
                            }
                        }
                    }
                }
                else
                {
                    // 结束拖拽，使用拖拽结束的位置
                    Debug.LogWarning("结束拖动");
                    endPos = new Vector2(_FinalCastPos.x, _FinalCastPos.y);
                }

                oVector2 targetpos = new oVector2
                {
                    X = endPos.x,
                    Y = endPos.y
                };

                Vector2 dragDistance = endPos - new Vector2(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y);
                if (dragDistance.magnitude >= _DragSkillDeadZone)
                {
                    endangle = _SkillDragAngle;
                }

                Vector2 characterPos = mainCharacterUnit.Position;
                oVector2 castPos = new oVector2
                {
                    X = characterPos.x,
                    Y = characterPos.y
                };

                // 使用普通攻击的技能配置释放技能
                _SkillManager.UseSkill(0, _ActiveSkillConfigID, endangle, targetpos, 0, castPos);


                Debug.LogWarning($"普通攻击拖动结束，拖动距离：{dragDistance.magnitude}，拖动角度：{endangle}，目标位置：{targetpos}，技能ID：{_ActiveSkillConfigID}");
            }
            _PressedSkillSlot = null;
            _IsDraggingToCancel = false;
            _IsLongPress = false;
            _SkillCancelArea.ResetCancelState();
            CancelSkillRelease();
        }
    }


    void ClickNormalAttackBtn()
    {
        IUnit character = _CharacterManager.GetMainCharacterUnit();
        if (character == null) return;
        Int64 characterID = character.ConfigID;
        UInt64 characterGuid = character.Guid;
        Vector2 characterPos = character.Position;
        oVector2 castPos = new oVector2
        {
            X = characterPos.x,
            Y = characterPos.y
        };

        Int64 lastAttackConfigID = GetCharacterLastAttackConfigID(characterGuid);
        Int64 lastAttackTime = GetCharacterLastAttackTime(characterGuid);
        if (lastAttackConfigID == 0 || lastAttackConfigID == AutoConfig.Character.Get(characterID)._Atk3)//第三段完成和刚开始的时候
        {
            //SetCharacterLastAttackConfigID(characterGuid, AutoConfig.Character.Get(characterID)._Atk1);
            Vector2 pos = new Vector2(0, 0);
            oVector2 targetpos = new oVector2
            {
                X = pos.x,
                Y = pos.y
            };

            _SkillManager.UseSkill(0, AutoConfig.Character.Get(characterID)._Atk1, character.Angle, targetpos, 0, castPos);

        }
        else if (lastAttackConfigID == AutoConfig.Character.Get(characterID)._Atk1)//完成第一段准备进入第二段的时候
        {
            if (SafeSystem.Time.GetMillisecondTick() - lastAttackTime <= AutoConfig.Character.Get(characterID)._AtkInteval)
            {
                Vector2 pos = new Vector2(0, 0);
                oVector2 targetpos = new oVector2
                {
                    X = pos.x,
                    Y = pos.y
                };
                //SetCharacterLastAttackConfigID(characterGuid, AutoConfig.Character.Get(characterID)._Atk2);
                _SkillManager.UseSkill(0, AutoConfig.Character.Get(characterID)._Atk2, character.Angle, targetpos, 0, castPos);
            }
            else
            {
                //Debug.LogError($"普通攻击间隔超时，当前时间：{SafeSystem.Time.GetMillisecondTick()}，攻击间隔：{AutoConfig.Character.Get(characterID)._AtkInteval}，差值：{SafeSystem.Time.GetMillisecondTick() - _LastAttackTime}");
                Vector2 pos = new Vector2(0, 0);
                oVector2 targetpos = new oVector2
                {
                    X = pos.x,
                    Y = pos.y
                };
                //SetCharacterLastAttackConfigID(characterGuid, AutoConfig.Character.Get(characterID)._Atk1);
                _SkillManager.UseSkill(0, AutoConfig.Character.Get(characterID)._Atk1, character.Angle, targetpos, 0, castPos);
            }
        }

        else if (lastAttackConfigID == AutoConfig.Character.Get(characterID)._Atk2)//完成第二段准备进入第三段的时候
        {
            if (SafeSystem.Time.GetMillisecondTick() - lastAttackTime <= AutoConfig.Character.Get(characterID)._AtkInteval)
            {
                Vector2 pos = new Vector2(0, 0);
                oVector2 targetpos = new oVector2
                {
                    X = pos.x,
                    Y = pos.y
                };
                //SetCharacterLastAttackConfigID(characterGuid, AutoConfig.Character.Get(characterID)._Atk3);
                _SkillManager.UseSkill(0, AutoConfig.Character.Get(characterID)._Atk3, character.Angle, targetpos, 0, castPos);
            }
            else
            {
                Vector2 pos = new Vector2(0, 0);
                oVector2 targetpos = new oVector2
                {
                    X = pos.x,
                    Y = pos.y
                };
                //SetCharacterLastAttackConfigID(characterGuid, AutoConfig.Character.Get(characterID)._Atk1);
                _SkillManager.UseSkill(0, AutoConfig.Character.Get(characterID)._Atk1, character.Angle, targetpos, 0, castPos);
            }
        }
        //Debug.LogWarning("点击了普通攻击按钮" + "时间戳是" + SafeSystem.Time.GetMillisecondTick());
        //Debug.LogError("完成普通攻击走新独立逻辑");
    }

    private Int64 GetCharacterLastAttackConfigID(UInt64 characterGuid)
    {
        if (_CharacterLastAttackConfigID.ContainsKey(characterGuid))
        {
            return _CharacterLastAttackConfigID[characterGuid];
        }
        return 0;
    }

    // 设置指定角色的上次攻击配置ID
    private void SetCharacterLastAttackConfigID(UInt64 characterGuid, Int64 configID)
    {
        _CharacterLastAttackConfigID[characterGuid] = configID;
    }

    // 获取指定角色的上次攻击时间
    private Int64 GetCharacterLastAttackTime(UInt64 characterGuid)
    {
        if (_CharacterLastAttackTime.ContainsKey(characterGuid))
        {
            return _CharacterLastAttackTime[characterGuid];
        }
        return 0;
    }

    // 设置指定角色的上次攻击时间
    private void SetCharacterLastAttackTime(UInt64 characterGuid, Int64 time)
    {
        _CharacterLastAttackTime[characterGuid] = time;
    }

    public void ChangeAtkInfo(UInt64 playerGuid, Int64 configID, Int64 timetick)
    {
        //Debug.LogError($"写入攻击信息：{configID}，{timetick}");
        SetCharacterLastAttackConfigID(playerGuid, configID);
        SetCharacterLastAttackTime(playerGuid, timetick);
    }


    private void StartDragPlacementMode(AutoConfig.Skill skillConfig)//技能释放模式-拖拽placement
    {
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        if (_IsSkillReleasing == false)
        {
            CancelSkillRelease();
            return;
        }
        IUnit testMainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        if (testMainCharacterUnit != null)
        {
            _skillOriginPos = testMainCharacterUnit.transform.position;
        }
        _skillDragMaxRange = AutoConfig.Skill.Get(skillConfig._ID)._MaxRangeRadius;

        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        GameObject exitBtn = canvas.transform.Find("ExitSkillBtn").gameObject;
        if (exitBtn != null)
        {
            exitBtn.SetActive(true);
            Button exitBtnBtn = exitBtn.GetComponent<Button>();
            if (exitBtnBtn != null)
            {
                exitBtnBtn.onClick.RemoveAllListeners();
                exitBtnBtn.onClick.AddListener(() =>
                {
                    CancelSkillRelease();
                });
            }
        }
        Int64 skillConfigId = skillConfig._ID;
        _SkillCancelArea._PressedSkillSlot = _PressedSkillSlot;

        //显示面片
        AutoEnum.eSheetsType sheetsType = AutoConfig.Skill.Get(skillConfigId)._SheetsType;
        float[] sheetsParameter = AutoConfig.Skill.Get(skillConfigId)._RangeParameter;
        switch (sheetsType)
        {
            case AutoEnum.eSheetsType.Circle://圆形，半径
                _SkillPreviewVisual = SkillVisual.Api.CreateCircleVisual(sheetsParameter[0]);
                Debug.LogWarning($"创建圆形技能预览，半径{sheetsParameter[0]}");
                if (_SkillPreviewVisual != null)
                {
                    _SkillPreviewVisual.GetComponent<MeshRenderer>().material = Resources.Load<UnityEngine.Material>("UI/Sprites/Skill/WorldPos");
                }
                break;
            case AutoEnum.eSheetsType.Rectangle://矩形，长宽
                _SkillPreviewVisual = SkillVisual.Api.CreateRectangleVisual(sheetsParameter[0], sheetsParameter[1]);
                Debug.LogWarning($"创建矩形技能预览，长{sheetsParameter[0]}，宽{sheetsParameter[1]}");
                if (_SkillPreviewVisual != null)
                {
                    MeshFilter meshFilter = _SkillPreviewVisual.GetComponent<MeshFilter>();
                    if (meshFilter != null && meshFilter.mesh != null)
                    {
                        Vector3[] vertices = meshFilter.mesh.vertices;
                        float bottomEdgeZOffset = Mathf.Abs(vertices[0].z);
                        // 所有顶点Z轴+偏移量，让底部边中心对齐原点
                        for (int i = 0; i < vertices.Length; i++)
                        {
                            vertices[i].z += bottomEdgeZOffset;
                        }
                        meshFilter.mesh.vertices = vertices;
                        meshFilter.mesh.RecalculateBounds();
                        _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);


                    }
                    if (_SkillPreviewVisual != null)
                    {
                        _SkillPreviewVisual.GetComponent<MeshRenderer>().material = Resources.Load<UnityEngine.Material>("UI/Sprites/Skill/Arrow");

                    }
                }
                break;
            case AutoEnum.eSheetsType.sector://扇形，半径和角度
                Debug.LogWarning($"创建扇形技能预览，半径{sheetsParameter[0]}，角度{sheetsParameter[1]}");
                _SkillPreviewVisual = SkillVisual.Api.CreateSectorVisual(sheetsParameter[0], sheetsParameter[1]);
                break;
        }

        if (_SkillPreviewVisual == null)
        {
            Debug.LogWarning("技能预览创建失败");
            CancelSkillRelease();
            return;
        }
        //判定是不是固定位置,是固定位置就只显示面片
        if (AutoConfig.Skill.Get(skillConfigId)._CanSeparated == false)
        {
            //IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
            GameObject parent = mainCharacterUnit.transform.Find("PlacementLab").gameObject;
            if (parent != null)
            {
                for (int i = 0; i < parent.transform.childCount; i++)
                {
                    GameObject.Destroy(parent.transform.GetChild(i).gameObject);
                }
            }
            _SkillPreviewVisual.transform.SetParent(parent.transform);//设置技能范围的父物体为主角色的技能范围
            _SkillPreviewVisual.transform.localPosition = Vector3.zero;
            _SkillPreviewVisual.transform.localScale = Vector3.one;
            _SkillPreviewVisual.transform.localRotation = Quaternion.Euler(90, 0, 0);


        }
        else
        {
            //非固定位置则显示技能范围
            GameObject skillRange = Resources.Load<GameObject>("UI/Sprites/Skill/Ring");//加载技能范围
            GameObject skillRangeParent = mainCharacterUnit.transform.Find("SkillRange").gameObject;//找到主角色的技能范围父物体
            if (skillRangeParent != null)
            {
                for (int i = 0; i < skillRangeParent.transform.childCount; i++)
                {
                    GameObject.Destroy(skillRangeParent.transform.GetChild(i).gameObject);
                }
            }
            GameObject skillRangeInstance = GameObject.Instantiate(skillRange);
            skillRangeInstance.transform.SetParent(skillRangeParent.transform);//设置技能范围的父物体为主角色的技能范围
            skillRangeInstance.transform.localPosition = Vector3.zero;
            Vector3 scale = skillRangeInstance.transform.localScale;
            scale.x = AutoConfig.Skill.Get(skillConfigId)._MaxRangeRadius;
            scale.y = AutoConfig.Skill.Get(skillConfigId)._MaxRangeRadius;
            skillRangeInstance.transform.localScale = scale;

            GameObject parent = mainCharacterUnit.transform.Find("PlacementLab").gameObject;
            _SkillPreviewVisual.transform.SetParent(parent.transform);//设置技能范围的父物体为主角色的Placement
            _SkillPreviewVisual.transform.localPosition = Vector3.zero;
            _SkillPreviewVisual.transform.localScale = Vector3.one;
            _SkillPreviewVisual.transform.localRotation = Quaternion.Euler(90, 0, 0);
            _SkillPreviewVisual.transform.Rotate(0, -90, +60, Space.Self);


        }
        MeshRenderer meshRenderer = _SkillPreviewVisual.transform.GetComponent<MeshRenderer>();
        if (meshRenderer != null)
        {
            meshRenderer.sortingOrder = 9;
        }

        GameObject attackBtn = GameObjectCommon.GetGameObjectByPath("Canvas/UIGameController/UIJoyButton/UIAttack");
        EventTrigger trigger = attackBtn.GetComponent<EventTrigger>();
        if (trigger == null)
        {
            trigger = attackBtn.AddComponent<EventTrigger>();
        }

        _SkillSlotEventTrigger = trigger;
        trigger.triggers.Clear();

        AddEventTriggerListener(trigger, EventTriggerType.BeginDrag, OnSkillDragBegin);
        AddEventTriggerListener(trigger, EventTriggerType.Drag, OnSkillDrag);
        AddEventTriggerListener(trigger, EventTriggerType.EndDrag, OnSkillDragEnd);
        //拖拽更新位置
        //松手释放技能

        IUnit lastattackUnit = _DamageManager.GetLastDamageUnit();
        if (lastattackUnit != null)
        {
            float distance = Vector2.Distance(mainCharacterUnit.Position, lastattackUnit.Position);
            if (distance <= AutoConfig.Skill.Get(skillConfigId)._MaxSkillDistacne)
            {
                if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(skillConfigId)._CanSeparated == true)
                {
                    if (lastattackUnit.transform != null) // 检查对象是否仍然有效
                    {
                        Vector3 targetPos3D = new Vector3(lastattackUnit.Position.x, lastattackUnit.Position.y, _SkillPreviewVisual.transform.position.z);
                        _SkillPreviewVisual.transform.position = targetPos3D;
                        //Debug.LogError($"面片移动，距离：{distance}，技能设定：{AutoConfig.Skill.Get(_ActiveSkillInfo.skillConfigID)._MaxSkillDistacne}");
                    }
                }
                else if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(skillConfigId)._CanSeparated == false)
                {
                    if (lastattackUnit.transform != null) // 检查对象是否仍然有效
                    {
                        _SkillDragAngle = Mathf.Atan2(lastattackUnit.Position.y - mainCharacterUnit.Position.y, lastattackUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                        _SkillDragAngle = (_SkillDragAngle + 360f) % 360f;
                        _SkillPreviewVisual.transform.eulerAngles = new Vector3(_SkillDragAngle, -90, -90);
                        _SkillPreviewVisual.transform.position = new Vector3(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y, 0);
                        _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);
                    }
                }
            }
            else
            {
                IUnit nowAttackUnit = GetCanAttackUint(skillConfigId);
                if (nowAttackUnit != null)
                {
                    if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(skillConfigId)._CanSeparated == true)
                    {
                        if (nowAttackUnit.transform != null) // 检查对象是否仍然有效
                        {
                            Vector3 targetPos3D = new Vector3(nowAttackUnit.Position.x, nowAttackUnit.Position.y, _SkillPreviewVisual.transform.position.z);
                            _SkillPreviewVisual.transform.position = targetPos3D;
                        }
                    }
                    else if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(skillConfigId)._CanSeparated == false)
                    {
                        if (nowAttackUnit.transform != null) // 检查对象是否仍然有效
                        {
                            _SkillDragAngle = Mathf.Atan2(nowAttackUnit.Position.y - mainCharacterUnit.Position.y, nowAttackUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                            _SkillDragAngle = (_SkillDragAngle + 360f) % 360f;
                            _SkillPreviewVisual.transform.eulerAngles = new Vector3(_SkillDragAngle, -90, -90);
                            _SkillPreviewVisual.transform.position = new Vector3(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y, 0);
                            _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);
                        }
                    }
                }

            }
        }
        else
        {
            IUnit nowAttackUnit = GetCanAttackUint(skillConfigId);
            if (nowAttackUnit != null)
            {
                if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(skillConfigId)._CanSeparated == true)
                {
                    if (nowAttackUnit.transform != null) // 检查对象是否仍然有效
                    {
                        Vector3 targetPos3D = new Vector3(nowAttackUnit.Position.x, nowAttackUnit.Position.y, _SkillPreviewVisual.transform.position.z);
                        _SkillPreviewVisual.transform.position = targetPos3D;
                    }
                }
                else if (_SkillPreviewVisual != null && AutoConfig.Skill.Get(skillConfigId)._CanSeparated == false)
                {
                    if (nowAttackUnit.transform != null) // 检查对象是否仍然有效
                    {
                        _SkillDragAngle = Mathf.Atan2(nowAttackUnit.Position.y - mainCharacterUnit.Position.y, nowAttackUnit.Position.x - mainCharacterUnit.Position.x) * Mathf.Rad2Deg;
                        _SkillDragAngle = (_SkillDragAngle + 360f) % 360f;
                        _SkillPreviewVisual.transform.eulerAngles = new Vector3(_SkillDragAngle, -90, -90);
                        _SkillPreviewVisual.transform.position = new Vector3(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y, 0);
                        _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);
                    }
                }
            }
        }
    }
    private void AddEventTriggerListener(EventTrigger trigger, EventTriggerType type, UnityAction<BaseEventData> action)
    {
        EventTrigger.Entry entry = new EventTrigger.Entry();
        entry.eventID = type;
        entry.callback.AddListener(action);
        trigger.triggers.Add(entry);
    }
    private void OnSkillDragBegin(BaseEventData data)
    {
        OnBeginDrag((PointerEventData)data);
    }
    private void OnSkillDrag(BaseEventData data)
    {
        OnDrag((PointerEventData)data);
    }
    private void OnSkillDragEnd(BaseEventData data)
    {
        OnEndDrag((PointerEventData)data);
    }
    public void OnBeginDrag(PointerEventData eventData)//技能拖拽开始
    {
        if (_CurrentSkillReleaseMode == SkillReleaseMode.DragPlacement)
        {
            _SkillStartPosition = eventData.position;
            _IsDragSkill = true;
            _IsSkillReleasing = true;
            _SkillDragAngle = 0f;
            _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);
        }
    }

    public void OnDrag(PointerEventData eventData)//技能拖拽更新位置
    {

        if (!_IsDragSkill || _SkillPreviewVisual == null)
        {
            return;
        }
        _SkillPreviewVisual.transform.localScale = new Vector3(1, 1, -1);
        Vector2 dragDistance = eventData.position - _SkillStartPosition;
        if (dragDistance.magnitude < _DragSkillDeadZone)
        {
            return;
        }

        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        float angle = Mathf.Atan2(dragDistance.y, dragDistance.x) * Mathf.Rad2Deg;
        angle = (angle + 360f) % 360f;
        _SkillDragAngle = angle;

        AutoEnum.eSheetsType sheetsType = AutoConfig.Skill.Get(_ActiveSkillConfigID)._SheetsType;
        bool canSeparated = AutoConfig.Skill.Get(_ActiveSkillConfigID)._CanSeparated;//是否固定位置
        if (!canSeparated)
        {
            _SkillPreviewVisual.transform.eulerAngles = new Vector3(angle, -90, -90);
            _SkillPreviewVisual.transform.position = new Vector3(mainCharacterUnit.transform.position.x, mainCharacterUnit.transform.position.y, 0);
            return;
        }

        float maxRange = AutoConfig.Skill.Get(_ActiveSkillConfigID)._MaxSkillDistacne;
        float changeDistance = Mathf.Min(dragDistance.magnitude / AutoConfig.Skill.Get(_ActiveSkillConfigID)._SheetsMove, maxRange);

        float deltaX = Mathf.Cos(angle * Mathf.Deg2Rad) * changeDistance;
        float deltaY = Mathf.Sin(angle * Mathf.Deg2Rad) * changeDistance;

        Vector3 currentPosition = mainCharacterUnit.transform.position;
        Vector3 targetpos = new Vector3(
            currentPosition.x + deltaX,
            currentPosition.y + deltaY,
            0
        );
        if (_SkillPreviewVisual == null) return;
        _SkillPreviewVisual.transform.position = targetpos;
        _FinalCastPos = _SkillPreviewVisual.transform.position; // 缓存最终施法世界坐标
    }

    public void OnEndDrag(PointerEventData eventData)//技能拖拽结束
    {
        CancelSkillRelease();
        ClearEventTriggerListeners();

    }
    public void CancelSkillRelease()//取消技能释放
    {
        _SkillDragAngle = 0f;
        _IsSkillReleasing = false;

        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        if (mainCharacterUnit != null)
        {
            XApi.TimerApi.StopTimer(OnLongPressTimer, mainCharacterUnit.gameObject);
            _IsLongPress = false;
            _IsDragSkill = false;

            Transform skillRangeTransform = mainCharacterUnit.transform.Find("SkillRange");
            if (skillRangeTransform != null)
            {
                GameObject skillRangeParent = skillRangeTransform.gameObject;//关闭技能范围显示
                if (skillRangeParent != null)
                {
                    for (int i = 0; i < skillRangeParent.transform.childCount; i++)
                    {
                        GameObject.Destroy(skillRangeParent.transform.GetChild(i).gameObject);
                    }
                }
            }

            GameObject parent = mainCharacterUnit.transform.Find("PlacementLab").gameObject;//关闭技能面片
            if (parent != null)
            {
                for (int i = 0; i < parent.transform.childCount; i++)
                {
                    GameObject.Destroy(parent.transform.GetChild(i).gameObject);
                }
            }
        }
        _ActiveSkillConfigID = 0;
        _CurrentSkillReleaseMode = SkillReleaseMode.None;
        _SkillPreviewVisual = null;
        ClearEventTriggerListeners();
        Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
        if (canvas != null)
        {
            GameObject exitBtn = canvas.transform.Find("ExitSkillBtn").gameObject;//关闭退出按钮
            if (exitBtn != null)
            {
                exitBtn.SetActive(false);
            }
        }
    }

    private void ClearEventTriggerListeners()
    {
        if (_SkillSlotEventTrigger != null)
        {
            // 移除所有事件监听器
            _SkillSlotEventTrigger.triggers.Clear();
            _SkillSlotEventTrigger = null;
        }
    }

    IUnit GetCanAttackUint(Int64 skillconfigID)//获取可攻击的Unit用于智能施法
    {
        List<IUnit> potentialTargets = new List<IUnit>();//可攻击的Unit列表
        AutoEnum.eTargetType[] targetType = AutoConfig.Skill.Get(skillconfigID)._TargetType;
        for (int i = 0; i < targetType.Length; i++)
        {
            eUnitType unitType = ChangeClickType(targetType[i]);//转换类型
            var units = GameUnitApi.GetUnitsByType(unitType);
            if (units != null)
            {
                foreach (var unit in units)
                {
                    if (unit.Value != null)
                    {
                        potentialTargets.Add(unit.Value);
                    }
                }
            }
        }

        if (potentialTargets.Count == 0)
        {
            Debug.LogWarning("没有可攻击的Unit");
            return null;
        }
        //此时获得所有可以使用此技能的Unit,进行距离内unit筛选
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        float skillDistance = AutoConfig.Skill.Get(skillconfigID)._MaxSkillDistacne;//技能最大距离
        List<IUnit> targetInRange = new List<IUnit>();
        if (mainCharacterUnit != null)
        {
            foreach (var unit in potentialTargets)
            {
                if (unit.GetBool(eEntityAttr.IsCantDamaged_Bool) == true)
                {
                    continue;//技能不能对这个单位造成伤害，跳过
                }

                float distance = Vector2.Distance(mainCharacterUnit.Position, unit.Position);

                if (distance <= skillDistance)
                {
                    targetInRange.Add(unit);
                    Debug.LogError($"Unit类型：{unit.Type}，Unit名字：{unit.ConfigID}，距离：{distance}，技能设定：{skillDistance}");
                }
            }
        }
        if (targetInRange.Count == 0)
        {
            Debug.LogError($"技能距离内没有可施法的目标");
            return null;
        }
        //获得技能范围内对应的可以使用技能的Unit，进行优先级判断
        List<IUnit> priorityTargets = new List<IUnit>();
        int minPriority = int.MaxValue;
        foreach (var unit in targetInRange)
        {
            int unitdamagepriority = unit.GetInt32(GameUnit.eEntityAttr.DamagePriority_Int32);
            if (unitdamagepriority < minPriority)
            {
                minPriority = unitdamagepriority;
                priorityTargets.Clear();
                priorityTargets.Add(unit);
            }
            else if (unitdamagepriority == minPriority)
            {
                priorityTargets.Add(unit);
            }
        }
        //获得技能范围以内的最高优先级的Unit，进行距离判断
        if (priorityTargets.Count > 0)
        {
            IUnit closestTarget = priorityTargets[0];
            float closestDistance = Vector2.Distance(mainCharacterUnit.Position, closestTarget.Position);
            foreach (var unit in priorityTargets)
            {
                float distance = Vector2.Distance(mainCharacterUnit.Position, unit.Position);
                if (distance < closestDistance)
                {
                    closestDistance = distance;
                    closestTarget = unit;
                }
            }
            Debug.LogError($"技能距离内可施法的目标有{targetInRange.Count}个，最高优先级为{minPriority}，技能范围是{skillDistance}，他和我的距离是{closestDistance}");
            return closestTarget;
        }
        return null;
    }

    eUnitType ChangeClickType(AutoEnum.eTargetType targetType)
    {
        switch (targetType)
        {
            case eTargetType.EnemyCharacter:
                return eUnitType.Creature;//预留
            case eTargetType.Monster:
                return eUnitType.Monster;
            case eTargetType.Boss:
                return eUnitType.Boss;
            case eTargetType.PlayerSelf:
                return eUnitType.Character;
            default:
                return eUnitType.Character;//预留
        }
    }

    void ExecuteSkillWithTarget(UInt64 target)
    {
        if (_ActiveSkillConfigID != 0 && target != 0)
        {
            IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
            IUnit targetUnit = GameUnitApi.QueryUnit(target);
            Vector3 direction = targetUnit.Position - mainCharacter.Position;
            if (direction.magnitude > AutoConfig.Skill.Get(_ActiveSkillConfigID)._MaxRangeRadius)
            {
                XApi.Debug($"Skill {_ActiveSkillConfigID} out of range");
                return;
            }
            
            float angle = Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg;
            oVector2 targetpos = new oVector2
            {
                X = targetUnit.Position.x,
                Y = targetUnit.Position.y,
            };
            Vector2 characterPos = mainCharacter.Position;
            oVector2 castPos = new oVector2
            {
                X = characterPos.x,
                Y = characterPos.y
            };

            AutoEnum.eTargetType targetUnitType = SelectUnitType(targetUnit);
            AutoEnum.eTargetType[] configtargetTypes = AutoConfig.Skill.Get(_ActiveSkillConfigID)._TargetType;

            if (Array.Exists(configtargetTypes, element => element == targetUnitType))
            {
                _SkillManager.UseSkill(0, _ActiveSkillConfigID, angle, targetpos, target, castPos);
                Debug.LogWarning($"使用技能，GUID是 {_ActiveSkillConfigID} ，技能ID是{_ActiveSkillConfigID} 角度是{angle} 目标位置是{targetpos} 目标GUID是{target}");
                CancelSkillRelease();
            }
            else
            {
                XApi.Debug($"目标类型{targetUnitType}不在技能{_ActiveSkillConfigID}的目标类型列表中");
            }
        }

    }

    AutoEnum.eTargetType SelectUnitType(IUnit unit)
    {
        eUnitType unitType = unit.Type;
        switch (unitType)
        {
            case eUnitType.Character://角色
                if (unit.Guid == _CharacterManager.GetMainCharacterUnit().Guid)
                {
                    return AutoEnum.eTargetType.PlayerSelf;
                }
                else
                {
                    return AutoEnum.eTargetType.PlayerCharacter;
                }

            case eUnitType.Monster://生物
                return AutoEnum.eTargetType.Monster;

        }
        return AutoEnum.eTargetType.All;
    }
    public void CancelCurrentSkill()
    {
        if (!string.IsNullOrEmpty(_PressedSkillSlot))
        {
            CancelSkillRelease();

            // 重置状态
            _PressedSkillSlot = null;
            _IsDraggingToCancel = false;
            _IsLongPress = false;
            if (_SkillCancelArea != null)
            {
                _SkillCancelArea.ResetCancelState();
            }
        }
    }

}