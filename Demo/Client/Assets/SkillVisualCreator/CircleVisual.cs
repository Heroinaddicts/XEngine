using UnityEngine;

namespace SkillVisual
{
    class CircleVisual : ISkillVisual
    {
        float _Angle;
        public override eSkillVisual Type { get { return eSkillVisual.Circle; } }
        public override float Angle
        {
            get
            {
                return _Angle;
            }
            set
            {
                _Angle = value;
                transform.rotation = Quaternion.Euler(_Angle+90f, 90f, 90f);
            }
        }
    }
}
