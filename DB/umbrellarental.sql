/*
 Navicat Premium Data Transfer

 Source Server         : test1
 Source Server Type    : MySQL
 Source Server Version : 50720
 Source Host           : localhost:3306
 Source Schema         : umbrellarental

 Target Server Type    : MySQL
 Target Server Version : 50720
 File Encoding         : 65001

 Date: 26/11/2017 17:37:41
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for hashcode
-- ----------------------------
DROP TABLE IF EXISTS `hashcode`;
CREATE TABLE `hashcode`  (
  `userID` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT '0',
  `umbID` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT '0',
  `hashCode` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '0',
  PRIMARY KEY (`hashCode`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for nearest_point
-- ----------------------------
DROP TABLE IF EXISTS `nearest_point`;
CREATE TABLE `nearest_point`  (
  `rs_id` int(20) NOT NULL DEFAULT 0 COMMENT '대여지점id',
  `umbrella_id` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '0' COMMENT '소유중인 우산 id들',
  `structure` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '0' COMMENT '대여기 배열(x X y)',
  `vacancy` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '111111' COMMENT '1 is 포착상태, 0 is  빈 자리',
  `latitude` double(7, 4) NOT NULL DEFAULT 0.0000 COMMENT '좌표,경도',
  `longitude` double(7, 4) NOT NULL DEFAULT 0.0000 COMMENT '좌표,위도',
  PRIMARY KEY (`rs_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for rental_spot
-- ----------------------------
DROP TABLE IF EXISTS `rental_spot`;
CREATE TABLE `rental_spot`  (
  `rs_id` int(20) NOT NULL DEFAULT 0 COMMENT '대여지점id',
  `umbrella_id` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '0' COMMENT '소유중인 우산 id들',
  `structure` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '0' COMMENT '대여기 배열(x X y)',
  `vacancy` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '111111' COMMENT '1 is 포착상태, 0 is  빈 자리',
  `latitude` double(7, 4) NOT NULL DEFAULT 0.0000 COMMENT '좌표,경도',
  `longitude` double(7, 4) NOT NULL DEFAULT 0.0000 COMMENT '좌표,위도',
  PRIMARY KEY (`rs_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for umbrella
-- ----------------------------
DROP TABLE IF EXISTS `umbrella`;
CREATE TABLE `umbrella`  (
  `umbrella_id` int(20) NOT NULL DEFAULT 0 COMMENT '우산id',
  `umbrella_status` int(1) NOT NULL DEFAULT 1 COMMENT '현재 우산 상태(1 is 보관,0 is 대여)',
  `borrower_id` int(20) NOT NULL DEFAULT 0 COMMENT '대여중일 경우 대여하는 user의 id',
  `slot_label` int(2) NOT NULL DEFAULT 0 COMMENT '우산이 몇 번째 자리에 있다.',
  PRIMARY KEY (`umbrella_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user`  (
  `user_id` int(20) NOT NULL DEFAULT 0 COMMENT 'unique id',
  `user_acc` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '0' COMMENT 'user account',
  `user_pw` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '0' COMMENT 'user password',
  `umbrella` int(20) NOT NULL DEFAULT 0 COMMENT '대여중인 우산 id',
  `hash_code` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '0' COMMENT 'hash_code',
  PRIMARY KEY (`user_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Procedure structure for dist
-- ----------------------------
DROP PROCEDURE IF EXISTS `dist`;
delimiter ;;
CREATE DEFINER=`root`@`%` PROCEDURE `dist`(IN lat_user DOUBLE(7,4),IN lng_user 
DOUBLE(7,4),IN dist int(5))
BEGIN

-- 需要定义接收游标数据的变量 
  -- 遍历数据结束标志


  DECLARE done INT DEFAULT FALSE;
	
	
  DECLARE rsid int(20);  -- rs_id
	DECLARE unbid VARCHAR(255); -- umbrella_id
  DECLARE str VARCHAR(255); -- structure
	DECLARE vac VARCHAR(255); -- vacancy
	DECLARE lat DOUBLE(7,4); -- 纬度
	DECLARE lng DOUBLE(7,4); -- 经度
	

	
  -- 游标
  DECLARE cur CURSOR FOR SELECT rs_id,umbrella_id,structure,vacancy,latitude,longitude FROM rental_spot;

  -- 将结束标志绑定到游标
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
  -- 打开游标
  OPEN cur;
	truncate table nearest_point; 
  -- 开始循环
  youbiao: LOOP
    -- 提取游标里的数据
    FETCH cur INTO rsid,unbid,str,vac,lat,lng;
		
    -- 声明结束的时候
    IF done THEN
      LEAVE youbiao;
    END IF;
    -- 事件
		
	
		
		SET @tem=(2 * 6367000* ASIN(SQRT(POW(SIN((lat_user-lat)*PI()/180*0.5),2)+COS(lat_user*PI()/180)* COS(lat * PI()/180)*POW(SIN((lng_user-lng)*PI()/180*0.5),2))));
   	 IF @tem<dist THEN
		 insert into nearest_point VALUES (rsid,unbid,str,vac,lat,lng);
		 END IF;

  END LOOP;
  -- 关闭游标
  CLOSE cur;

SELECT * FROM nearest_point;
END
;;
delimiter ;

-- ----------------------------
-- Procedure structure for Uni_delete
-- ----------------------------
DROP PROCEDURE IF EXISTS `Uni_delete`;
delimiter ;;
CREATE DEFINER=`root`@`%` PROCEDURE `Uni_delete`(IN tname varchar(64),IN Claim VARCHAR(255))
BEGIN

SET @sqlcmd = CONCAT('DELETE FROM ',tname);  

IF Claim<>'0' THEN
	SET @sqlcmd = CONCAT(@sqlcmd,' WHERE ',Claim); 
END IF;

PREPARE stmt FROM @sqlcmd;  
EXECUTE stmt;  
DEALLOCATE PREPARE stmt;  
END
;;
delimiter ;

-- ----------------------------
-- Procedure structure for Uni_Inquire
-- ----------------------------
DROP PROCEDURE IF EXISTS `Uni_Inquire`;
delimiter ;;
CREATE DEFINER=`root`@`%` PROCEDURE `Uni_Inquire`(IN tname varchar(64),IN colname VARCHAR(255), iscondition varchar(255))
BEGIN
IF colname<>'0' THEN
SET @sqlcmd = CONCAT('SELECT ',colname,' FROM ');
ELSE
SET @sqlcmd ='SELECT * FROM ';
END IF;

SET @sqlcmd = CONCAT(@sqlcmd,tname);  

IF iscondition<>'0' THEN
	SET @sqlcmd = CONCAT(@sqlcmd,' WHERE ',iscondition); 
END IF;

PREPARE stmt FROM @sqlcmd;  
EXECUTE stmt;  
DEALLOCATE PREPARE stmt;  
END
;;
delimiter ;

-- ----------------------------
-- Procedure structure for Uni_insert
-- ----------------------------
DROP PROCEDURE IF EXISTS `Uni_insert`;
delimiter ;;
CREATE DEFINER=`root`@`%` PROCEDURE `Uni_insert`(IN tname varchar(64),IN colname VARCHAR(255))
BEGIN

SET @sqlcmd = CONCAT("insert into ",tname);  

SET @sqlcmd = CONCAT(@sqlcmd," values(",colname,");"); 

PREPARE stmt FROM @sqlcmd;  
EXECUTE stmt;  
DEALLOCATE PREPARE stmt;  
END
;;
delimiter ;

-- ----------------------------
-- Procedure structure for uni_update
-- ----------------------------
DROP PROCEDURE IF EXISTS `uni_update`;
delimiter ;;
CREATE DEFINER=`root`@`%` PROCEDURE `uni_update`(IN tname varchar(64),IN colname VARCHAR(255), iscondition varchar(255))
BEGIN

SET @sqlcmd = CONCAT('UPDATE ',tname,' SET ',colname);  


IF iscondition<>'0' THEN
	SET @sqlcmd = CONCAT(@sqlcmd,' WHERE ',iscondition); 
END IF;

PREPARE stmt FROM @sqlcmd;  
EXECUTE stmt;  
DEALLOCATE PREPARE stmt;  
END
;;
delimiter ;

SET FOREIGN_KEY_CHECKS = 1;
