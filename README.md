# XEngine API 使用手册

面向对象: 服务器逻辑开发者  
阅读目标: 看完后，能够独立阅读 `XEngine` 的接口，写出自己的业务模块，并且知道各种 API 应该在什么场景下使用。

---

## 1. 这份手册是给谁看的

这份手册是写给两类人的:

1. 刚接触这个项目的同学
2. 已经会写 C++，但还不熟悉 `XEngine` 的同学

如果你是第一次看这个工程，可以先记住一句最重要的话:

> `XEngine` 不是让你“写多线程逻辑”的框架，而是一个“主线程驱动逻辑，异步系统帮你干脏活累活”的服务端运行时。

换句话说:

- 业务逻辑主要写在主线程
- 实体状态主要在主线程修改
- 模块之间的协作主要发生在主线程
- `Task`、`Http`、`Mysql`、`Redis` 这些异步能力，主要负责把阻塞工作搬走，再把结果送回主线程

这就是理解整个 `XEngine` 的总钥匙。

---

## 2. 一句话理解 XEngine

可以把 `XEngine` 想成下面这个结构:

```text
                +----------------------+
                |      Logic 模块      |
                |  Gate / Game / ...   |
                +----------+-----------+
                           |
                           | 全部在主线程里组织业务
                           |
         +-----------------+-----------------+
         |                 |                 |
         v                 v                 v
     Timer API         Net API          Engine API
   主线程时间调度      主线程收发事件      总入口/模块定位

         +-----------------+-----------------+
         |                 |                 |
         v                 v                 v
      Task API         Http API         Mysql/Redis API
    后台做重任务       后台发请求         后台做阻塞 IO
      回主线程           回主线程           回主线程
```

所以你在写逻辑时，不要把它想成“线程池框架”，而要把它想成:

> “我在主线程发起一件事，底层帮我异步做，做完后再回到主线程告诉我结果。”

---

## 3. 最核心的编程模型

### 3.1 主线程原则

对于逻辑开发者来说，请默认以下规则成立:

- 你调用 API 的地方在主线程
- 你收到大多数业务回调的地方也在主线程
- 你修改角色、怪物、场景、背包、任务等业务状态，也应该在主线程

这套模式的好处非常大:

- 逻辑代码更好理解
- 不容易出现并发读写问题
- 业务状态更容易保持一致
- 模块之间的协作更直接

### 3.2 异步能力的真实作用

为什么还需要异步系统?

因为有些事情如果放在主线程做，会把整个服务器卡住。比如:

- 连数据库
- 跑 SQL
- 请求外部 HTTP 服务
- 连 Redis
- 做很重的计算
- 读写耗时资源

这些事情不适合直接在主线程做，所以 `XEngine` 把它们放到后台线程或后台系统里做。  
但是它不会让你直接去写复杂的并发逻辑，而是把结果重新送回主线程。

### 3.3 你可以把它记成一句口诀

> 重活在后台做，业务在主线程收。

---

## 4. XEngine 的启动与主循环

### 4.1 启动顺序

`XEngine` 的启动顺序大致是:

1. 初始化日志
2. 初始化时间系统
3. 初始化网络系统
4. 初始化 HTTP 系统
5. 初始化 MySQL 系统
6. 初始化任务系统
7. 初始化逻辑模块系统

也就是说，逻辑模块不是最底层，它是建立在这些基础系统之上的。

### 4.2 主循环分成三段

整个服务器每一帧都会跑三段:

1. `EarlyUpdate`
2. `Update`
3. `LaterUpdate`

你可以把它们粗略理解为:

- `EarlyUpdate`: 先处理一批比较靠前的系统事件
- `Update`: 主逻辑阶段
- `LaterUpdate`: 收尾和延后回调阶段

### 4.3 常见回调大概发生在哪一段

从实现上看，逻辑开发者最关心的是下面这些:

| 系统 | 回调大致发生阶段 | 你可以怎么理解 |
| --- | --- | --- |
| `Net` | `EarlyUpdate` | 网络事件先到主线程 |
| `Timer` | `Update` | 定时器在主逻辑阶段触发 |
| `Task` | `Update` | 后台任务做完后，在主线程回调 |
| 业务模块 `Update` | `Update` | 你自己的逐帧逻辑 |
| `Http` | `LaterUpdate` | HTTP 结果偏后回到主线程 |
| `Mysql` | `LaterUpdate` | SQL 查询结果偏后回到主线程 |
| `Redis` | 走 `Task` 回调 | 本质也是“后台做完，主线程通知” |

你不需要死记每一个阶段的精确顺序，但要记住一个大方向:

> 最终业务回调是有节奏地回到主线程，而不是在后台线程里直接乱飞。

---

## 5. 先认识几个最重要的接口

在 `XEngine` 里，逻辑开发者最常接触的接口是这几个:

- `iEngine`
- `iModule`
- `iTimerApi`
- `iTaskApi`
- `iNetApi`
- `iHttpApi`
- `iMysqlApi`
- `iRedisApi`

下面先用一句话说明每个接口的职责:

| 接口 | 用途 |
| --- | --- |
| `iEngine` | 引擎总入口，拿其他系统 API、取参数、打日志、找模块 |
| `iModule` | 逻辑模块生命周期接口 |
| `iTimerApi` | 主线程定时器 |
| `iTaskApi` | 后台任务桥，做耗时工作 |
| `iNetApi` | TCP 网络能力 |
| `iHttpApi` | HTTP 请求能力 |
| `iMysqlApi` | MySQL 连接与查询能力 |
| `iRedisApi` | Redis 异步命令能力 |

接下来我们逐个讲。

