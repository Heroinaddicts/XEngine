using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class Adjust : MonoBehaviour
{

    static void __AdjustPosition(Transform tf)
    {
        Vector3 pos = tf.position;
        for(int i=0; i<tf.childCount; i++)
        {
            tf.GetChild(i).position += pos;
        }

        tf.position = new Vector3(0, 0, 0);
    }

    [MenuItem("Adjust/Position")]
    public static void AdjustPosition()
    {
        Transform[] tf = Selection.transforms;
        for(int i=0; i<tf.Length; i++)
        {
            __AdjustPosition(tf[i]);
        }
    }

    static void __getMiniPosition(Transform tf, ref Vector3 min)
    {
        for (int i=0; i<tf.childCount; i++)
        {
            if (tf.GetChild(i).position.x < min.x)
            {
                min.x = tf.GetChild(i).position.x;
            }
            if (tf.GetChild(i).position.y < min.y)
            {
                min.y = tf.GetChild(i).position.y;
            }
            if (tf.GetChild(i).position.z < min.z)
            {
                min.z = tf.GetChild(i).position.z;
            }
        }
    }

    static void __adjustRelative(Transform tf, Vector3 min)
    {
        for (int i = 0; i < tf.childCount; i++)
        {
            tf.GetChild(i).position -= min;
        }
    }

    [MenuItem("Adjust/Relative")]
    public static void AdjustRelative()
    {
        Vector3 min = new Vector3(10000000000000, 10000000000000, 10000000000000);

        Transform[] tf = Selection.transforms;
        for (int i = 0; i < tf.Length; i++)
        {
            __getMiniPosition(tf[i], ref min);
        }

        Debug.Log("min position " + min);

        for (int i=0; i<tf.Length; i++)
        {
            __adjustRelative(tf[i], min);
        }
    }
}
