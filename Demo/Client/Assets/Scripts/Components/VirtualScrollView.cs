using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
class VirtualScrollView : GameObjectPool
{
    [Header("引用")]
    public ScrollRect _ScrollRect;  // ScrollView 的 ScrollRect 组件
    public RectTransform _ViewPort; // 视口
    public RectTransform _Content; // Content
    public RectTransform _Placeholder; // 占位区域
    public GameObject _ItemPrefab; // Item 预制体
    public Transform _ItemParent; // Item 的父节点（通常是 Content）

    [Header("配置")]
    public float _ItemHeight = 100; // 单个 Item 的高度
    public int _BufferCount = 2; // 视野外缓冲的 Item 数量（避免快速滚动空白）

    // 内部变量
    //     private List<YourData> allData; // 所有数据
    //     private List<ItemView> activeItems = new List<ItemView>(); // 当前显示的 Item
    //     private ObjectPool<ItemView> itemPool; // Item 对象池
    private int _VisibleCount; // 可视区域能容纳的 Item 数量
    private float _ViewPortHeight; // 视口高度

    public override GameObject CreateGameObject()
    {
        return new GameObject();
    }
}