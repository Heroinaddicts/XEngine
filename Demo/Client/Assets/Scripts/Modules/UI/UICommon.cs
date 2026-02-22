using System;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using XEngine;
using XGame;
using XUtils;

public interface IUICommon : IModule
{
    void AlertUI(in string title, in string content, in string btnText, Action callback = null);
    void ConfirmUI(in string title, in string content, Action yes = null, Action no = null);
    void FloatText(in string content);
    void Danmaku(in string content);
}

class UICommon : IUICommon
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IUICommon, UICommon>();
    }


    GameObject _UIAlert = null;
    Text _UIAlertBtnOKText;
    Text _UIAlertTextContent;
    Button _UIAlertBtnOK;

    IGameSceneManager _GameSceneManager;

    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();
        _GameSceneManager.RegistEvent(IGameSceneManager.eEvent.SceneLoaded, OnSceneLoaded);
        return true;
    }

    public void Release()
    {

    }

    private void OnSceneLoaded(UnityEngine.SceneManagement.Scene scene)
    {
        if (_UIAlert != null)
        {
            GameObject.Destroy(_UIAlert);
        }

        _UIAlert = GameObject.Instantiate(Resources.Load<GameObject>("UI/Prefabs/Common/UIAlert"));
        _UIAlert.transform.parent = GameObjectCommon.GetGameObjectByPath("Canvas").transform;
        _UIAlert.transform.localPosition = Vector2.zero;
        _UIAlertTextContent = GameObjectCommon.GetChildByPath(_UIAlert, "TextContent").GetComponent<Text>();
        _UIAlertBtnOK = GameObjectCommon.GetChildByPath(_UIAlert, "BtnOK").GetComponent<Button>();
        _UIAlertBtnOKText = GameObjectCommon.GetChildByPath(_UIAlert, "BtnOK/TextBtn").GetComponent<Text>();
        _UIAlert.SetActive(false);
    }

    public void AlertUI(in string title, in string content, in string btnText, Action callback)
    {
        _UIAlert.SetActive(true);
        _UIAlertTextContent.text = content;
        _UIAlertBtnOKText.text = btnText;
        _UIAlertBtnOK.onClick.RemoveAllListeners();
        _UIAlertBtnOK.onClick.AddListener(() =>
        {
            _UIAlert.SetActive(false);
            callback?.Invoke();
        });
    }

    public void ConfirmUI(in string title, in string content, Action yes, Action no)
    {
    }

    public void Danmaku(in string content)
    {

    }

    public void FloatText(in string content)
    {

    }
}
