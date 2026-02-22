using System;
using System.Collections.Generic;
using System.Linq;
using AutoConfig;
using DG.Tweening;
using GameUnit;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using static GameUnitApi;

namespace XGameClient
{
    public interface IUIMiniTask : IModule
    {

    }

    public class UIMiniTask : IUIMiniTask
    {

        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<IUIMiniTask, UIMiniTask>();
        }

        IUIMain _UIMain;
        ICharacterManager _CharacterManager;
        GameObject _UIMiniTask;
        IUnit _CharacterUnit;
        IClient _Client;
        bool isOpen = false;

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {
            _Client = Launcher.GetModule<IClient>();
            _UIMain = Launcher.GetModule<IUIMain>();
            _CharacterManager = Launcher.GetModule<ICharacterManager>();
            _UIMain.RegistEvent(IUIMain.eEvent.OpenMiniTask, OpenMiniTask);
            return true;
        }

        void OpenMiniTask()
        {
            //_UIMain.UnRegistEvent(IUIMain.eEvent.OpenMiniTask, OpenMiniTask);
            GameObject TaskFrame = GameObject.Find("TaskFrame");
            if (TaskFrame != null)
            {
                RectTransform rectTransform = TaskFrame.GetComponent<RectTransform>();
                Transform arrow = TaskFrame.transform.Find("Withdraw");
                if (rectTransform != null)
                {
                    float targetX;
                    float targetRotation;
                    if (isOpen)
                    {
                        targetX = rectTransform.anchoredPosition.x - 330f;
                        targetRotation = 180f;
                    }
                    else
                    {
                        targetX = rectTransform.anchoredPosition.x + 330f;
                        targetRotation = 0f;
                    }

                    DOTween.To(() => rectTransform.anchoredPosition.x,
                              x => rectTransform.anchoredPosition = new Vector2(x, rectTransform.anchoredPosition.y),
                              targetX, 0.5f);

                    if (arrow != null)
                    {
                        arrow.DORotate(new Vector3(0, 0, targetRotation), 0.5f)
                             .SetEase(Ease.Linear);
                    }
                    else
                    {
                        Debug.LogWarning("未找到箭头对象，请检查是否存在名为'Arrow'的子对象");
                    }

                    isOpen = !isOpen;
                }
                else
                {
                    Debug.LogError("任务界面没有RectTransform组件");
                }
            }
            InitMiniTaskUI();
        }

        void InitMiniTaskUI()//初始化任务UI
        {
            
        }

        public void Release()
        {

        }
    }
}