---

## 6. `iModule`: 逻辑模块是怎么写出来的

### 6.1 什么叫模块

在 `XEngine` 里，业务代码不是一股脑写在 `main()` 里，而是拆成多个模块。  
例如你可能会见到这些模块:

- `Gate`
- `Game`
- `PlayerManager`
- `SceneManager`
- `SkillManager`

每个模块都像一个独立的小系统，负责自己的一块业务。

### 6.2 `iModule` 的核心生命周期

一个模块通常会经历这几个阶段:

1. `Initialize`
2. `Launch`
3. `Update`
4. `Destroy`

可以这样理解:

| 生命周期函数 | 作用 |
| --- | --- |
| `Initialize` | 绑定引擎、拿其他模块、做基础准备 |
| `Launch` | 真正启动逻辑，注册监听、开 timer、建连接等 |
| `Update` | 每帧执行一次的主线程逻辑 |
| `Destroy` | 释放资源、清理状态 |

### 6.3 一个模块最常见的写法

```cpp
class Foo final : public IFoo, public XEngine::Api::iModule
{
public:
    bool Initialize(XEngine::Api::iEngine* const engine) override
    {
        _Engine = engine;
        return true;
    }

    bool Launch(XEngine::Api::iEngine* const engine) override
    {
        return true;
    }

    void Update(XEngine::Api::iEngine* const engine) override
    {
    }

    void Destroy(XEngine::Api::iEngine* const engine) override
    {
    }

private:
    XEngine::Api::iEngine* _Engine = nullptr;
};
```

### 6.4 `Initialize` 和 `Launch` 的区别

很多新同学一开始会把这两个搞混。

你可以这样分:

- `Initialize`: “我先把自己准备好”
- `Launch`: “我开始真正工作了”

举例:

- 在 `Initialize` 里保存 `engine`
- 在 `Initialize` 里拿其他模块指针
- 在 `Launch` 里启动 timer
- 在 `Launch` 里开网络监听
- 在 `Launch` 里连数据库

这样的分工会更清楚。

### 6.5 模块是如何被引擎找到的

模块不是你手动 `new` 完再塞给引擎的。  
这个工程使用了一套模块导出和加载机制:

- 模块在动态库里注册
- 逻辑组件把动态库加载进来
- 引擎把模块按名字放入表里
- 其他模块可以通过 `GetModule` 宏按名字拿接口

所以站在逻辑开发者角度，你最常见的用法是:

```cpp
GetModule(engine, g_Config, Config);
GetModule(engine, g_PlayerManager, PlayerManager);
```

这句话的含义不是“创建模块”，而是:

> “从已经加载好的模块表里，把对应名字的模块接口取出来。”

---

## 7. `iEngine`: 所有系统的总入口

### 7.1 `iEngine` 是什么

`iEngine` 可以理解成服务器里的“总控制台”。  
很多能力都从这里拿:

- 取启动参数
- 取子系统 API
- 打日志
- 关闭服务器
- 启动同构进程
- 按名字找模块

### 7.2 你最常用的几类功能

#### 第一类: 取系统 API

这是最常见的用法:

```cpp
auto timerApi = engine->GetTimerApi();
auto taskApi = engine->GetTaskApi();
auto netApi = engine->GetNetApi();
auto mysqlApi = engine->GetMysqlApi();
```

这说明 `iEngine` 本身不是业务系统，而是业务系统的入口。

#### 第二类: 取启动参数

服务器启动时支持传参数，例如:

- `--name=Gate_1`
- `--modules=Gate,Game`
- `--core_xml=xxx.xml`
- `--module_path=...`

逻辑层可以通过 `engine->GetLaunchParam(...)` 读取这些参数。

这类参数最常用来做:

- 区分进程实例
- 读取启动配置
- 判断当前加载哪些模块

#### 第三类: 打日志

`iEngine` 还提供统一日志能力。  
对逻辑开发者来说，重点不是日志系统内部怎么实现，而是:

- 你通过引擎打日志
- 不要自己随意搞一套独立日志体系

#### 第四类: 关服和进程拉起

`iEngine` 还有下面这类“宿主级能力”:

- `Shutdown()`
- `LaunchApp(...)`

这些能力一般不是普通业务模块高频使用的，但做管理、守护、工具模块时可能会用到。

### 7.3 对 `iEngine` 的最佳理解

请把它记成:

> `iEngine` 是宿主门面，不是业务容器。

意思是:

- 它帮你进入系统
- 它帮你拿工具
- 它帮你找其他模块
- 但真正的业务状态，不应该都堆在 `iEngine` 里

---

## 8. `iTimerApi`: 主线程定时器系统

### 8.1 什么时候该用 Timer

当你需要“过一会儿再做某件事”或者“每隔一段时间做某件事”时，就应该想到 `Timer`。

典型场景:

- 每秒刷新一次在线状态
- 5 秒后关闭一个对象
- 每 100ms 做一次 AI 检查
- 过 30 秒让副本超时

### 8.2 它和线程有什么关系

对逻辑开发者来说，`Timer` 回调是在主线程触发的。  
这件事非常重要。

所以你在 `OnTimer()` 里:

- 可以安全地改业务状态
- 可以安全地访问大多数主线程容器
- 可以直接调用别的逻辑模块

你不需要把它想成“后台定时器线程”。

### 8.3 最常用的接口长什么样

`XEngine` 给了两个很好用的宏:

```cpp
START_TIMER(engine, this, id, delay, count, interval, context);
STOP_TIMER(engine, this, id, context);
```

含义如下:

