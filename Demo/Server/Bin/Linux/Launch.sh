CurrentPath=$(dirname "$(realpath "$0")")
cd $CurrentPath
pkill XEngine
./XEngine --name=Gate --modules='Gate;Db' --DbGroupCount=10 --MysqlHost=172.16.0.25 --MysqlPort=3306 --MysqlDb=game --MysqlUsername=xgame --MysqlPassword=abc123123 --RedisHost=127.0.0.1 --RedisPort=6379 --PortForClient=18888 --GameServerPort=5000 --log=./Log &
./XEngine --name=Game --modules='Fsm;Game;BossManager;ServerManager;BuffManager;TaskManager;RefreshRules;MatchManager;TeamManager;MonsterManager;PlayerManager;SceneManager;BuffManager;GameEntitySync;NpcManager;SkillManager;TilemapManager;Config;CharacterManager;Backpack;Combat;GameWorld;Movement;SceneManager;CacheClient;SQLCache' --ServerID=201 --PortForGate=5000 --CachePort=6000 --RedisHost=127.0.0.1 --RedisPort=6379 --TilemapPath=$CurrentPath/Tilemap/ --log=./Log &
./XEngine --name=Cache --modules='Cache;Db;SQLCache' --DbGroupCount=10 --MysqlHost=172.16.0.25 --MysqlPort=3306 --MysqlDb=game --MysqlUsername=xgame --MysqlPassword=abc123123 --RedisHost=127.0.0.1 --RedisPort=6379 --PortForGame=6000 --log=./Log &
