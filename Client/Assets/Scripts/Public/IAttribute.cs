using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum eAttributeType
{
    INT,
    FLOAT,
    STRING,
    BOOL
}

public delegate void OnValueChangedCallback<T>(in string name, in int index, in T oldValue, in T newValue, in object Context);

[Serializable]
public class AttrInfo
{
    [SerializeField] public string _name;
    [SerializeField] public int _index;
    [SerializeField] public eAttributeType _type;
    [SerializeField] public string _desc;

    public AttrInfo(in string name, in int index, in eAttributeType type, in string desc)
    {
        _name = name;
        _index = index;
        _type = type;
        _desc = desc;
    }

    [SerializeField] long _int_value;
    [SerializeField] float _float_value;
    [SerializeField] string _string_value;
    [SerializeField] bool _bool_value;

    interface IValueChangedCallback
    {
        public void OnValueChanged(in string name, in int index, in object oldValue, in object newValue, in object context);
    }

    class TValueChangedCallback<T> : IValueChangedCallback
    {

        OnValueChangedCallback<T> _call;
        public TValueChangedCallback(in OnValueChangedCallback<T> call)
        {
            _call = call;
        }

        public void OnValueChanged(in string name, in int index, in object oldValue, in object newValue, in object context)
        {
            if (null != _call)
            {
                _call(name, index, (T)oldValue, (T)newValue, context);
            }
        }
    }


    List<IValueChangedCallback> _callback_list;

    public void RegisterValueChanged<T>(in OnValueChangedCallback<T> callback)
    {
        if (null == _callback_list)
        {
            _callback_list = new List<IValueChangedCallback>();
        }

        _callback_list.Add(new TValueChangedCallback<T>(callback));
    }

    public eAttributeType GetAttrType()
    {
        return _type;
    }

    public long GetInt()
    {
        return _int_value;
    }

    public void SetInt(in long value, in object context = null, in bool trigger = true)
    {
        long old = _int_value;
        _int_value = value;
        if (_callback_list == null || trigger == false) return;
        foreach (IValueChangedCallback callback in _callback_list)
        {
            callback.OnValueChanged(_name, _index, old, _int_value, context);
        }
    }

    public float GetFloat()
    {
        return _float_value;
    }

    public void SetFloat(in float value, in object context = null, in bool trigger = true)
    {
        float old = _float_value;
        _float_value = value;
        if (_callback_list == null || trigger == false) return;
        foreach (IValueChangedCallback callback in _callback_list)
        {
            callback.OnValueChanged(_name, _index, old, _float_value, context);
        }
    }

    public string GetString()
    {
        return _string_value;
    }

    public void SetString(in string value, in object context = null, in bool trigger = true)
    {
        string old = _string_value;
        _string_value = value;
        if (_callback_list == null || trigger == false) return;
        foreach (IValueChangedCallback callback in _callback_list)
        {
            callback.OnValueChanged(_name, _index, old, _string_value, context);
        }
    }

    public bool GetBool()
    {
        return _bool_value;
    }

    public void SetBool(in bool value, in object context = null, in bool trigger = true)
    {
        bool old = _bool_value;
        _bool_value = value;
        if (_callback_list == null || trigger == false) return;
        foreach (IValueChangedCallback callback in _callback_list)
        {
            callback.OnValueChanged(_name, _index, old, _bool_value, context);
        }
    }
}

public class IAttribute : MonoBehaviour
{
    [Header("属性表")]
    [SerializeField] protected AttrInfo[] Attributes;

    public eAttributeType GetAttrType(in int index)
    {
        return Attributes[index].GetAttrType();
    }

    public long GetAttrIntValue(in int index)
    {
        if (Attributes[index].GetAttrType() == eAttributeType.INT)
        {
            return Attributes[index].GetInt();
        }
        else
        {
            Debug.LogError("Attribute type is not int");
            return -108873;
        }
    }

    public void SetAttrIntValue(in int index, in int value, in object context = null, in bool trigger = true)
    {
        if (Attributes[index].GetAttrType() == eAttributeType.INT)
        {
            Attributes[index].SetInt(value, context, trigger);
        }
        else
        {
            Debug.LogError("Attribute type is not int");
        }
    }

    public float GetAttrFloatValue(in int index)
    {
        if (Attributes[index].GetAttrType() == eAttributeType.FLOAT)
        {
            return Attributes[index].GetFloat();
        }
        else
        {
            Debug.LogError("Attribute type is not float");
            return -108873.108873f;
        }
    }

    public void SetAttrFloatValue(in int index, in float value, in object context = null, in bool trigger = true)
    {
        if (Attributes[index].GetAttrType() == eAttributeType.FLOAT)
        {
            Attributes[index].SetFloat(value, context, trigger);
        }
        else
        {
            Debug.LogError("Attribute type is not float");
        }
    }


    public void RegisterAttrValueChangedCallback<T>(in int attr_index, in OnValueChangedCallback<T> callback)
    {
        if (attr_index < 0 || attr_index >= Attributes.Length)
        {
            Debug.LogError("attr index error");
            return;
        }

        Attributes[attr_index].RegisterValueChanged<T>(callback);
    }
}