| 参数 | 意义 |
| --- | --- |
| `engine` | 引擎指针 |
| `this` | 当前 timer 挂在哪个对象上 |
| `id` | 定时器编号 |
| `delay` | 第一次触发前等待多久 |
| `count` | 触发次数 |
| `interval` | 后续触发间隔 |
| `context` | 额外上下文 |

### 8.4 `count` 怎么理解

`count` 表示这个定时器要触发几次。

常见情况:

- `1`: 只触发一次
- `Api::Unlimited`: 无限次循环

所以:

- 一次性延时任务，用 `count = 1`
- 周期任务，用 `count = Api::Unlimited`

### 8.5 `context` 是干什么的

`context` 是一个 `UInt64`，本质上是让你给 timer 带一点额外信息回来。

常见写法:

- 带一个角色 `Guid`
- 带一个怪物 `Guid`
- 带一个枚举值
- 带一个对象指针强转后的值

例如:

```cpp
START_TIMER(engine, this, 1, 1000, 1, 0, playerGuid);
```

1 秒后回调时，你就知道这个 timer 是给哪个玩家准备的。

### 8.6 Timer 的唯一键规则

这是一个很重要但很容易忽略的点:

> 一个 timer 的唯一键不是只有 `id`，而是 `(timer对象, id, context)` 的组合。

也就是说:

- 同一个对象
- 同一个 `id`
- 同一个 `context`

这三者都一样时，才被认为是“同一个 timer”。

重新 `StartTimer` 时，引擎会先把同键旧 timer 干掉，再创建一个新的。

这套规则的好处是:

- 你可以给一个对象挂多个 timer
- 同一个 `id` 也能通过不同 `context` 区分多个实例

### 8.7 回调有哪些

定时器对象通常实现 `iTimer` 接口，会收到这些回调:

- `OnStart`
- `OnTimer`
- `OnEnd`
- `OnPause`
- `OnResume`

其中最常用的是:

- `OnTimer`: 真正触发时执行
- `OnEnd`: timer 结束时执行

### 8.8 一个完整示例

```cpp
class Foo final : public XEngine::Api::iTimer
{
public:
    void Start(XEngine::Api::iEngine* engine, XEngine::UInt64 guid)
    {
        START_TIMER(engine, this, 1, 1000, XEngine::Api::Unlimited, 1000, guid);
    }

    void OnTimer(const int id, const XEngine::UInt64 context, const XEngine::Int64 tick) override
    {
        // 每秒执行一次
        // context 里带回了 guid
    }

    void OnEnd(const int id, const XEngine::UInt64 context, const bool nonviolent, const XEngine::Int64 tick) override
    {
        // 定时器自然结束或被强制停掉时执行
    }
};
```

### 8.9 什么时候优先选 Timer

下面这些情况，优先考虑 `Timer`:

- 纯时间驱动逻辑
- 周期性检查
- 延迟执行
- 状态超时
- AI tick

如果你的需求是“做一段耗时计算”，那就不该优先用 `Timer`，而更应该用 `Task`。

---

## 9. `iTaskApi`: 后台任务桥

### 9.1 为什么需要 Task

有些事情不适合在主线程里做，因为会卡住整个服务器，例如:

- 很慢的文件处理
- 很慢的第三方库调用
- 很重的计算
- 需要等待的阻塞操作

这些工作应该放到后台去做。  
这就是 `TaskApi` 的用途。

### 9.2 它不是让你“随意开线程”

很多人第一次看到 `Task`，容易想成“那我是不是可以在线程里把角色数据改了?”

不建议这么做。

正确理解应该是:

- 后台线程负责做耗时工作
- 做完以后，把结果带回主线程
- 真正的业务状态更新，在主线程完成回收

### 9.3 最常见的调用形式

```cpp
engine->GetTaskApi()->PushTask(
    XEngine::Api::eTaskProcessorType::OrderedQueue,
    [](XEngine::Api::iTaskContext& context) -> XEngine::Int64
    {
        // 后台线程执行
        return 0;
    },
    [](const XEngine::Int64 code, const XEngine::Api::iTaskContext& context)
    {
        // 主线程执行
    }
);
```

这个接口里最重要的是两个回调:

| 回调 | 在哪里执行 |
| --- | --- |
| `fTask` | 后台线程 |
| `fTaskCompleted` | 主线程 |

### 9.4 `iTaskContext` 是干什么的

`iTaskContext` 可以理解成一个“小信封”，用来把后台阶段算出来的结果，带回主线程阶段。

它本身很轻量，核心就是存一个指针。

你可以这样理解:

1. 在后台阶段 `new` 一个结果对象
2. 把指针塞进 `context`
3. 在主线程完成回调里取出来
4. 用完后自己释放

### 9.5 `mask` 的意义

`TaskApi` 里有一个常被忽视的概念: `mask`

它不是权限，也不是状态位。  
它更接近“路由 key”。

作用是:

> 让某些相关任务尽量落到同一条后台处理 lane 上，保持相对串行。

比如:

- 同一个账号的任务
- 同一个房间的任务
- 同一个玩家的存档任务

如果你希望它们不要乱序交错，可以给它们同一个 `mask`。

### 9.6 `OrderedQueue` 和 `SeparateThread`

`TaskApi` 支持不同处理方式，最常见的有:

| 类型 | 理解方式 |
| --- | --- |
| `OrderedQueue` | 进入后台任务队列 |
| `SeparateThread` | 单独开线程执行 |

一般业务更常用 `OrderedQueue`。  
除非你非常明确知道为什么需要单独线程，否则不要一上来就选 `SeparateThread`。

### 9.7 什么时候应该用 Task

这些场景适合用 `Task`:

