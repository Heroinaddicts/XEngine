using System;
using UnityEngine;
using XGame;
using UnityEngine.UI;
using GameUnit;
using XEngine;
using XUtils;
using static GameUnitApi;
using Spine.Unity;


namespace XGameClient
{
    public interface ISpineAnimationController : IModule
    {
        enum eEvent
        {


        }
        public void HandleAnimation(IUnit unit, string targetAnimation, bool loop);

        public void FlipCharacterToDirection(IUnit unit, float moveDirectionX);
    }

    class SpineAnimationController : ISpineAnimationController
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<ISpineAnimationController, SpineAnimationController>();
        }

        ICharacterManager _CharacterManager;
        XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {

            return true;
        }

        public void Release()
        {

        }

        public void HandleAnimation(IUnit unit, string targetAnimation, bool loop = true)
        {
            if (unit == null)
            {
                XApi.Debug("缺少动画单位");
                return;
            }
            SkeletonAnimation skeletonAnimation = unit.gameObject.GetComponent<SkeletonAnimation>();
            if (skeletonAnimation == null)
            {
                XApi.Debug("没找到Spine动画组件");
                return;
            }
            Spine.Animation animationObject = skeletonAnimation.Skeleton.Data.FindAnimation(targetAnimation);
            if (animationObject == null)
            {
                XApi.Debug($"找不到动画: {targetAnimation}");
                return;
            }

            if (targetAnimation != skeletonAnimation.AnimationName)
            {
                skeletonAnimation.AnimationState.SetAnimation(0, animationObject, loop);
                if (unit.Type == eUnitType.Monster)
                {      
                    //Debug.LogError($"播放动画 {targetAnimation}， 循环: {loop}");
                }
            }
            else
            {
                Spine.TrackEntry currentTrack = skeletonAnimation.AnimationState.GetCurrent(0);
                if (currentTrack != null && currentTrack.Loop != loop)
                {
                    currentTrack.Loop = loop;
                }
            }
        }

        public void FlipCharacterToDirection(IUnit unit, float JoystickAngle)//根据摇杆角度翻转角色方向
        {
            if (unit == null || JoystickAngle == 0)
            {
                return;
            }

            SkeletonAnimation skeletonAnimation = unit.gameObject.GetComponent<SkeletonAnimation>();
            if (unit.Type == eUnitType.Bullet)
            {
                Debug.LogWarning("子弹不支持方向翻转");
                return;
            }
            if (skeletonAnimation == null)
            {
                Debug.LogWarning("没找到Spine动画组件");
                return;
            }
            float horizontal = Mathf.Cos(JoystickAngle * Mathf.Deg2Rad);

            if (horizontal > 0.01f && skeletonAnimation.Skeleton.ScaleX < 0)
            {
                skeletonAnimation.Skeleton.ScaleX *= -1;
            }

            else if (horizontal < -0.01f && skeletonAnimation.Skeleton.ScaleX > 0)
            {
                skeletonAnimation.Skeleton.ScaleX *= -1;
            }
        }

    }
}
