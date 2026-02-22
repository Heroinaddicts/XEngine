using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class SkillCancelArea : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    [SerializeField] private GameObject ExitSkillBtn;
    public string _PressedSkillSlot;
    // 是否处于取消区域
    public bool _IsDraggingToCancel;

    private void Start()
    {
        if (ExitSkillBtn == null)
            ExitSkillBtn = gameObject;
        if (ExitSkillBtn.GetComponent<Graphic>() != null)
            ExitSkillBtn.GetComponent<Graphic>().raycastTarget = true;
    }

    // 指针进入取消按钮时触发
    public void OnPointerEnter(PointerEventData eventData)
    {
        // 只有“按下了技能”且“进入的是取消按钮”时，标记为取消状态
        if (!string.IsNullOrEmpty(_PressedSkillSlot) && ExitSkillBtn != null)
        {
            _IsDraggingToCancel = true;
            Debug.LogError($"进入技能释放取消区域，技能槽：{_PressedSkillSlot}");
        }
    }

    // 指针离开取消按钮时触发
    public void OnPointerExit(PointerEventData eventData)
    {
        // 只有“按下了技能”且“离开的是取消按钮”时，取消标记
        if (!string.IsNullOrEmpty(_PressedSkillSlot) && ExitSkillBtn != null)
        {
            _IsDraggingToCancel = false;
            Debug.LogError($"退出技能释放取消区域，技能槽：{_PressedSkillSlot}");
        }
    }

    public void ResetCancelState()
    {
        _PressedSkillSlot = string.Empty;
        _IsDraggingToCancel = false;
    }
}