- 需要耗时计算
- 需要调用阻塞式库
- 需要把慢操作搬出主线程
- 你自己想封装一套“后台算，主线程收”的工具流程

不太适合直接用 `Task` 的场景:

- 单纯延时
- 周期循环
- 普通网络收包分发
- 主线程轻量逻辑

### 9.8 一个更完整的例子

```cpp
struct QueryResult
{
    int Value = 0;
};

engine->GetTaskApi()->PushTask(
    XEngine::Api::eTaskProcessorType::OrderedQueue,
    [](XEngine::Api::iTaskContext& ctx) -> XEngine::Int64
    {
        auto* result = XEngine::txnew QueryResult();
        result->Value = 42;
        ctx.SetContext(result);
        return 0;
    },
    [](const XEngine::Int64 code, const XEngine::Api::iTaskContext& ctx)
    {
        auto* result = static_cast<QueryResult*>(ctx.GetContext());
        if (result != nullptr)
        {
            // 主线程里使用结果
            XEngine::txdel result;
        }
    }
);
```

### 9.9 `iTaskGroup` 是什么

有时候你不是只想做一个任务，而是想同时发起多个任务，等它们全都回来以后，再做最后一步收尾。

这时候可以用 `iTaskGroup`。

你可以把它理解成:

> “一组任务的集合，所有任务做完后，再统一通知我一次。”

适合场景:

- 同时拉多个分片数据
- 同时向多个服务查询
- 并行准备多块资源，再统一组装

---

## 10. `iNetApi`: TCP 网络系统

### 10.1 它解决什么问题

`iNetApi` 主要负责 TCP 通信。

在游戏服务端里，最常见的用法有两种:

1. 主动连别的服务器
2. 自己开监听，接收别人的连接

对应的角色是:

- `iTcpSession`: 一条连接对应的逻辑对象
- `iTcpServer`: 监听器对象
- `iTcpPipe`: 已建立连接后的运行时通道

### 10.2 先建立一个直观理解

可以这样理解这三者:

| 对象 | 作用 |
| --- | --- |
| `iTcpServer` | “门卫”，负责开门接客 |
| `iTcpSession` | “会话逻辑对象”，负责这一条连接的业务语义 |
| `iTcpPipe` | “底层管道”，负责真正发送和关闭 |

最实用的记忆方式是:

> `Session` 管逻辑，`Pipe` 管链路。

### 10.3 主动连接模式

如果你想主动连接另一个服务，一般会:

1. 自己写一个继承 `iTcpSession` 的类
2. 调 `LaunchTcpSession`
3. 等连接建立
4. 在 `OnConnected`、`OnReceive` 里处理业务

### 10.4 监听模式

如果你想开一个 TCP 服务端，一般会:

1. 自己写一个继承 `iTcpServer` 的类
2. 实现 `OnMallocTcpSession`
3. 调 `LaunchTcpServer`
4. 新连接到来时，引擎会让你创建一个新的 session

### 10.5 `OnReceive` 为什么这么重要

`OnReceive(const void* content, int size)` 是整个 TCP 接口里最关键的函数之一。

因为它的返回值不是“成功还是失败”，而是:

> “这次我消费掉了多少字节。”

这个设计意味着:

- 底层负责把字节流送上来
- 逻辑层自己负责拆包
- 你告诉引擎: 我已经处理了前面多少字节

### 10.6 大学生一定要理解的一个概念: TCP 是字节流

TCP 不会帮你保留消息边界。  
它只保证字节按顺序到达。

所以你一次 `OnReceive` 里拿到的数据可能是:

- 半个包
- 一个完整包
- 两个包拼在一起
- 一个半包加下一个包的开头

这就是为什么 `OnReceive` 要返回“消费字节数”。

### 10.7 一个最常见的拆包思路

假设协议规定:

- 前 4 个字节是包长
- 后面是正文

那么逻辑大概会这样写:

```cpp
int MySession::OnReceive(const void* content, const int size)
{
    if (size < 4)
    {
        return 0;
    }

    const int packetSize = ReadPacketSize(content);
    if (size < packetSize)
    {
        return 0;
    }

    HandlePacket(content, packetSize);
    return packetSize;
}
```

如果返回 `0`，就表示:

> “数据还不够，我这次先不消费，等后面更多数据到了再说。”

### 10.8 `iTcpSession` 常用接口

会话里最常用的方法通常有:

- `Send(...)`
- `Close()`
- `Pause()`
- `Resume()`
- `RemoteIp()`
- `RemotePort()`
- `IsActiveConnect()`

常见理解如下:

| 方法 | 含义 |
| --- | --- |
| `Send` | 往这条连接发数据 |
| `Close` | 主动关闭连接 |
| `Pause` | 暂停读写或暂停处理 |
| `Resume` | 恢复 |
| `RemoteIp` | 对端 IP |
| `RemotePort` | 对端端口 |
| `IsActiveConnect` | 这条连接是不是我主动发起的 |

### 10.9 连接事件也回到主线程

从逻辑使用角度，你可以这样理解:

- `OnConnected` 在主线程
- `OnDisconnect` 在主线程
- `OnReceive` 在主线程

所以你在这些回调里做会话状态更新，是符合这套框架设计的。

### 10.10 最适合 `NetApi` 的场景

- 客户端网关连接
- 服务间 TCP 通信
- 内部转发链路
- 长连接协议

如果你只是想调一个网页接口，那不是 `NetApi` 的主要使用场景，而更适合 `HttpApi`。

---

## 11. `iHttpApi`: HTTP 请求系统

### 11.1 它适合做什么

`HttpApi` 适合做“向外部 HTTP 服务发请求”这类事情，例如:

