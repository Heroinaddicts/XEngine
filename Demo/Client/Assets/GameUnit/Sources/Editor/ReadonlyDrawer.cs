#if UNITY_EDITOR
using GameUnit;
using UnityEditor;
using UnityEngine;

namespace GameUnitEditor
{

    //     [CustomPropertyDrawer(typeof(ReadonlyAttribute))]
    //     class ReadonlyDrawer : PropertyDrawer
    //     {
    //         public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
    //         {
    //             GUI.enabled = false;  // 让控件变成灰色不可编辑
    //             EditorGUI.PropertyField(position, property, label, true);
    //         }
    // 
    //         public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
    //         {
    //             return EditorGUI.GetPropertyHeight(property, label, true);
    //         }
    //     }
}

#endif
