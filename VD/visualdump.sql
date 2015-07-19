-- --------------------------------------------------------
-- Hôte :                        127.0.0.1
-- Version du serveur:           10.0.19-MariaDB - mariadb.org binary distribution
-- SE du serveur:                Win64
-- HeidiSQL Version:             9.2.0.4947
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Export de la structure de la base pour visualdump
CREATE DATABASE IF NOT EXISTS `visualdump` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `visualdump`;


-- Export de la structure de table visualdump. employee
CREATE TABLE IF NOT EXISTS `employee` (
  `idEmployee` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `firstName` text NOT NULL,
  `lastName` text NOT NULL,
  `gender` tinyint(4) NOT NULL,
  `birthDate` date NOT NULL,
  PRIMARY KEY (`idEmployee`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- L'exportation de données n'était pas sélectionnée.


-- Export de la structure de table visualdump. recongitioninfo
CREATE TABLE IF NOT EXISTS `recongitioninfo` (
  `idEmployee` bigint(20) unsigned NOT NULL,
  `timestampValue` int(11) NOT NULL,
  `status` int(11) NOT NULL,
  PRIMARY KEY (`idEmployee`,`timestampValue`,`status`),
  CONSTRAINT `FK1_iEmployee` FOREIGN KEY (`idEmployee`) REFERENCES `employee` (`idEmployee`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- L'exportation de données n'était pas sélectionnée.


-- Export de la structure de table visualdump. vsapiuser
CREATE TABLE IF NOT EXISTS `vsapiuser` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `firstName` text NOT NULL,
  `lastName` text NOT NULL,
  `username` text NOT NULL,
  `password` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- L'exportation de données n'était pas sélectionnée.
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