- 登录服调平台验证
- 请求第三方接口
- 发 webhook
- 拉取远端配置
- 上传或下载某些资源

### 11.2 基本使用流程

使用方式通常是四步:

1. `CreateHttpRequest()`
2. 配置请求参数
3. `Do(url, method, callback)`
4. 在主线程回调中处理结果

### 11.3 常见配置方法

一个请求对象通常支持下面这些配置:

- `SetKeepConnection`
- `AppendHeader`
- `SetParameter`
- `SetPostFileds`
- `PostFile`

这些名字已经很直白了，可以这样理解:

| 方法 | 作用 |
| --- | --- |
| `SetKeepConnection` | 是否保持连接 |
| `AppendHeader` | 添加 HTTP Header |
| `SetParameter` | 设置参数 |
| `SetPostFileds` | 设置 POST 文本内容 |
| `PostFile` | 上传文件 |

### 11.4 请求真正在哪里执行

这里的关键点是:

- 你在主线程上配置请求
- 真正发请求是在后台线程里
- 请求完成后，结果回到主线程

所以它的使用感受很像:

> “我先把一张表单填好，交给后台同学去送；送完以后，后台同学回来告诉我结果。”

### 11.5 回调里能拿到什么

HTTP 回调大致会给你这些内容:

- 是否成功
- 响应头
- 响应体

响应体使用的是 `IBuffer`。

### 11.6 `IBuffer` 怎么看

`IBuffer` 可以理解成一个数据缓冲区。  
你通常可以通过:

- `GetData()`
- `GetDataSize()`

来取出它的内容。

例如:

```cpp
request->Do(url, XEngine::Api::GET, [](bool success, const std::map<std::string, std::string>& headers, XEngine::Api::IBuffer& buffer)
{
    if (!success)
    {
        return;
    }

    const char* data = reinterpret_cast<const char*>(buffer.GetData());
    const int size = buffer.GetDataSize();
    // 这里解析 data
});
```

### 11.7 关于生命周期的一个重要提醒

`IBuffer` 是请求对象持有的。  
所以如果你只是想“当场解析”，直接用没问题。  
但如果你想把内容留到以后再用，最好自己拷贝一份。

简单说:

- 当场用，没问题
- 想长期保存，自己复制

### 11.8 一个标准示例

```cpp
auto* request = engine->GetHttpApi()->CreateHttpRequest();
request->AppendHeader("Content-Type", "application/json");
request->SetPostFileds("{\"uid\":1001}");
request->Do("http://127.0.0.1:8080/check", XEngine::Api::POST,
    [](bool success, const std::map<std::string, std::string>& headers, XEngine::Api::IBuffer& buffer)
    {
        if (!success)
        {
            return;
        }

        const char* body = reinterpret_cast<const char*>(buffer.GetData());
        const int size = buffer.GetDataSize();
        // 在主线程里解析返回内容
    });
```

### 11.9 什么时候优先选 Http

适合:

- 与外部 Web 服务打交道
- 需要 Header、URL、POST Body 这些 HTTP 概念
- 不是长连接协议

不适合:

- 自己实现高频游戏长连接
- 高实时双向 TCP 会话

这些更适合 `NetApi`。

---

## 12. `iMysqlApi`: MySQL 会话与查询

### 12.1 它解决什么问题

MySQL 是典型的阻塞式外部资源。  
如果你在主线程直接慢吞吞连库、查库，服务器就容易被卡住。

所以 `XEngine` 提供的是一种更合理的模式:

- 连接在后台建立
- 查询在后台执行
- 查询结果回到主线程

### 12.2 基本使用流程

通常分两段:

1. 先 `ConnectMysql(...)`
2. 成功后通过 `iMysqlSession` 去 `Query(...)`

### 12.3 连接是异步的

`ConnectMysql` 不是当场把连接马上建立好再返回。  
它会在后台尝试连接，连接成功后再在主线程回调你。

这很重要，因为它说明:

- 你不能假设 `ConnectMysql` 调完就立刻有可用 session
- 你应该在连接成功回调里保存 session 指针

### 12.4 `iMysqlSession` 可以怎么理解

你可以把 `MysqlSession` 理解成:

> “一条数据库会话通道，背后自己带着一条查询线程。”

这意味着:

- 同一个 session 上的查询是顺序处理的
- 逻辑层不用自己再去处理复杂的 SQL 并发顺序

### 12.5 `Query` 的典型使用方式

```cpp
session->Query("select * from t_player",
    [engine](const int code, const std::vector<std::vector<std::string>>* const result)
    {
        if (code == XEngine::Api::MYSQL_CODE_SUCCESS && result != nullptr)
        {
            // 主线程使用 result
        }

        engine->GetMysqlApi()->FreeQueryResult(result);
    });
```

### 12.6 为什么要 `FreeQueryResult`

这是 `MysqlApi` 最容易漏掉的一点。

查询结果不是自动帮你销毁的。  
逻辑层用完以后，要显式调用:

```cpp
engine->GetMysqlApi()->FreeQueryResult(result);
```

如果你忘了这一步，就容易造成内存泄漏。

### 12.7 查询结果的结构怎么理解

返回值通常是:

```cpp
std::vector<std::vector<std::string>>
```

你可以把它想成:

- 外层 `vector`: 多行
- 内层 `vector`: 每行的多列

也就是一张二维表。

举例:

```text
id   name   level
1    tom    10
2    bob    12
```

对应到内存里就是:

```text
[
  ["1", "tom", "10"],
  ["2", "bob", "12"]
]
```

### 12.8 `EscapeSql` 是干什么的

