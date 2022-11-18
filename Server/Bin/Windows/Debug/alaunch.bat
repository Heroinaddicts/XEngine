cd %~dp0
echo off;
echo "kill old proc..."
taskkill /im XEngine.exe
pause
echo "launche string...."

start "Gate" XEngine --modules=Connecter --name=Gate --node_id=10001 --remote_nodes=127.0.0.1:4499
start "Lobby" XEngine --modules=Connecter --name=Lobby --node_id=20001 --node_ip=127.0.0.1 --node_port=4499