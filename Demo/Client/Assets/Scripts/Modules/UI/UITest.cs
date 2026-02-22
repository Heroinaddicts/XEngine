using System;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using static GameUnitApi;
public class UITest : MonoBehaviour
{
    [SerializeField]
    Vector2 _Position;

    [SerializeField]
    InputField _BuffConfigID;

    public void OnBtnTestClick()
    {
        Launcher.GetModule<IMovement>().MainCharacterMoveto(_Position, (in bool ret) =>
        {
            XApi.Debug($"Arrived {ret}");
        });
    }

    public void OnBtnTestBuff()
    {
        oBuffTest test = new oBuffTest();
        test.BuffConfigId = Int64.Parse(_BuffConfigID.text);
        Launcher.GetModule<IClient>().SendProtobuf(eClientID.BuffTest, test);
    }

    //public void OnBtnTestBuff()
    //{
    //    oLauncherTest test = new oLauncherTest();
    //    test.LauncherConfigId = Int64.Parse(_BuffConfigID.text);
    //    Launcher.GetModule<IClient>().SendProtobuf(eClientID.LauncherTest, test);
    //}

    [SerializeField]
    LineRenderer _LineRenderer;
    public void OnBtnRaycastTest()
    {
        IUnit mainCharacterUnit = Launcher.GetModule<ICharacterManager>().GetMainCharacterUnit();
        IGameTilemap gtp = Launcher.GetModule<IGameSceneManager>().GetCurrentTilemap();

        Vector2 hitPoint = new Vector2();
        gtp.Raycast(mainCharacterUnit.Position, mainCharacterUnit.Angle, 10000, ref hitPoint);

        _LineRenderer.startWidth = 0.1f;
        _LineRenderer.endWidth = 0.1f;
        _LineRenderer.positionCount = 2;
        _LineRenderer.SetPosition(0, mainCharacterUnit.Position);
        _LineRenderer.SetPosition(1, hitPoint);
    }
}
