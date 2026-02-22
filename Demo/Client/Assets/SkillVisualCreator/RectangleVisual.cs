using UnityEngine;

namespace SkillVisual
{
    class RectangleVisual : ISkillVisual
    {
        float _Angle;
        public override eSkillVisual Type { get { return eSkillVisual.Rectangle; } }
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
