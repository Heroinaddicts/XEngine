-- --------------------------------------------------------
-- 主机:                           127.0.0.1
-- 服务器版本:                        11.7.2-MariaDB - mariadb.org binary distribution
-- 服务器操作系统:                      Win64
-- HeidiSQL 版本:                  12.10.0.7000
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


-- 导出 game 的数据库结构
DROP DATABASE IF EXISTS `game`;
CREATE DATABASE IF NOT EXISTS `game` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci */;
USE `game`;

-- 导出  存储过程 game.GetAccount 结构
DROP PROCEDURE IF EXISTS `GetAccount`;
DELIMITER //
CREATE PROCEDURE `GetAccount`(
	IN `p_Platform` INT,
	IN `p_UnionID` VARCHAR(255)
)
BEGIN
    DECLARE v_Guid BIGINT;
    DECLARE v_RegistTick BIGINT DEFAULT UNIX_TIMESTAMP();
    DECLARE v_LastLoginTick BIGINT DEFAULT UNIX_TIMESTAMP();

    DECLARE exit handler for SQLEXCEPTION
    BEGIN
        SELECT -1 AS Platform, -1 AS Guid, -1 AS LastLoginTick, -1 AS RegistTick;
        ROLLBACK;
    END;

    START TRANSACTION;

    INSERT INTO t_account (Platform, UnionID, LastLoginTick, RegistTick)
    VALUES (p_Platform, p_UnionID, v_LastLoginTick, v_RegistTick)
    ON DUPLICATE KEY UPDATE
        LastLoginTick = v_LastLoginTick,
        Guid = LAST_INSERT_ID(Guid);

    SET v_Guid = LAST_INSERT_ID();

    COMMIT;

    SELECT p_Platform AS Platform, v_Guid AS Guid, v_LastLoginTick AS LastLoginTick, v_RegistTick AS RegistTick;
END//
DELIMITER ;

-- 导出  表 game.t_account 结构
DROP TABLE IF EXISTS `t_account`;
CREATE TABLE IF NOT EXISTS `t_account` (
  `Platform` int(11) NOT NULL,
  `Guid` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `UnionID` varchar(255) NOT NULL,
  `LastLoginTick` bigint(20) NOT NULL,
  `RegistTick` bigint(20) NOT NULL,
  PRIMARY KEY (`Guid`) USING BTREE,
  UNIQUE KEY `Platform_UnionID` (`Platform`,`UnionID`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb3 COLLATE=utf8mb3_general_ci;

-- 数据导出被取消选择。

-- 导出  表 game.t_backpack 结构
DROP TABLE IF EXISTS `t_backpack`;
CREATE TABLE IF NOT EXISTS `t_backpack` (
  `Account` bigint(20) unsigned NOT NULL,
  `CharacterId` bigint(20) unsigned NOT NULL,
  `BackPackData` text NOT NULL,
  PRIMARY KEY (`CharacterId`),
  KEY `Account` (`Account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- 数据导出被取消选择。

-- 导出  表 game.t_character 结构
DROP TABLE IF EXISTS `t_character`;
CREATE TABLE IF NOT EXISTS `t_character` (
  `Account` bigint(20) unsigned NOT NULL,
  `Guid` bigint(20) unsigned NOT NULL,
  `ConfigID` bigint(20) NOT NULL,
  `Level` bigint(20) NOT NULL DEFAULT 0,
  `Exp` bigint(20) NOT NULL DEFAULT 0,
  `LastPublicSceneConfigID` bigint(20) NOT NULL COMMENT '上一次所在的公共场景配置id',
  `SceneGUID` bigint(20) unsigned NOT NULL DEFAULT 0 COMMENT '最近一次副本全局ID',
  `X` float NOT NULL COMMENT '下线时候的坐标',
  `Y` float NOT NULL COMMENT '下线时候的坐标',
  `Angle` float NOT NULL,
  `SlotCloths` bigint(20) unsigned NOT NULL,
  `ClothsConfigID` bigint(20) NOT NULL,
  `SlotPants` bigint(20) unsigned NOT NULL,
  `PantsConfigID` bigint(20) NOT NULL,
  `SlotBelt` bigint(20) unsigned NOT NULL,
  `BeltConfigID` bigint(20) NOT NULL,
  `SlotShoes` bigint(20) unsigned NOT NULL,
  `ShoesConfigID` bigint(20) NOT NULL,
  `SlotJewelry` bigint(20) unsigned NOT NULL,
  `JewelryConfigID` bigint(20) NOT NULL,
  `SlotWeapon` bigint(20) unsigned NOT NULL,
  `WeaponConfigID` bigint(20) NOT NULL,
  PRIMARY KEY (`Guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- 数据导出被取消选择。

-- 导出  表 game.t_equip 结构
DROP TABLE IF EXISTS `t_equip`;
CREATE TABLE IF NOT EXISTS `t_equip` (
  `Guid` bigint(20) unsigned NOT NULL,
  `ConfigID` bigint(20) NOT NULL,
  `Equiped` bit(1) NOT NULL,
  `EquipType` int(11) NOT NULL,
  `Patience` int(11) NOT NULL,
  `StrengthLv` int(11) NOT NULL,
  `DynamicAtk` float NOT NULL DEFAULT 0,
  `DynamicSpeed` float NOT NULL DEFAULT 0,
  `DynamicHp` float NOT NULL DEFAULT 0,
  `DynamicCritical` float NOT NULL DEFAULT 0,
  `DynamicCriticalDamage` float NOT NULL DEFAULT 0,
  `DynamicDefend` float NOT NULL DEFAULT 0,
  `DynamicMagicDefend` float NOT NULL DEFAULT 0,
  `DynamicBlood` float NOT NULL DEFAULT 0,
  `DynamicMagicAttack` float NOT NULL DEFAULT 0,
  `DynamicPhysicalPierce` float NOT NULL DEFAULT 0,
  `DynamicMagicPierce` float NOT NULL DEFAULT 0,
  `DynamicDodge` float NOT NULL DEFAULT 0,
  PRIMARY KEY (`Guid`),
  KEY `ConfigID` (`ConfigID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- 数据导出被取消选择。

-- 导出  表 game.t_player 结构
DROP TABLE IF EXISTS `t_player`;
CREATE TABLE IF NOT EXISTS `t_player` (
  `Account` bigint(20) unsigned NOT NULL COMMENT '1',
  `Name` varchar(50) NOT NULL COMMENT '2',
  `Gold` int(11) unsigned NOT NULL COMMENT '3',
  `Diamond` int(11) unsigned NOT NULL COMMENT '4',
  `DeployCharacterID` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`Account`),
  UNIQUE KEY `Name` (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- 数据导出被取消选择。

-- 导出  表 game.t_skill 结构
DROP TABLE IF EXISTS `t_skill`;
CREATE TABLE IF NOT EXISTS `t_skill` (
  `Guid` bigint(20) unsigned NOT NULL,
  `Account` bigint(20) unsigned NOT NULL,
  `CharacterID` bigint(20) unsigned NOT NULL,
  `ConfigID` bigint(20) NOT NULL,
  `SkillSlot` varchar(50) DEFAULT '',
  `LastCastTick` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`Guid`),
  KEY `Account` (`Account`),
  KEY `CharacterID` (`CharacterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- 数据导出被取消选择。

-- 导出  表 game.t_task 结构
DROP TABLE IF EXISTS `t_task`;
CREATE TABLE IF NOT EXISTS `t_task` (
  `Account` bigint(20) unsigned NOT NULL,
  `TaskData` text NOT NULL,
  PRIMARY KEY (`Account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- 数据导出被取消选择。

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
