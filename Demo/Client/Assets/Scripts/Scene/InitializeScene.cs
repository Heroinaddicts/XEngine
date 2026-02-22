using UnityEngine;
using UnityEngine.SceneManagement;
using XEngine;

public class InitializeScene : IScene<InitializeScene>
{
    public void OnBtnConfirmClick()
    {
        Launcher.GetModule<IGameSceneManager>().AsyncLoadScene("LoginScene");
    }
}
