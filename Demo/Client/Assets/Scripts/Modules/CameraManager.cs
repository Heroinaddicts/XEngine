using UnityEngine;
using XGame;
using static GameUnitApi;

public interface ICameraManage : IModule
{
}


public class CameraManager : ICameraManage
{

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<ICameraManage, CameraManager>();
    }


    ICharacterManager _CharacterManager;
    public bool Initialize()
    {
        return true;
    }


    public bool Launch()
    {
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        return true;
    }


    public void Release()
    {

    }

    public void Update()
    {
        IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
        if (null != mainCharacter)
        {
            Camera.main.transform.position = new Vector3(mainCharacter.transform.position.x, mainCharacter.transform.position.y, -10);
        }
    }
}
