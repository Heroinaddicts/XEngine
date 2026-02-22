using System;
using UnityEngine;
using XEngine;
using XGame;
using static GameUnitApi;
using System.Collections.Generic;
using AutoConfig;
using GameUnit;

public interface ISkillWarningManager : IModule
{

}
public class SkillWarningManager : ISkillWarningManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<ISkillWarningManager, SkillWarningManager>();
    }
    IClient _Client;
    SkillVisual.ISkillVisual _SkillPreviewVisual = null;

    class SkillWarningInfo
    {
        public GameObject _Parent;
        public GameObject _TargetUnit;
        public SkillVisual.ISkillVisual _SkillVisual;
    }

    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _Client.RegistProtobuf<oSkillWarningPush>(eServerID.SkillWarningPush, SkillWarningPush);

        return true;
    }
    public void Release()
    {

    }


    private void SkillWarningPush(oSkillWarningPush body)
    {
        //Debug.LogError("收到技能警告推送");
        IUnit castUnit = GameUnitApi.QueryUnit(body.CastGuid);
        eUnitType castUnitType = castUnit.Type;
        Vector2 targetPos = new Vector2(body.TargetPos.X, body.TargetPos.Y);
        float agnel = body.Angle;
        Int64 skillConfigID = body.SkillConfigId;
        if (_SkillPreviewVisual != null)
        {
            _SkillPreviewVisual = null;
        }
        switch (castUnitType)
        {
            case eUnitType.Character:
                //TODO 玩家技能预览
                break;
            case eUnitType.Boss:
                DrawMonsterSkillSheetsType(castUnit, targetPos, skillConfigID, agnel);
                //Debug.LogError("收到技能警告推送 boss技能ID:" + skillConfigID);
                break;
            case eUnitType.Monster:
                DrawMonsterSkillSheetsType(castUnit, targetPos, skillConfigID, agnel);
                //Debug.LogError("收到技能警告推送 monster技能ID:" + skillConfigID);
                break;
        }
    }

    void DrawMonsterSkillSheetsType(IUnit castUnit, Vector2 targetPos, Int64 skillConfigID, float angle)
    {
        AutoEnum.eSheetsType sheetsType = AutoConfig.MonsterSkill.Get(skillConfigID)._SheetsType;
        float[] sheetsParameter = AutoConfig.MonsterSkill.Get(skillConfigID)._RangeParameter;
        SkillVisual.ISkillVisual skillVisual = null;

        switch (sheetsType)
        {
            case AutoEnum.eSheetsType.Circle://圆形
                skillVisual = SkillVisual.Api.CreateCircleVisual(sheetsParameter[0]);
                if (skillVisual != null)
                {
                    skillVisual.GetComponent<MeshRenderer>().material = Resources.Load<UnityEngine.Material>("Material_SpeTexture/Material/SkillWarning");
                    //skillVisual.GetComponent<MeshRenderer>().material = Resources.Load<UnityEngine.Material>("Material&SpeTexture/Shader/ShockWave_Sprite");
                    //Debug.LogError("绘制对应面片技能预览，技能ID:" + skillConfigID + " 半径:" + sheetsParameter[0]);
                }
                break;
            case AutoEnum.eSheetsType.Rectangle://矩形
                skillVisual = SkillVisual.Api.CreateRectangleVisual(sheetsParameter[0], sheetsParameter[1]);
                if (skillVisual != null)
                {
                    MeshFilter meshFilter = skillVisual.GetComponent<MeshFilter>();
                    if (meshFilter != null && meshFilter.mesh != null)
                    {
                        Vector3[] vertices = meshFilter.mesh.vertices;
                        float bottomEdgeZOffset = Mathf.Abs(vertices[0].z);
                        for (int i = 0; i < vertices.Length; i++)
                        {
                            vertices[i].z += bottomEdgeZOffset;
                        }
                        meshFilter.mesh.vertices = vertices;
                        meshFilter.mesh.RecalculateBounds();
                        skillVisual.transform.localScale = new Vector3(1, 1, -1);
                    }
                    if (skillVisual != null)
                    {
                        skillVisual.GetComponent<MeshRenderer>().material = Resources.Load<UnityEngine.Material>("UI/Sprites/Skill/Arrow");
                    }
                }
                break;
            case AutoEnum.eSheetsType.sector://扇形，半径和角度
                skillVisual = SkillVisual.Api.CreateSectorVisual(sheetsParameter[0], sheetsParameter[1]);
                if (skillVisual != null)
                {
                    skillVisual.GetComponent<MeshRenderer>().material = Resources.Load<UnityEngine.Material>("UI/Sprites/Skill/WorldPos");
                }
                break;
            case AutoEnum.eSheetsType.None://无技能预览
                break;
        }

        if (skillVisual == null)
        {
            Debug.LogWarning("技能预览创建失败");
            return;
        }

        MeshRenderer meshRenderer = skillVisual.transform.GetComponent<MeshRenderer>();
        if (meshRenderer != null)
        {
            meshRenderer.sortingOrder = 9;
        }

        Transform placementLabTransform = castUnit.transform.Find("PlacementLab");
        GameObject parent;
        if (placementLabTransform == null)
        {
            XApi.Debug($"PlacementLab not found for castUnit {castUnit.Guid}, using castUnit as parent");
            // 如果没找到PlacementLab，则使用castUnit的gameObject作为parent
            parent = castUnit.gameObject;
        }
        else
        {
            parent = placementLabTransform.gameObject;
        }
        skillVisual.transform.SetParent(parent.transform);
        skillVisual.transform.localPosition = Vector3.zero;
        skillVisual.transform.localScale = AutoConfig.MonsterSkill.Get(skillConfigID)._RangeParameter[0] * Vector3.one;
        skillVisual.transform.localRotation = Quaternion.Euler(90, 0, 0);
        angle = (angle + 360f) % 360f;
        skillVisual.transform.eulerAngles = new Vector3(angle, -90, -90);
        skillVisual.transform.position = new Vector2(targetPos.x, targetPos.y);
        if (sheetsType == AutoEnum.eSheetsType.Circle)
        {
            skillVisual.transform.localRotation = Quaternion.Euler(30, 0, 0);
        }

        FillSkillSheets(skillConfigID, sheetsType, parent, skillVisual);
    }
    void FillSkillSheets(Int64 skillConfigID, AutoEnum.eSheetsType sheetsType, GameObject parent, SkillVisual.ISkillVisual skillVisual)
    {
        string fillpng = AutoConfig.MonsterSkill.Get(skillConfigID)._EnoughMaxSheetsPrefab;
        if (string.IsNullOrEmpty(fillpng))
        {
            return;
        }
        Sprite fillSprite = Resources.Load<Sprite>(fillpng);
        if (fillSprite != null)
        {
            GameObject imageGO = new GameObject("SkillFillImage");
            imageGO.transform.SetParent(skillVisual.transform, false);
            SpriteRenderer spriteRenderer = imageGO.AddComponent<SpriteRenderer>();
            spriteRenderer.sprite = fillSprite;
            spriteRenderer.sortingOrder = 9;

            imageGO.transform.localPosition = Vector3.zero;
            imageGO.transform.localScale = Vector3.zero;
            // imageGO.transform.Rotate(30,0,0);
            if (sheetsType == AutoEnum.eSheetsType.Circle)
            {
                imageGO.transform.localRotation = Quaternion.Euler(30, 0, 0);
            }

            Int64 previousTime = AutoConfig.MonsterSkill.Get(skillConfigID)._PreviousTime;
            if (previousTime <= 0)
            {
                skillVisual.transform.localScale = Vector3.one;
                return;
            }
            int allcount = (int)previousTime / 16;
            if (allcount <= 0)
            {
                allcount = 1;
            }
            SkillWarningInfo skillWarningInfo = new SkillWarningInfo();
            skillWarningInfo._Parent = parent;
            skillWarningInfo._TargetUnit = imageGO.gameObject;
            skillWarningInfo._SkillVisual = skillVisual;
            XApi.TimerApi.StartTimer(FillSheetsEnd, skillWarningInfo, skillConfigID, 0, allcount, 16);
        }
    }

    private void FillSheetsEnd(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
    {
        SkillWarningInfo skillWarningInfo = (SkillWarningInfo)obj;
        GameObject skillPreviewGO = skillWarningInfo._TargetUnit;
        GameObject parent = skillWarningInfo._Parent;
        SkillVisual.ISkillVisual skillVisual = skillWarningInfo._SkillVisual;
        // int allcount = (int)AutoConfig.MonsterSkill.Get((Int64)context)._PreviousTime / 16;
        // float oncescale = AutoConfig.MonsterSkill.Get((Int64)context)._RangeParameter[0] / allcount;
        Int64 skillConfigID = (Int64)context;
        MonsterSkill skillData = AutoConfig.MonsterSkill.Get(skillConfigID);

        if (state == XApi.ITimer.eState.BEAT)
        {
            if (skillPreviewGO != null)
            {
                if (skillPreviewGO.transform.localScale.x >= Vector3.one.x &&
                   skillPreviewGO.transform.localScale.y >= Vector3.one.y &&
                   skillPreviewGO.transform.localScale.z >= Vector3.one.z
                   )
                {
                    return;
                }
                float scaleIncrement = (skillData._RangeParameter[0]*skillData._FullRangeParameter[0]) / (skillData._PreviousTime / 16);

                Vector3 newScale = new Vector3(skillPreviewGO.transform.localScale.x + Vector3.one.x * scaleIncrement,
                                                skillPreviewGO.transform.localScale.y + Vector3.one.y * scaleIncrement,
                                                skillPreviewGO.transform.localScale.z);
                skillPreviewGO.transform.localScale = newScale;
            }
            //Debug.LogError("填充技能预览，当前填充次数:" + count + " 填充比例:" + skillPreviewGO.transform.localScale.x);
        }

        if (state == XApi.ITimer.eState.END)
        {
            //Debug.LogError("填充技能预览结束");
            XApi.TimerApi.StopTimer(FillSheetsEnd, obj);

            if (skillPreviewGO != null)
            {
                GameObject.Destroy(skillPreviewGO);
            }
            if (skillVisual != null)
            {
                GameObject.Destroy(skillVisual.gameObject);
                // skillVisual = null;
            }
            // if (parent != null && parent.transform != null)
            // {
            //     for (int i = parent.transform.childCount - 1; i >= 0; i--)
            //     {
            //         GameObject.Destroy(parent.transform.GetChild(i).gameObject);
            //     }
            // }

        }
    }
}




