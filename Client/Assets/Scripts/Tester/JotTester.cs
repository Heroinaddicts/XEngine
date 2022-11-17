using System.Collections;
using System.Collections.Generic;
using System.Threading;
using Unity.Jobs;
using UnityEngine;

public class JotTester : MonoBehaviour
{

    struct Job : IJob
    {
        public void Execute()
        {
            Thread.Sleep(10);
        }
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        Job test = new Job();
        JobHandle handle = test.Schedule();
        JobHandle.ScheduleBatchedJobs();
        handle.Complete();
        Debug.Log("hello");
    }
}