如果你要把字符串拼进 SQL，应该考虑转义问题。  
`MysqlApi` 提供了 `EscapeSql` 来做基础的 SQL 转义。

它的意义是:

- 减少字符串拼接时的风险
- 避免某些特殊字符把 SQL 结构搞坏

虽然更理想的方式是参数化查询，但在当前这套接口风格下，`EscapeSql` 仍然很实用。

### 12.9 什么时候用 Mysql

适合:

- 持久化数据
- 查询玩家档案
- 查询账号信息
- 更新角色状态
- 批量读写业务表

如果你的需求是:

- 高频小 key 读写
- 简单缓存
- 临时状态共享

那么常常更适合 Redis。

---

## 13. `iRedisApi`: Redis 异步命令系统

### 13.1 Redis 在这里扮演什么角色

Redis 的定位通常更接近:

- 高速缓存
- 轻量状态存储
- 分布式协调辅助
- 高频小数据读写

它不像 MySQL 那样偏“正式存档表”，而更像“快而轻”的数据层。

### 13.2 使用流程

一般也是先连接，再发命令:

1. `ConnectRedis(...)`
2. 回调里拿到 `iRedisSession`
3. 调 `AsyncGet/AsyncSet/...`
4. 在主线程回调里处理结果

### 13.3 它常见支持哪些操作

当前常见接口有:

- `AsyncGet`
- `AsyncSet`
- `AsyncSetWhenNotExists`
- `AsyncCAS`
- `Close`

你可以这样理解:

| 方法 | 含义 |
| --- | --- |
| `AsyncGet` | 异步取值 |
| `AsyncSet` | 异步设置 |
| `AsyncSetWhenNotExists` | 只有 key 不存在时才写入 |
| `AsyncCAS` | 类似 compare-and-set |
| `Close` | 关闭会话 |

### 13.4 它的底层模式是什么

从逻辑开发者视角看，Redis 的行为模式和 `TaskApi` 很像:

- 你在主线程发命令
- 底层异步执行
- 结果回主线程

所以如果你已经理解了 `TaskApi`，那理解 `RedisApi` 会很快。

### 13.5 同一个 Redis 会话为什么更像“串行 lane”

同一个 `RedisSession` 会带一个稳定的路由索引。  
这会让这个 session 上的任务倾向于走同一条后台处理 lane。

你可以把它理解成:

> “同一个 Redis 会话上的命令，顺序感会更稳定一些。”

这对很多业务是有帮助的，比如:

- 同一个账号的状态更新
- 同一个 key 的前后依赖操作

### 13.6 `AsyncGet` 返回的字符串要注意什么

这是 Redis 使用里非常重要的一点:

`AsyncGet` 回调里给你的 `const char*`，只适合在回调当场使用。  
如果你要在回调结束后继续使用，应该自己复制出来。

也就是说:

- 临时解析: 可以直接用
- 长期保存: 自己拷贝

### 13.7 一个简单示例

```cpp
session->AsyncGet("player:1001",
    [](const char* value)
    {
        if (value == nullptr)
        {
            return;
        }

        std::string copy = value;
        // 后续使用 copy
    });
```

### 13.8 什么时候优先考虑 Redis

适合:

- 高频轻量状态
- 登录态缓存
- 临时令牌
- 分布式锁的辅助方案
- 排行榜或快速键值查询

不太适合:

- 大量复杂关联查询
- 强关系型表结构
- 正式业务存档的唯一来源

这些通常更适合 MySQL。

---

## 14. `Http`、`Mysql`、`Redis` 三者怎么选

很多同学刚接手项目时，会分不清这三个异步系统的边界。  
其实它们的用途差别很大。

### 14.1 先看一句最直白的判断

- 调网页接口，用 `Http`
- 查关系型数据库，用 `Mysql`
- 读写高速缓存，用 `Redis`

### 14.2 对比表

| 系统 | 最适合干什么 | 最常见返回结果 |
| --- | --- | --- |
| `Http` | 调外部 Web 接口 | Header + Body |
| `Mysql` | 查正式业务数据 | 表格型结果集 |
| `Redis` | 查轻量 key-value | 单值或简单状态 |

### 14.3 一个生活化比喻

可以把它们想成:

- `Http`: 打电话问外部单位事情
- `Mysql`: 查档案室里的正式纸质档案
- `Redis`: 看桌边便利贴上的快速记录

这个比喻虽然不严格，但对初学者很好记。

---

## 15. `iNetApi` 和 `iHttpApi` 的区别

这两个容易混，但其实不是一类东西。

### 15.1 `NetApi`

更像:

- 原始 TCP 长连接
- 需要自己定义包协议
- 高实时双向通信
- 更适合游戏内部会话

### 15.2 `HttpApi`

更像:

- 标准 HTTP 请求
- 有 URL、Header、Body 这些概念
- 更适合 Web 风格接口

### 15.3 一句判断法

如果你脑子里想的是:

- “我要收包、拆包、长连接通信”  
  那大概率是 `NetApi`

- “我要发 GET/POST 请求，拿网页接口返回结果”  
  那大概率是 `HttpApi`

---

## 16. `iWebSocketsApi` 和 `iZipApi` 的当前状态

### 16.1 `iWebSocketsApi`

从接口设计上看，`WebSocketsApi` 也有自己的一套抽象:

- `iWebSocketSession`
- `iWebSocketServer`
- `iWebSocketsApi`

语义上它和 TCP 很像，也是:

- 建连接
- 收消息
- 断开连接

但是在当前这套引擎接线里，它没有像 `Net/Http/Mysql/Task` 那样真正作为可用子系统暴露给逻辑层。

