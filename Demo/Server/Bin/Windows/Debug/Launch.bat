@echo off
chcp 65001 >nul
cd %~dp0
echo "本地调试"

taskkill /F /IM XEngine.exe

start "Gate" XEngine.exe --name=Gate --modules=Gate;Db --DbGroupCount=30 --MysqlHost=127.0.0.1 --MysqlPort=3306 --MysqlDb=game --MysqlUsername=root --MysqlPassword=abc123123 --RedisHost=127.0.0.1 --RedisPort=6379 --PortForClient=18888 --GameServerPort=5000 --log=./Log
start "Game" XEngine.exe --name=Game --modules=Fsm;Game;BossManager;ServerManager;TeamManager;BuffManager;TaskManager;RefreshRules;MatchManager;MonsterManager;PlayerManager;LevelManager;SceneManager;BuffManager;GameEntitySync;NpcManager;SkillManager;TilemapManager;Config;CharacterManager;Backpack;Combat;GameWorld;Movement;SceneManager;CacheClient;SQLCache --ServerID=201 --PortForGate=5000 --CachePort=6000 --RedisHost=127.0.0.1 --RedisPort=6379 --TilemapPath=./Tilemap/ --log=./Log
start "Cache" XEngine.exe --name=Cache --modules=Cache;Db;SQLCache --DbGroupCount=30 --MysqlHost=127.0.0.1 --MysqlPort=3306 --MysqlDb=game --MysqlUsername=root --MysqlPassword=abc123123 --RedisHost=127.0.0.1 --RedisPort=6379 --PortForGame=6000 --log=./Log

