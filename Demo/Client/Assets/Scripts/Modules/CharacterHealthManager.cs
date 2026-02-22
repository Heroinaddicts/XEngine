// using System;
// using UnityEngine;
// using XEngine;
// using XGame;
// using static GameUnitApi;
// using System.Collections.Generic;
// using TMPro;
// using GameUnit;
// using UnityEngine.UI;

// public interface ICharacterHealthManager : IModule
// {
//     public void MpChange();
//     public void HpChange();
// }
// public class CharacterHealthManager : ICharacterHealthManager
// {
//     [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
//     static void Regist()
//     {
//         Launcher.RegistModule<ICharacterHealthManager, CharacterHealthManager>();
//     }
//     IClient _Client;
//     ICharacterManager _CharacterManager;
//     GameObject _CanvasBuffLab;

//     Int64 HPMax;
//     float MPMax;

//     public bool Initialize()
//     {
//         return true;
//     }

//     public bool Launch()
//     {
//         _Client = Launcher.GetModule<IClient>();
//         _CharacterManager = Launcher.GetModule<ICharacterManager>();
//         _CharacterManager.RegistEvent(ICharacterManager.eEvents.MainCharacterEnter, ClickMainUIOnMainCharacterEnter);


//         return true;
//     }

//     public void Release()
//     {

//     }

//     private void ClickMainUIOnMainCharacterEnter(IUnit unit)
//     {
//         _CanvasBuffLab = GameObject.FindFirstObjectByType<Canvas>().transform.gameObject;
//         Slider hpSlider = _CanvasBuffLab.transform.Find("UIMain/HeadIcon/HPLab/Slider").GetComponent<Slider>();
//         Slider mpSlider = _CanvasBuffLab.transform.Find("UIMain/HeadIcon/MPLab/Slider").GetComponent<Slider>();
//         TextMeshProUGUI hpMax = _CanvasBuffLab.transform.Find("UIMain/HeadIcon/HPLab/Max").GetComponent<TextMeshProUGUI>();
//         TextMeshProUGUI mpMax = _CanvasBuffLab.transform.Find("UIMain/HeadIcon/MPLab/Max").GetComponent<TextMeshProUGUI>();
//         TextMeshProUGUI hpCurrent = _CanvasBuffLab.transform.Find("UIMain/HeadIcon/HPLab/Current").GetComponent<TextMeshProUGUI>();
//         TextMeshProUGUI mpCurrent = _CanvasBuffLab.transform.Find("UIMain/HeadIcon/MPLab/Current").GetComponent<TextMeshProUGUI>();

//         IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();

//         mpSlider.maxValue = mainCharacter.GetFloat(eCharacterAttr.MaxMp_Float); ;
//         hpSlider.maxValue = mainCharacter.GetFloat(eCharacterAttr.MaxHp_Float);
//         hpSlider.value = mainCharacter.GetFloat(eCharacterAttr.Hp_Float);
//         mpSlider.value = mainCharacter.GetFloat(eCharacterAttr.Mp_Float);
//        // Debug.LogError($"HPMax {hpSlider.maxValue} MPMax {mpSlider.maxValue},currHP {hpSlider.value} currMP {mpSlider.value}");
//         hpMax.text = "/" + hpSlider.maxValue.ToString();
//         mpMax.text = "/" + mpSlider.maxValue.ToString();
//         hpCurrent.text = hpSlider.value.ToString();
//         mpCurrent.text = mpSlider.value.ToString();

//     }

//     public void MpChange()
//     {
//         Slider MPSlider = _CanvasBuffLab.transform.Find("UIMain/HeadIcon/MPLab/Slider").GetComponent<Slider>();
//         IUnit unit = _CharacterManager.GetMainCharacterUnit();
//         MPSlider.value = unit.GetFloat(eCharacterAttr.Mp_Float);
//     }

//     public void HpChange()
//     {
//         Slider HPSlider = _CanvasBuffLab.transform.Find("UIMain/HeadIcon/HPLab/Slider").GetComponent<Slider>();
//         IUnit unit = _CharacterManager.GetMainCharacterUnit();
//         HPSlider.value = unit.GetFloat(eCharacterAttr.Hp_Float);
//     }

// }