所以站在当前项目的逻辑开发者角度，可以这样理解:

> 这个接口面存在，但当前版本里不应该把它当作主力可用能力来依赖。

### 16.2 `iZipApi`

`ZipApi` 的接口目标很明确，就是做异步解压。  
但在当前版本里，它同样没有作为稳定可用的 `engine` 子系统接出来。

因此对于当前业务开发来说:

- 知道它存在就好
- 不要把它当成当前主流程的基础能力

---

## 17. 推荐的逻辑开发姿势

这一节非常重要。  
即使你把所有 API 都背下来，如果使用姿势不对，代码还是会越写越乱。

### 17.1 姿势一: 业务状态放在主线程维护

这是最重要的总原则。

比如:

- 玩家上下线
- 背包变化
- 角色属性变化
- 场景切换
- 技能释放状态

这些最好都在主线程里处理。

### 17.2 姿势二: 耗时操作只负责“生产结果”

后台任务最好只做下面这些事:

- 查数据
- 算数据
- 准备数据

不要在后台线程里直接改复杂业务状态。

正确流程应该是:

1. 后台线程准备结果
2. 主线程完成回调收结果
3. 主线程修改业务状态

### 17.3 姿势三: 定时逻辑优先用 Timer，不要乱写轮询

如果你的需求是:

- N 秒后执行
- 每隔 N 秒执行

那就优先用 `Timer`。  
不要把这类逻辑硬塞到 `Update` 里手搓时间累加器，除非你非常明确这么做更合理。

### 17.4 姿势四: 网络层负责传输，业务层负责语义

`iTcpSession` 做的事情最好是:

- 收包
- 拆包
- 分发

而不是把所有业务都写在 `OnReceive` 里。

更推荐的方式是:

1. `OnReceive` 完成拆包
2. 识别消息类型
3. 转发给对应管理模块

这样层次会更清楚。

### 17.5 姿势五: 资源生命周期要明确

下面这些对象，使用时要特别留意生命周期:

- `Http` 的响应 `IBuffer`
- `Mysql` 的结果集
- `Redis` 回调里的字符串
- `TaskContext` 里自己塞进去的堆对象

记忆方法:

- 如果不是你长期持有的稳定对象，就默认“只当场有效”
- 如果想以后继续用，就自己复制

---

## 18. 一个完整的思维例子: 登录流程怎么拆

为了让初学者更容易理解，我们举一个完整场景: 玩家登录。

### 18.1 第一步: 收到网络包

客户端发来登录包。  
这个包会先到 `Gate` 或某个网络 session 的 `OnReceive`。

这一步应该做的事情是:

- 拆包
- 识别消息类型
- 提取账号参数
- 转给登录逻辑

### 18.2 第二步: 如果要查数据库

如果需要查账号表，就不要在主线程里直接阻塞等待。  
这时候应该用 `MysqlApi`。

流程变成:

1. 主线程收到登录请求
2. 发起异步 SQL 查询
3. 查询完成后回主线程
4. 主线程根据结果决定登录成功或失败

### 18.3 第三步: 如果要查缓存

如果登录态、限流标记、临时 token 在 Redis 里，那就用 `RedisApi`。

流程还是一样:

1. 主线程发请求
2. 后台查 Redis
3. 结果回主线程
4. 主线程做业务决策

### 18.4 第四步: 如果要调第三方平台

如果登录还需要请求外部平台验证，就用 `HttpApi`。

流程:

1. 主线程构造 HTTP 请求
2. 后台发送
3. 主线程收到 HTTP 结果
4. 再决定登录是否通过

### 18.5 第五步: 如果有超时逻辑

比如登录流程 10 秒超时，就可以挂一个 `Timer`。

如果在超时前已经完成，就停掉这个 timer。  
如果到时间还没完成，就进入超时处理。

你会发现:

一个完整的业务流程里，往往不是只用一个 API，  
而是:

- `Net` 负责入口
- `Mysql/Redis/Http` 负责取外部数据
- `Timer` 负责超时控制
- 主线程负责最终落业务状态

这就是 `XEngine` 的典型使用方式。

---

## 19. 大学生最容易混淆的 10 个问题

### 19.1 “我是不是要学会多线程才能写 XEngine?”

不需要先把自己变成并发专家。  
更重要的是先建立“主线程逻辑”这套心智模型。

你需要知道:

- 哪些东西在后台做
- 哪些结果会回主线程
- 真正的业务状态在哪里改

把这三件事分清，已经很够用了。

### 19.2 “为什么 `OnReceive` 返回的是消费字节数?”

因为 TCP 是字节流，不是天然按消息分包的。  
引擎把拆包权交给逻辑层，所以你要告诉它:

- 这次处理了多少
- 哪些还要继续留着等后续数据

### 19.3 “Timer 为什么要有 `context`?”

因为仅靠 `id` 经常不够。  
同一个对象可能会同时挂多份同类 timer。

`context` 可以帮你区分:

- 这是哪个玩家的 timer
- 这是哪个怪的 timer
- 这是哪个场景实例的 timer

### 19.4 “Task 回调里能不能直接改玩家对象?”

不推荐。  
更稳妥的方式是:

- 后台任务只产出结果
- 主线程完成回调里改玩家对象

### 19.5 “HTTP 返回的数据能不能存起来以后再解析?”

不要默认可以长期持有。  
最稳妥的方式是:

- 回调里当场解析
- 或者自己拷贝一份再保存

### 19.6 “MySQL 查询结果为什么还要我自己释放?”

因为这套接口把结果的使用权交给逻辑层了。  
你拿到结果，就也承担了用完释放的责任。

