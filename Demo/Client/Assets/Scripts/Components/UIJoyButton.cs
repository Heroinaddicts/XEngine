using System;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.InputSystem;
using UnityEngine.UI;
using XEngine;
using XGame;
using XUtils;
using static GameUnitApi;
using static UIJoyStick;

class UIJoyButton : MonoBehaviour, IDragHandler, IPointerDownHandler, IPointerUpHandler
{

    public enum eEvent
    {
        BtnSkillClickDown = 0,
        BtnSkillClickDrag = 1,
        BtnSkillClickUp = 2,
        BtnSkillClickCancel = 3
    }

    //public delegate void fOnSkillBtn(in AutoConfig.Skill config, in float angle);

    static XEventPool<eEvent> s_EventPool = new XEventPool<eEvent>();
    //     public static void RegistEvent(in eEvent ev, in fOnSkillBtn callback)
    //     {
    //         s_EventPool.Regist(ev, callback);
    //     }

    IUISkillManager _UISkillManager;
    ISkillManager _SkillManager;
    AutoConfig.Skill _SkillConfig;
    ICharacterManager _CharacterManager;
    Int64 _SkillConfigID;

    [SerializeField]
    Image _SkillIcon;
    [SerializeField]
    Image _CDMask;

    [Header("技能取消区域")]
    public SkillCancelArea _SkillCancelArea;

    private string _PressedSkillSlot = null;
    private bool _IsDraggingToCancel = false;
    private bool _IsLongPress = false;

    private void Start()
    {
        _UISkillManager = Launcher.GetModule<IUISkillManager>();
        _SkillManager = Launcher.GetModule<ISkillManager>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
    }

    float _LastAngle;
    public void OnPointerDown(PointerEventData eventData)
    {
        XApi.Debug($"OnPointerDown {gameObject.name}, Skill {_UISkillManager?.GetSkillConfig(gameObject.name)?._Name}");
        if (_CDMask.fillAmount > 0)
        {
            Debug.LogWarning("技能冷却中");
            return;
        }
        // 设置当前按下的技能槽
        _PressedSkillSlot = gameObject.name;
        _IsLongPress = false;
        _IsDraggingToCancel = false;
        _SkillConfigID = _UISkillManager?.GetSkillConfigID(gameObject.name) ?? 0;
        _SkillConfig = _UISkillManager?.GetSkillConfig(gameObject.name);

        if (_SkillCancelArea != null)
        {
            _SkillCancelArea._PressedSkillSlot = _PressedSkillSlot;
        }

        _UISkillManager?.OnSkillButtonPress(gameObject.name);
        Debug.LogWarning("技能槽按下：" + gameObject.name);
    }

    public void OnDrag(PointerEventData eventData)
    {
        // 拖拽过程中检查是否进入取消区域已在OnPointerEnter中处理
    }

    public void OnPointerUp(PointerEventData eventData)
    {
        Debug.LogWarning("技能槽抬起：" + gameObject.name);
        XApi.Debug($"OnPointerUp {gameObject.name}, Skill {_UISkillManager?.GetSkillConfig(gameObject.name)?._Name}");

        if (!string.IsNullOrEmpty(_PressedSkillSlot) && _SkillCancelArea != null)
        {
            // 检查是否滑动到取消区域
            if (_SkillCancelArea._IsDraggingToCancel)
            {
                // 取消技能释放
                CancelCurrentSkill();
            }
            else
            {
                // 正常释放技能
                Debug.LogWarning("技能释放成功");
                _UISkillManager?.OnSkillButtonRelease(_PressedSkillSlot);

            }

            // 重置状态
            _PressedSkillSlot = null;
            _IsDraggingToCancel = false;
            _IsLongPress = false;
            _SkillCancelArea.ResetCancelState();
        }
    }


    void RefreshCDMask()
    {
        // 确保所有必要的对象都不为空
        if (_UISkillManager == null || _SkillManager == null || _CDMask == null)
        {
            return;
        }

        Int64 currentSkillConfigId = _UISkillManager.GetSkillConfigID(gameObject.name);
        if (currentSkillConfigId != 0)
        {
            //ICharacterManager characterManager = Launcher.GetModule<ICharacterManager>();
            UInt64 characterId = 0;
            if (_CharacterManager != null)
            {
                IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
                if (mainCharacter != null)
                {
                    characterId = mainCharacter.Guid;
                }
            }
            Int64 lastCastTick = 0;
            if (characterId != 0)
            {
                lastCastTick = _SkillManager.GetLastCastTickForCharacter(characterId, currentSkillConfigId);
            }
            else
            {
                lastCastTick = _SkillManager.GetLastCastTick(currentSkillConfigId);
            }
            Int64 tick = SafeSystem.Time.GetMillisecondTick() - lastCastTick;
            if (_SkillConfig != null && _SkillConfig._CoolDown > 0)
            {
                if (tick >= _SkillConfig._CoolDown)
                {
                    _CDMask.fillAmount = 0;
                }
                else
                {
                    _CDMask.fillAmount = (float)(_SkillConfig._CoolDown - tick) / (float)_SkillConfig._CoolDown;
                }
            }
            else
            {
                _CDMask.fillAmount = 0;
            }
        }
        else
        {
            // 如果没有技能，则不显示CD遮罩
            if (_CDMask != null)
                _CDMask.fillAmount = 0;
        }
    }

    void Update()
    {
        // 添加空值检查
        if (_UISkillManager == null)
            return;

        _SkillConfig = _UISkillManager.GetSkillConfig(gameObject.name);
        if (_SkillConfig != null)
        {
            RefreshCDMask();
        }
    }


    //取消当前技能释放
    public void CancelCurrentSkill()
    {
        if (!string.IsNullOrEmpty(_PressedSkillSlot))
        {
            XApi.Debug("取消当前技能: " + _PressedSkillSlot);
            // 调用技能管理器的取消方法
            _UISkillManager?.CancelSkillRelease();

            // 重置状态
            _PressedSkillSlot = null;
            _IsDraggingToCancel = false;
            _IsLongPress = false;
        }
    }

}