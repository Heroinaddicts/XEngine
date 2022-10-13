using UnityEngine;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

public class EventTester : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {

    }

    private void OnEnable()
    {
//         Module.GetModule<Events>().Register<JObject>(Ev.eID.Test, OnEventTest);
//         Module.GetModule<Events>().Register<JObject>(Ev.eID.Test, OnEventTest);
    }

    private void OnDisable()
    {
        //Module.GetModule<Events>().Cancellation<JObject>(Ev.eID.Test, OnEventTest);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnEventTest(in JObject o)
    {
        Debug.Log("events test");
    }
}
