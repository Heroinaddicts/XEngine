# XTests Release Performance Report

Date: 2026-06-05
Platform: macOS arm64
Build: Release
Duration: 30 seconds

## Test Setup

- Modules: `Sender`, `Forwarder`, `Reciver`
- Process layout: three `XEngine` processes on local loopback
- Ports: `ForwarderPort=18210`, `ReciverPort=18220`
- Sender interval: `10ms`
- Packet format: `UInt16 bodyLength` + random body size `0-4096` bytes
- Build command:

```bash
cmake --build /Users/max/GitHub/XEngine/Demo/Server/Build --config Release --target XEngine Sender Forwarder Reciver -j 8
```

## Result Summary

The sender is intentionally rate-limited to one packet every 10ms, so this test validates stability at about 100 packets/sec. It does not represent the maximum throughput ceiling of the engine.

| Role | Avg QPS | Min QPS | Max QPS | Samples |
|---|---:|---:|---:|---:|
| Sender | 99.97 | 99 | 101 | 30 |
| Forwarder | 99.97 | 99 | 100 | 30 |
| Reciver | 99.97 | 99 | 101 | 30 |

| Role | Avg CPU % | Max CPU % | Avg RSS MB | Max RSS MB | Samples |
|---|---:|---:|---:|---:|---:|
| Sender | 2.94 | 3.60 | 10.84 | 13.80 | 30 |
| Forwarder | 3.10 | 3.50 | 13.73 | 19.62 | 30 |
| Reciver | 2.72 | 3.10 | 10.84 | 13.80 | 30 |

## Observations

- Forwarder connected to Reciver successfully before Sender traffic started.
- Sender, Forwarder, and Reciver stayed aligned around 100 qps.
- Release used less CPU and RSS than Debug under the same capped workload.
- No test process remained after shutdown.
- Build succeeded. Existing project warnings were observed, including the existing `libCache.dylib` reexport linker warning.

## 100 Sender Stress Test

This test starts 102 `XEngine` processes: 100 Sender processes, 1 Forwarder process, and 1 Reciver process.

- Ports: `ForwarderPort=18510`, `ReciverPort=18520`
- Sender interval: `10ms`
- Expected offered load: about `10000` packets/sec
- Duration: 60 seconds

| Role | Avg QPS | Min QPS | Max QPS | Samples |
|---|---:|---:|---:|---:|
| Sender aggregate | 9998.36 | 9900 | 10100 | 61 |
| Forwarder | 9990.26 | 9390 | 10010 | 62 |
| Reciver | 9988.47 | 9280 | 10016 | 62 |

| Role | Avg Total CPU % | Max Total CPU % | Avg Total RSS MB | Max Total RSS MB | Samples |
|---|---:|---:|---:|---:|---:|
| 100 Senders | 426.90 | 468.40 | 1336.55 | 1607.72 | 60 |
| Forwarder | 18.37 | 20.80 | 550.95 | 819.42 | 60 |
| Reciver | 6.99 | 8.00 | 15.96 | 16.11 | 60 |

Forwarder can sustain roughly 10000 forwarded packets/sec in Release under this workload. Forwarder RSS is high because each accepted Sender connection currently uses an `8MB` TCP buffer.
