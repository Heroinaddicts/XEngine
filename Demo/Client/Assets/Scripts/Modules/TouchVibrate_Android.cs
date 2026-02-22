using Unity.VisualScripting;
using UnityEngine;
using XGame;

namespace XGameClient
{
    public interface IAndroidVibrateManager : IModule
    {

        public void SettingVibrateByLevel(int levelNum);//设置震动等级
        public void StopVibration();//停止震动
        public void OnceVibration(long milliseconds);//一次震动,不可调整震动力度，不限制安卓版本使用
        public void VibrateWithIntensity(long milliseconds, int intensity);//自定义震动，可调整震动力度，安卓8.0以上可用，8.0以下只可模拟效果
        public bool VibrationSupportCheck();//检查是否支持震动
    }

    public class AndroidVibrateManager : IAndroidVibrateManager
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<IAndroidVibrateManager, AndroidVibrateManager>();
        }

        AndroidJavaObject _AndroidVibrate;

        public bool Initialize()
        {
            if (Application.platform == RuntimePlatform.Android)
            {
                try
                {
                    _AndroidVibrate = new AndroidJavaObject("com.example.android2unity.Android2U");
                }
                catch (System.Exception e)
                {
                    Debug.LogError("初始化失败，检查JAVA路径和代码" + e.Message);
                }
            }
            return true;
        }
        public bool Launch()
        {
            return true;
        }

        public void Release()
        {

        }

        /// <summary>
        /// 传入震动等级，已经预设好1，2，3等级，安卓8.0以上可用
        /// </summary>
        /// <param name="SettingVibrateByLevel"></param>
        public void SettingVibrateByLevel(int levelNum)
        {
            if (levelNum <= 0 && levelNum >= 3)
            {
                Debug.LogError("震动等级参数错误，检查范围1~3" + levelNum);
                return;
            }
            if (levelNum == 1)
            {
                _AndroidVibrate.Call("SettingVibrateByLevel", 1);
            }
            else if (levelNum == 2)
            {
                _AndroidVibrate.Call("SettingVibrateByLevel", 2);
            }
            else if (levelNum == 3)
            {
                _AndroidVibrate.Call("SettingVibrateByLevel", 3);
            }
        }

        /// <summary>
        /// 是否支持震动,返回布尔值，安卓使用
        /// </summary>
        /// <returns></returns>
        public bool VibrationSupportCheck()
        {
            return _AndroidVibrate.Call<bool>("isVibrationSupported");
        }

        /// <summary>
        /// 停止震动,不限安卓使用
        /// </summary>
        public void StopVibration()
        {
            _AndroidVibrate.Call("stopVibration");
        }

        /// <summary>
        /// 一次震动,不可调整震动力度，不限制安卓版本使用
        /// </summary>
        public void OnceVibration(long milliseconds)
        {
            _AndroidVibrate.Call("OnceVibration", milliseconds);
        }


        /// <summary>
        /// 自定义震动，可调整震动力度，安卓8.0以上可用，8.0以下只可模拟效果
        /// </summary>
        /// <param name="milliseconds"></param>
        /// <param name="intensity"></param>
        public void VibrateWithIntensity(long milliseconds, int intensity)
        {
            _AndroidVibrate.Call("VibrateWithIntensity", milliseconds, intensity);
        }
    }
}