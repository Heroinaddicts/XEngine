using UnityEngine;
using XEngine;
using XGame;

public class LoginScene : IScene<LoginScene>
{
    public void OnBtnLoginClic()
    {
        Launcher.GetModule<IClient>().Login(eAccountType.Trial);
    }
}
