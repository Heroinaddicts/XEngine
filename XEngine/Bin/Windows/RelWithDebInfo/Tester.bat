cd %~dp0
echo off
for /l %%i in (1,1,25) do start "Test" XEngine --components=ObjLoader;TestPhysics --pvd_host=127.0.0.1 --pvd_port=5425