### 19.7 “Redis 回调里的 `const char*` 为什么不建议长期保存?”

因为它通常依赖底层 reply 对象的生命周期。  
回调结束后，这块内存未必还可靠。

### 19.8 “`Update` 和 `Timer` 怎么选?”

优先判断你的需求是不是“按时间触发”。

如果是:

- 延时
- 周期
- 超时

优先选 `Timer`。

如果是:

- 每帧都要做
- 和帧逻辑强耦合

再考虑写进模块 `Update`。

### 19.9 “模块之间怎么通信最好?”

优先通过模块接口通信。  
也就是:

1. 通过 `GetModule` 拿到对方接口
2. 调它暴露出来的方法

这样比到处乱共享全局变量更清晰。

### 19.10 “我要先学哪个 API?”

推荐顺序是:

1. `iModule`
2. `iEngine`
3. `iTimerApi`
4. `iNetApi`
5. `iTaskApi`
6. `iMysqlApi`
7. `iRedisApi`
8. `iHttpApi`

因为这个顺序最符合从“主线程逻辑”到“异步外设”的学习路径。

---

## 20. 一个适合新人的学习路线

如果你是第一次正式上手这个工程，建议按下面顺序学习:

### 第一阶段: 建立骨架认识

目标:

- 知道模块是怎么加载的
- 知道主循环怎么跑
- 知道 `iEngine` 是总入口

这时你应该重点理解:

- `iModule`
- `iEngine`

### 第二阶段: 学会主线程调度

目标:

- 能写简单定时逻辑
- 能区分 `Update` 和 `Timer`

这时你应该重点理解:

- `iTimerApi`

### 第三阶段: 学会网络入口

目标:

- 能读懂一个 session
- 能理解 `OnReceive` 的拆包语义

这时重点理解:

- `iNetApi`
- `iTcpSession`
- `iTcpServer`

### 第四阶段: 学会异步外部系统

目标:

- 能异步查 MySQL
- 能异步查 Redis
- 能异步发 HTTP

这时重点理解:

- `iTaskApi`
- `iMysqlApi`
- `iRedisApi`
- `iHttpApi`

### 第五阶段: 回到业务模块

当你前面这些都懂了，再去看:

- `Gate`
- `Game`
- `PlayerManager`
- `SceneManager`
- `SkillManager`

你会发现很多逻辑就会变得非常顺了。

---

## 21. 快速参考表

### 21.1 需求到 API 的映射

| 需求 | 优先考虑的 API |
| --- | --- |
| 过 5 秒执行一次 | `iTimerApi` |
| 每 100ms 做一次 AI 检查 | `iTimerApi` |
| 主动连另一个 TCP 服务 | `iNetApi` |
| 开一个 TCP 监听端口 | `iNetApi` |
| 调第三方登录接口 | `iHttpApi` |
| 查玩家数据库档案 | `iMysqlApi` |
| 读写登录态缓存 | `iRedisApi` |
| 做重计算但不想卡主线程 | `iTaskApi` |

### 21.2 主线程回调速查

| API | 回调是否回主线程 |
| --- | --- |
| `Timer` | 是 |
| `Net` | 是 |
| `Task 完成回调` | 是 |
| `Http 回调` | 是 |
| `Mysql 回调` | 是 |
| `Redis 回调` | 是 |

### 21.3 使用时最该记住的注意点

| API | 最容易漏掉的点 |
| --- | --- |
| `Timer` | 唯一键是 `(对象, id, context)` |
| `Net` | `OnReceive` 返回消费字节数，不是 true/false |
| `Task` | 后台做重活，主线程收结果 |
| `Http` | `IBuffer` 想长期使用就自己拷贝 |
| `Mysql` | 查询结果用完要 `FreeQueryResult` |
| `Redis` | 回调里的字符串想长期使用就自己拷贝 |

---

## 22. 一段真正应该记住的总结

如果你已经看到了这里，可以把整套 `XEngine` 收缩成下面几句话:

### 22.1 第一层理解

`XEngine` 是一个以主线程逻辑为中心的服务器运行时。

### 22.2 第二层理解

逻辑开发者主要做三件事:

1. 写模块
2. 组织主线程状态
3. 通过各种 API 和外部系统交互

### 22.3 第三层理解

各种 API 的职责边界大致是:

- `iEngine`: 总入口
- `iModule`: 模块生命周期
- `iTimerApi`: 时间调度
- `iNetApi`: TCP 通信
- `iTaskApi`: 后台重任务
- `iHttpApi`: HTTP 请求
- `iMysqlApi`: MySQL
- `iRedisApi`: Redis

### 22.4 第四层理解

最重要的使用原则是:

> 不要把业务写成“到处并发乱跑”的样子，而要写成“主线程组织状态，异步系统提供结果”的样子。

一旦你真的把这句话吃透，后面再看 `Gate`、`Game`、`PlayerManager`、`SkillManager`、`SceneManager` 这些模块，理解速度会快很多。

---

## 23. 给第一次上手的同学一个最后建议

如果你是新人，不要一开始就试图把整个 `XEngine` 的实现细节全部背下来。  
更好的方法是按下面这个顺序:

1. 先明白主线程模型
2. 再学模块生命周期
3. 再学 Timer 和 Net
4. 再学 Task、Mysql、Redis、Http
5. 最后回到具体业务模块

因为真正让你能写业务的，不是“把所有底层代码背下来”，而是:

> 知道每个 API 在什么场景下该用，结果会在什么时候回到主线程，以及业务状态应该在哪里修改。

做到这一点，你就已经真正入门 `XEngine` 了。
