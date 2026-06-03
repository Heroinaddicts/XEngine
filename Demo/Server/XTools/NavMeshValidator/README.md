# NavMeshValidator

`NavMeshValidator` 用来验证 `D:\Projects\MMO\Server\XEngine\Engine\Component\Navigation\NavMesh.cpp` 这套运行时查询结果。

## 输入

1. `navmesh` 文件  
   必须是 `RecastDemo/Sample.cpp` 导出的普通 `MSET` 二进制。

2. `cases` 文件  
   JSON 格式，样例见：

- `D:\Projects\MMO\Server\XTools\NavMeshValidator\Cases.sample.json`

## 支持的 case

### nearest

- `name`
- `point`
- `searchRadius`
- `agentRadius`
- `agentHeight`
- `expectSuccess`
- `expectPoint` 可选
- `positionTolerance` 可选

### path

- `name`
- `begin`
- `end`
- `agentRadius`
- `agentHeight`
- `expectSuccess`
- `expectPointCountMin` 可选
- `expectPointCountMax` 可选
- `expectPathLengthMin` 可选
- `expectPathLengthMax` 可选
- `expectEndPoint` 可选
- `positionTolerance` 可选

### raycast

- `name`
- `begin`
- `end`
- `agentRadius`
- `agentHeight`
- `expectHit`
- `expectHitPoint` 可选
- `positionTolerance` 可选

## 输出

控制台输出总计结果。

如果传了 `--report=...`，还会生成 JSON 报告，包含：

- 每条 case 的输入
- 实际结果
- 失败原因
- 汇总统计

## 用法

```bat
NavMeshValidator.exe --navmesh=D:\Path\To\level.bin --cases=D:\Path\To\cases.json --report=D:\Path\To\report.json
```

## 建议流程

1. 先从你的地图里人工挑 3 组 `nearest`
2. 再挑 5 组 `path`
3. 再挑 3 组 `raycast`
4. 先保证这些 case 稳定通过
5. 后面每改一次导航实现，就重新跑一遍
