namespace XUtils
{
    using UnityEngine;

    // 泛型单例类，继承自 MonoBehaviour
    public abstract class MonoSingleton<T> : MonoBehaviour where T : MonoBehaviour
    {
        // 静态私有实例，确保只有一个实例
        private static T instance;

        bool _IsFirstStart = false;
        // 公有静态属性，用于访问单例实例
        public static T Instance
        {
            get
            {
                if (instance == null)
                {
                    // 尝试在场景中查找该类型的实例
                    instance = FindFirstObjectByType<T>();

                    if (instance == null)
                    {
                        // 如果场景中没有，创建一个新的 GameObject 并添加该组件
                        GameObject singletonObject = new GameObject(typeof(T).Name);
                        instance = singletonObject.AddComponent<T>();
                    }
                }

                return instance;
            }
        }

        protected virtual void Awake()
        {
            if (instance == null)
            {
                instance = this as T;
                // 确保在场景切换时单例对象不会被销毁
                DontDestroyOnLoad(gameObject);
            }
            else
            {
                // 如果已经存在实例，销毁当前对象
                if (this != instance)
                {
                    Destroy(gameObject);
                }
            }
        }

        void Start()
        {
            if (!_IsFirstStart)
            {
                this.Initialize();
                _IsFirstStart = true;
            }
        }

        protected virtual void Initialize() { }
    }
}
