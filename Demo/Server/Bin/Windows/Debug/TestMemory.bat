@echo off
chcp 65001 >nul
cd %~dp0
echo "本地调试"

taskkill /F /IM XEngine.exe

start "TestMemory" XEngine.exe --modules=TestMemory --log=./Log
start "TestMemory" XEngine.exe --modules=TestMemory --log=./Log
start "TestMemory" XEngine.exe --modules=TestMemory --log=./Log
start "TestMemory" XEngine.exe --modules=TestMemory --log=./Log
start "TestMemory" XEngine.exe --modules=TestMemory --log=./Log
start "TestMemory" XEngine.exe --modules=TestMemory --log=./Log
start "TestMemory" XEngine.exe --modules=TestMemory --log=./Log
start "TestMemory" XEngine.exe --modules=TestMemory --log=./Log
start "TestMemory" XEngine.exe --modules=TestMemory --log=./Log

