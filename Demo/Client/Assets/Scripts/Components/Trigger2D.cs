using System.Collections.Generic;
using UnityEngine;

class Trigger2D : MonoBehaviour
{
    public delegate void fOnTrigger2D(Collider2D other);

    List<fOnTrigger2D> _OnTriggerEnter2DList = null;
    List<fOnTrigger2D> _OnTriggerExit2DList = null;
    List<fOnTrigger2D> _OnTriggerStay2DList = null;

    public void RegistTriggerCallback(in fOnTrigger2D enter, in fOnTrigger2D exit, in fOnTrigger2D stay)
    {
        if (enter != null)
        {
            if (_OnTriggerEnter2DList == null)
                _OnTriggerEnter2DList = new List<fOnTrigger2D>();
            _OnTriggerEnter2DList.Add(enter);
        }

        if (exit != null)
        {
            if (_OnTriggerExit2DList == null)
                _OnTriggerExit2DList = new List<fOnTrigger2D>();
            _OnTriggerExit2DList.Add(exit);
        }

        if (stay != null)
        {
            if (_OnTriggerStay2DList == null)
                _OnTriggerStay2DList = new List<fOnTrigger2D>();
            _OnTriggerStay2DList.Add(stay);
        }
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if (_OnTriggerEnter2DList != null)
        {
            foreach (var func in _OnTriggerEnter2DList)
            {
                func(other);
            }
        }
    }
    private void OnTriggerExit2D(Collider2D other)
    {
        if (_OnTriggerExit2DList != null)
        {
            foreach (var func in _OnTriggerExit2DList)
            {
                func(other);
            }
        }
    }
    private void OnTriggerStay2D(Collider2D other)
    {
        if (_OnTriggerStay2DList != null)
        {
            foreach (var func in _OnTriggerStay2DList)
            {
                func(other);
            }
        }
    }
}