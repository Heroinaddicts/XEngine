# XTests Debug Performance Report

Date: 2026-06-05
Platform: macOS arm64
Build: Debug
Duration: 30 seconds

## Test Setup

- Modules: `Sender`, `Forwarder`, `Reciver`
- Process layout: three `XEngine` processes on local loopback
- Ports: `ForwarderPort=18110`, `ReciverPort=18120`
- Sender interval: `10ms`
- Packet format: `UInt16 bodyLength` + random body size `0-4096` bytes
- Build command:

```bash
cmake --build /Users/max/GitHub/XEngine/Demo/Server/Build --config Debug --target XEngine Sender Forwarder Reciver -j 8
```

## Result Summary

The sender is intentionally rate-limited to one packet every 10ms, so this test validates stability at about 100 packets/sec. It does not represent the maximum throughput ceiling of the engine.

| Role | Avg QPS | Min QPS | Max QPS | Samples |
|---|---:|---:|---:|---:|
| Sender | 99.97 | 99 | 101 | 30 |
| Forwarder | 99.93 | 98 | 101 | 30 |
| Reciver | 100.00 | 99 | 101 | 30 |

| Role | Avg CPU % | Max CPU % | Avg RSS MB | Max RSS MB | Samples |
|---|---:|---:|---:|---:|---:|
| Sender | 3.55 | 4.00 | 11.39 | 14.47 | 30 |
| Forwarder | 3.59 | 4.00 | 14.52 | 20.38 | 30 |
| Reciver | 3.12 | 3.40 | 11.56 | 14.50 | 30 |

## Observations

- Forwarder connected to Reciver successfully before Sender traffic started.
- Sender, Forwarder, and Reciver stayed aligned around 100 qps.
- No test process remained after shutdown.
- Build succeeded. Existing project warnings were observed, including the existing `libCache.dylib` reexport linker warning.

## 100 Sender Stress Test

This test starts 102 `XEngine` processes: 100 Sender processes, 1 Forwarder process, and 1 Reciver process.

- Ports: `ForwarderPort=18610`, `ReciverPort=18620`
- Sender interval: `10ms`
- Expected offered load: about `10000` packets/sec
- Duration: 60 seconds

| Role | Avg QPS | Min QPS | Max QPS | Samples |
|---|---:|---:|---:|---:|
| Sender aggregate | 9998.36 | 9900 | 10100 | 61 |
| Forwarder | 9997.30 | 9818 | 10018 | 61 |
| Reciver stable | 10000.05 | 9990 | 10011 | 61 |
| Reciver including startup | 9925.85 | 5400 | 10011 | 62 |

| Role | Avg Total CPU % | Max Total CPU % | Avg Total RSS MB | Max Total RSS MB | Samples |
|---|---:|---:|---:|---:|---:|
| 100 Senders | 440.63 | 482.00 | 1393.71 | 1672.52 | 60 |
| Forwarder | 18.79 | 20.30 | 543.92 | 820.03 | 60 |
| Reciver | 7.29 | 8.10 | 16.58 | 16.73 | 60 |

Forwarder can sustain roughly 10000 forwarded packets/sec in Debug under this workload. Forwarder RSS is high because each accepted Sender connection currently uses an `8MB` TCP buffer.
