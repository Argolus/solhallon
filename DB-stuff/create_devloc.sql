
-- **********************************
-- *********** device at location devloc  ************************************************
-- **********************************


CREATE TABLE devloc (
 devloc_id INTEGER PRIMARY KEY,
 location_id INTEGER REFERENCES location,
 device_id INTEGER REFERENCES device,
 enumeration INTEGER,
 binding_date date DEFAULT NOW()
);

GRANT INSERT ON devloc TO solar;
GRANT SELECT ON devloc TO public;

INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (1,3,1,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (2,12,2,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (3,2,3,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (4,1,4,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (5,9,5,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (6,8,6,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (7,4,7,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (8,5,8,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (9,11,9,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (10,19,10,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (11,18,11,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (12,15,12,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (13,17,13,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (14,16,14,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (15,13,15,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (16,23,16,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (17,14,17,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (18,20,18,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (19,21,19,1);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (20,3,20,2);
INSERT INTO devloc (devloc_id,location_id,device_id,enumeration) VALUES (21,22,100,1);
-- INSERT INTO devloc (devloc_id,location_id,device_id) VALUES (,,);



CREATE OR REPLACE FUNCTION sensorlabel(integer) RETURNS varchar AS
'SELECT abbreviation||''_''||q_letter||enumeration FROM device JOIN devloc USING (device_id) 
JOIN location USING (location_id) JOIN sensor USING (device_id) JOIN quantity ON 
(sensor.quantity=quantity.quantity_id) WHERE device_id=$1;' LANGUAGE SQL;