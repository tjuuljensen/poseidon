/*
-- Query: SELECT * FROM poseidondb.sensor
LIMIT 0, 1000

-- Date: 2013-05-05 23:35
*/
INSERT INTO `sensor` (`id`,`name`,`sensortype`,`active`,`maxvalue`,`minvalue`,`tank_id`) VALUES (1,'LightSensor1','light',1,2500000,10000,1);
INSERT INTO `sensor` (`id`,`name`,`sensortype`,`active`,`maxvalue`,`minvalue`,`tank_id`) VALUES (2,'FlowSensor1','flow',1,1500,0,1);
INSERT INTO `sensor` (`id`,`name`,`sensortype`,`active`,`maxvalue`,`minvalue`,`tank_id`) VALUES (3,'TemperaturSensor1','temp',1,4000,1600,1);
INSERT INTO `sensor` (`id`,`name`,`sensortype`,`active`,`maxvalue`,`minvalue`,`tank_id`) VALUES (4,'PHsensor1','ph',1,1400,0,1);
INSERT INTO `sensor` (`id`,`name`,`sensortype`,`active`,`maxvalue`,`minvalue`,`tank_id`) VALUES (5,'CO2','co2',1,50,0,1);
