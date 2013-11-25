SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

DROP SCHEMA IF EXISTS `poseidondb` ;
CREATE SCHEMA IF NOT EXISTS `poseidondb` DEFAULT CHARACTER SET utf8 ;
USE `poseidondb` ;

-- -----------------------------------------------------
-- Table `poseidondb`.`actiontrigger`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `poseidondb`.`actiontrigger` ;

CREATE  TABLE IF NOT EXISTS `poseidondb`.`actiontrigger` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `outlet_id` INT(11) NULL DEFAULT NULL COMMENT 'Output_id can be NULL when a sensor_id is monitoring but does not have a connected outlet (e.g. a thermometer is connected to a wall-outlet, but the temperature sensor is still monitoring tank values)' ,
  `sensor_id` INT(11) NULL DEFAULT NULL ,
  `controltype` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `thresholdlow` FLOAT NULL DEFAULT NULL ,
  `warnlow` FLOAT NULL DEFAULT NULL ,
  `warnhigh` FLOAT NULL DEFAULT NULL ,
  `thresholdhigh` FLOAT NULL DEFAULT NULL ,
  `okaction` TINYINT(4) NOT NULL DEFAULT '1' ,
  `ts` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 9
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;


-- -----------------------------------------------------
-- Table `poseidondb`.`description`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `poseidondb`.`description` ;

CREATE  TABLE IF NOT EXISTS `poseidondb`.`description` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `tank_id` INT(11) NOT NULL ,
  `name` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `manufactor` VARCHAR(1000) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `description` VARCHAR(2000) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `acquisition` DATE NOT NULL ,
  `maintenancelast` DATE NOT NULL ,
  `maintenancenext` DATE NOT NULL ,
  `output_id` INT(11) NOT NULL ,
  `updated` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;


-- -----------------------------------------------------
-- Table `poseidondb`.`environment`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `poseidondb`.`environment` ;

CREATE  TABLE IF NOT EXISTS `poseidondb`.`environment` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `etype` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `name` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `value` INT(11) NOT NULL ,
  `tank_id` INT(11) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;


-- -----------------------------------------------------
-- Table `poseidondb`.`log`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `poseidondb`.`log` ;

CREATE  TABLE IF NOT EXISTS `poseidondb`.`log` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `logtype` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `logtext` VARCHAR(2000) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `ts` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP ,
  `tank_id` INT(11) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;


-- -----------------------------------------------------
-- Table `poseidondb`.`outlet`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `poseidondb`.`outlet` ;

CREATE  TABLE IF NOT EXISTS `poseidondb`.`outlet` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `name` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `manualoverride` TINYINT(4) NOT NULL DEFAULT '0' ,
  `value` TINYINT(4) NOT NULL ,
  `tank_id` INT(11) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 6
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;


-- -----------------------------------------------------
-- Table `poseidondb`.`outlettimer`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `poseidondb`.`outlettimer` ;

CREATE  TABLE IF NOT EXISTS `poseidondb`.`outlettimer` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `outlet_id` INT(11) NOT NULL ,
  `starttime` TIME NOT NULL ,
  `stoptime` TIME NOT NULL ,
  `ts` TIMESTAMP NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 4
DEFAULT CHARACTER SET = utf8
COMMENT = '	';


-- -----------------------------------------------------
-- Table `poseidondb`.`rpi`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `poseidondb`.`rpi` ;

CREATE  TABLE IF NOT EXISTS `poseidondb`.`rpi` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `mac` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `hostip` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `publicip` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 2
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;


-- -----------------------------------------------------
-- Table `poseidondb`.`sensor`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `poseidondb`.`sensor` ;

CREATE  TABLE IF NOT EXISTS `poseidondb`.`sensor` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `uid` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL COMMENT 'Unique Identificator' ,
  `sensortype` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `active` TINYINT(4) NOT NULL ,
  `maxvalue` DOUBLE NOT NULL ,
  `minvalue` DOUBLE NOT NULL ,
  `thresholdhigh` FLOAT NULL DEFAULT NULL ,
  `thresholdlow` FLOAT NULL DEFAULT NULL ,
  `warnhigh` FLOAT NULL DEFAULT NULL ,
  `warnlow` FLOAT NULL DEFAULT NULL ,
  `tank_id` INT(11) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 7
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;


-- -----------------------------------------------------
-- Table `poseidondb`.`sensorreading`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `poseidondb`.`sensorreading` ;

CREATE  TABLE IF NOT EXISTS `poseidondb`.`sensorreading` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `sensor_id` INT(11) NOT NULL ,
  `tank_id` INT(11) NOT NULL ,
  `reading` INT(11) NOT NULL ,
  `calculated` DOUBLE NOT NULL ,
  `ts` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 94
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;


-- -----------------------------------------------------
-- Table `poseidondb`.`tank`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `poseidondb`.`tank` ;

CREATE  TABLE IF NOT EXISTS `poseidondb`.`tank` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `name` VARCHAR(250) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `rpi_id` INT(11) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 2
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

USE `poseidondb` ;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
