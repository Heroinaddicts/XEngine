cd %~dp0
echo off;
echo "kill old proc..."
taskkill /im XEngine.exe
pause
echo "launche string...."

start "Tester" XEngine --components=ObjLoader;TestPhysics --pvd_host=127.0.0.1 --pvd_port=5425