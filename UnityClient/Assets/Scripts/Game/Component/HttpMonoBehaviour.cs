using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;
using UnityEngine.Networking;

public abstract class HttpMonoBehaviour : MonoBehaviour
{
    protected abstract void OnResponse(in HttpDef.eRequestID rid, in bool isSuccess, in string data);

    protected void StartRequest<T>(string url, HttpDef.eRequestID rid, HttpDef.eRequestType type, T parameter)
    {
        StartCoroutine(DoRequest(url, rid, type, parameter));
    }

    IEnumerator DoRequest<T>(string url, HttpDef.eRequestID rid, HttpDef.eRequestType type, T parameter)
    {
        UnityWebRequest req = null;
        switch(type)
        {
            case HttpDef.eRequestType.Get:
                {
                    req = UnityWebRequest.Get(url);
                    break;
                }
            case HttpDef.eRequestType.Post:
                {
                    FieldInfo[] infos = typeof(T).GetFields();
                    Dictionary<string, string> param_map = new Dictionary<string, string>();
                    foreach (FieldInfo p in infos)
                    {
                        string value = p.GetValue(parameter).ToString();
                        param_map.Add(p.Name, value);
                    }
                    req = UnityWebRequest.Post(url, param_map);
                    break;
                }
            default:
                break;
        }
        using (req)
        {
            yield return req.SendWebRequest();

            if (req.result != UnityWebRequest.Result.Success)
            {
                Debug.LogError("Http request error, code " + req.result);
                this.OnResponse(rid, false, null);
            } else
            {
                Debug.Log("Http request success, data : " + req.downloadHandler.text);
                this.OnResponse(rid, true, req.downloadHandler.text);
            }
        }
    }
}
