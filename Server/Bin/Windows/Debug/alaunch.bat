cd %~dp0
echo off;
echo "kill old proc..."
taskkill /im XEngine.exe
pause
echo "launche string...."

start "Balance" XEngine --modules=Connecter;Balance --name=Balance --node_id=10001 --node_ip=127.0.0.1 --node_port=10001
start "Lobby" XEngine --modules=Connecter;Lobby --name=Lobby --node_id=20001 --node_ip=127.0.0.1 --node_port=20001
##start "Gate" XEngine --modules=Connecter --name=Gate --node_id=10001 --remote_nodes=127.0.0.1:4499