using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NaughtyAttributes;
using Cinemachine;

public class BaseCamera : MonoBehaviour
{
    [ReadOnly]
    public bool fix = false;
    [ReadOnly]
    public Vector3 posOffset;
    [ReadOnly]
    public Vector3 rotOffset;
    [ReadOnly]
    public Transform focusTrans;
    protected Matrix4x4 focusMat;
    protected Matrix4x4 translateMat;
    protected Vector3 mouseClick;
    public CinemachineVirtualCamera vCamera;
    // Start is called before the first frame update
    void Start()
    {
        translateMat = Matrix4x4.Translate(posOffset);
        GameApp.Instance.CurSceneCamera = this;

        if(null != CharactorManager.Instance.LocalPlayer)
        {
            SetFocusTrans(CharactorManager.Instance.LocalPlayer.transform);
        }

        CharactorManager.Instance.BindLocalPlayerAppear(OnLocalFocusTransAppear, true);
    }

    void OnLocalFocusTransAppear(Charactor _charactor, bool _appear)
    {
        if(_appear)
        {
            SetFocusTrans(_charactor.transform);
        }
        else
        {
            SetFocusTrans(null);
        }
    }

    public void SetFocusTrans(Transform trans)
    {
        //focusTrans = trans;
        vCamera.Follow = trans;
        vCamera.LookAt = trans;
    }

    private void OnDisable()
    {
        GameApp.Instance.CurSceneCamera = null;
        //CharactorManager.Instance.BindLocalPlayerAppear(OnLocalFocusTransAppear, false);
    }

//#if UNITY_EDITOR
//    private void Update()
//    {
//        if(Input.GetMouseButtonDown(1))
//        {
//            mouseClick = Input.mousePosition;
//        }
//    }
//#endif
//    void LateUpdate()
//    {
//        Quaternion q = Quaternion.identity;
//#if UNITY_EDITOR
//        if(Input.GetMouseButton(1))
//        {
//            Vector3 dtV = Input.mousePosition - mouseClick;
//            q = Quaternion.Euler(-dtV.y * 0.5f, dtV.x * 0.5f, 0);
//        }
//#endif

//        if(!fix && focusTrans)
//        {
//            focusMat.SetTRS(focusTrans.position, Quaternion.identity, Vector3.one);
//            translateMat.SetTRS(posOffset, Quaternion.identity, Vector3.one);
//            Matrix4x4 result = translateMat * focusMat;
//            transform.position = result.GetPosition();
//            transform.rotation = Quaternion.Euler(rotOffset) * q;
//        }
//        else if(fix)
//        {
//            if(focusTrans)
//            {
//                focusMat.SetTRS(focusTrans.position, Quaternion.identity, Vector3.one);
//                Matrix4x4 mat = Matrix4x4.Translate(transform.position);
//                posOffset = (mat * focusMat.inverse).GetPosition();
//            }
//            rotOffset = transform.rotation.eulerAngles;
//        }
//    }
}
