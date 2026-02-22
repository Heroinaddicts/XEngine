@echo off
chcp 65001 >nul
cd %~dp0
echo "本地调试"
start "GameRobot" XEngine.exe --name=GameRobot --modules=GameRobot;RobotSkill;RobotSceneManager;TilemapManager;Config;RobotNpcManager;RobotAi;RobotGameObjectSync;RobotMovement;GameWorld --TilemapPath=D:\Projects\Daomu\Server\Bin\Windows\Debug\Tilemap\ --Host=127.0.0.1 --Port=18888 --RobotCount=2 --Interval=100 --log=./Log
##start "GameRobot" XEngine.exe --name=GameRobot --modules=GameRobot;RobotSkill;RobotSceneManager;TilemapManager;Config;RobotNpcManager;RobotAi;RobotGameObjectSync;RobotMovement;GameWorld --TilemapPath=D:\Projects\Daomu\Server\Bin\Windows\Debug\Tilemap\ --Host=game.game-ai.com.cn --Port=18888 --RobotCount=100 --Interval=1 --log=./Log
##start "GameRobot" XEngine.exe --name=GameRobot --modules=GameRobot;RobotSkill;RobotSceneManager;TilemapManager;Config;RobotNpcManager;RobotAi;RobotGameObjectSync;RobotMovement;GameWorld --TilemapPath=D:\Projects\Daomu\Server\Bin\Windows\Debug\Tilemap\ --Host=game.game-ai.com.cn --Port=18888 --RobotCount=100 --Interval=1 --log=./Log
##start "GameRobot" XEngine.exe --name=GameRobot --modules=GameRobot;RobotSkill;RobotSceneManager;TilemapManager;Config;RobotNpcManager;RobotAi;RobotGameObjectSync;RobotMovement;GameWorld --TilemapPath=D:\Projects\Daomu\Server\Bin\Windows\Debug\Tilemap\ --Host=game.game-ai.com.cn --Port=18888 --RobotCount=100 --Interval=1 --log=./Log
##start "GameRobot" XEngine.exe --name=GameRobot --modules=GameRobot;RobotSkill;RobotSceneManager;TilemapManager;Config;RobotNpcManager;RobotAi;RobotGameObjectSync;RobotMovement;GameWorld --TilemapPath=D:\Projects\Daomu\Server\Bin\Windows\Debug\Tilemap\ --Host=game.game-ai.com.cn --Port=18888 --RobotCount=100 --Interval=1 --log=./Log

