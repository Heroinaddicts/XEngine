using System;
using UnityEditor;
using UnityEngine;
public class TimeScaleTool : EditorWindow 
{

    private float timeScale = 1;
    private float timeScaleMax = 100f;
    private float timeScaleMin = 0;

	private void DrawTimeScale()
    {
        EditorGUILayout.Space();
        EditorGUILayout.BeginVertical("box", new GUILayoutOption[0]);
        this.timeScaleMin = EditorGUILayout.FloatField("最小时速", this.timeScaleMin, new GUILayoutOption[0]);
        //this.timeScaleMax = EditorGUILayout.FloatField("最大时速", this.timeScaleMax, new GUILayoutOption[0]);
        EditorGUILayout.EndVertical();
        EditorGUILayout.Space();
        EditorGUILayout.BeginVertical("box", new GUILayoutOption[0]);
        this.timeScale = EditorGUILayout.Slider("当前时速", this.timeScale, this.timeScaleMin, this.timeScaleMax, new GUILayoutOption[0]);
        EditorGUILayout.EndVertical();
        EditorGUILayout.Space();
        if (GUILayout.Button("还原", new GUILayoutOption[0]))
        {
            this.timeScaleMin = 0f;
            this.timeScale = 1f;
        }
//         if(GUILayout.Button("恢复至"+timeScale, new GUILayoutOption[0]))
//         {
//             
//         }
        if (GUILayout.Button("暂停", new GUILayoutOption[0]))
        {
            this.timeScale = 0f;
        }
        Time.timeScale = timeScale;
    }

    public void Init()
    {
        timeScale = Time.timeScale;
        if (this.timeScale > this.timeScaleMax)
        {
            this.timeScaleMin = this.timeScale - 1f;
        }
    }

    private void OnGUI()
    {
        this.DrawTimeScale();
    }


}
