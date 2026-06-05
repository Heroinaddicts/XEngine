#!/bin/zsh

CurrentPath=$(cd "$(dirname "$0")" && pwd)
BinPath="$CurrentPath/../../Bin/MacOS"

ForwarderPort=${ForwarderPort:-7010}
ReciverPort=${ReciverPort:-7020}
SenderInterval=${SenderInterval:-2}
SenderCount=${SenderCount:-1}
SenderPrintQps=${SenderPrintQps:-0}

cd "$BinPath" || exit 1
mkdir -p ./Log

sender_pids=()
forwarder_pid=
reciver_pid=

cleanup() {
    for pid in $sender_pids $forwarder_pid $reciver_pid; do
        if [ -n "$pid" ]; then
            kill "$pid" 2>/dev/null || true
        fi
    done
}

trap cleanup INT TERM EXIT

./XEngine --name=ReciverPerf --modules='Reciver' --ReciverPort="$ReciverPort" --log=./Log &
reciver_pid=$!

sleep 1

./XEngine --name=ForwarderPerf --modules='Forwarder' --ForwarderPort="$ForwarderPort" --ReciverHost=127.0.0.1 --ReciverPort="$ReciverPort" --log=./Log &
forwarder_pid=$!

sleep 1

for index in $(seq 1 "$SenderCount"); do
    ./XEngine --name="SenderPerf${index}" --modules='Sender' --ForwarderHost=127.0.0.1 --ForwarderPort="$ForwarderPort" --SenderInterval="$SenderInterval" --SenderPrintQps="$SenderPrintQps" --log=./Log &
    sender_pids+=($!)
done

echo "ReciverPerf pid: $reciver_pid port: $ReciverPort"
echo "ForwarderPerf pid: $forwarder_pid port: $ForwarderPort"
echo "SenderPerf count: $SenderCount interval: ${SenderInterval}ms"
echo "SenderPrintQps: $SenderPrintQps"
if [ "$SenderCount" -le 20 ]; then
    echo "SenderPerf pids: $sender_pids"
else
    echo "SenderPerf first pid: ${sender_pids[1]} last pid: ${sender_pids[-1]}"
fi
echo "Log path: $BinPath/Log"

wait
