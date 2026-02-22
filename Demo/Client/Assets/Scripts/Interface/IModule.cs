namespace XGame
{
    public interface IModule
    {
        bool Initialize();
        bool Launch();
        void Release();

        public void Update()
        {

        }

        public void FixedUpdate()
        {

        }

        public void LateUpdate()
        {

        }
    }
}