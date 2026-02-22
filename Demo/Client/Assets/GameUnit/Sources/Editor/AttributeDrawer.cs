#if UNITY_EDITOR
using GameUnit;
using UnityEditor;
using UnityEngine;

namespace GameUnitEditor
{
    [CustomPropertyDrawer(typeof(Attribute))]
    public class AttributeDrawer : PropertyDrawer
    {
        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            EditorGUI.BeginProperty(position, label, property);

            // 计算整体布局
            float lineHeight = EditorGUIUtility.singleLineHeight;
            float spacing = 2f;

            // 取所有字段
            var typeProp = property.FindPropertyRelative("_Type");
            var indexProp = property.FindPropertyRelative("_Index");
            var nameProp = property.FindPropertyRelative("_Name");
            var valueProp = property.FindPropertyRelative("_Value");

            // 第一行: Type, Index, Name
            Rect typeRect = new Rect(position.x, position.y, 80f, lineHeight);
            Rect indexRect = new Rect(position.x + 90f, position.y, 50f, lineHeight);
            Rect nameRect = new Rect(position.x + 150f, position.y, position.width - 150f, lineHeight);

            EditorGUI.PropertyField(typeRect, typeProp, GUIContent.none);
            EditorGUI.PropertyField(indexRect, indexProp, GUIContent.none);
            EditorGUI.PropertyField(nameRect, nameProp, GUIContent.none);

            if (null != valueProp)
            {
                Rect valueRect = new Rect(position.x, position.y + lineHeight + spacing, position.width, lineHeight);
                EditorGUI.PropertyField(valueRect, valueProp, GUIContent.none);
            }
            EditorGUI.EndProperty();
        }

        public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
        {
            float lineHeight = EditorGUIUtility.singleLineHeight;
            float spacing = 2f;
            return (lineHeight * 2) + spacing;
        }
    }
}
#endif
