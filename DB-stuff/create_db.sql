-- **************************************************
-- ***************   SOLAR ´DB **********************
-- **************************************************

-- TODO
-- Move all DROP TABLE statement to the beginning: in correct order... 
DROP TABLE IF EXISTS event_measurement ;
DROP TABLE IF EXISTS event ;
DROP TABLE IF EXISTS eventtype ;
DROP TABLE IF EXISTS sensor ;
DROP TABLE IF EXISTS quantity ;
DROP TABLE IF EXISTS devloc ;
DROP TABLE IF EXISTS location ;
DROP TABLE IF EXISTS measurement ;
DROP TABLE IF EXISTS device ;
DROP TABLE IF EXISTS devicetype ;
DROP TABLE IF EXISTS message ;
DROP TABLE IF EXISTS xferlog ;
DROP TABLE IF EXISTS tables ;
DROP TABLE IF EXISTS css_position_data;

-- **********************************
-- *********** quantity **********************************************
-- **********************************


CREATE TABLE quantity (
 quantity_id INTEGER PRIMARY KEY,
 quantity varchar, -- ex. temperature
 unit varchar, -- ex. °C
 q_letter char(1)
);

GRANT INSERT ON quantity TO solar;
GRANT SELECT ON quantity TO public;

INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (1,'Temperature','&deg;C','T');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (2,'Pressure','Pa','P');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (3,'Flow','l/s','F');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (4,'Mass','kg','M');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (5,'State','Boolean','S'); --  On/Off, Open/Close
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (6,'Current','A','I');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (7,'Voltage','V','U');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (8,'Length','m','s');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (9,'Time','s','t');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (10,'Frequency','Hz','f');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (11,'Force','N','F');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (12,'Energy','J','E'); -- W*s
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (13,'Power','W','P'); -- J/s
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (14,'Magnetic flux density','T','B');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (15,'Luminous flux','lm','L');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (16,'Illuminance','lx','X');
INSERT INTO quantity (quantity_id, quantity, unit, q_letter) VALUES (17,'Rotation speed','rpm','R');

-- **********************************
-- *********** location **********************************************
-- **********************************


CREATE TABLE location ( -- Both for sensors and actuators/pumps
 location_id INTEGER PRIMARY KEY,
 abbreviation char(3) UNIQUE NOT NULL,
-- shortname varchar,
 location_description varchar,
 location_desciption_sv varchar,
-- purpose_of_monitoring text,
 comment varchar,
-- channel varchar -- where is information pouring in?  > NOT HERE!! <
 old_label varchar UNIQUE
);

GRANT INSERT ON location TO solar;
GRANT SELECT ON location TO public;

INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (1,'SPI','Solar panels, inlet','Dykr&ouml;r','TK1SS');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (2,'SPM','Solar panels, inbetween','Dykr&ouml;r i S1(?)','TM1SS');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (3,'SPO','Solar panels, outlet','This should be hottest point in system when sunny','TV1SS');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (4,'RFA','Roof: free air',' ','TA');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (5,'REB','Roof equipment box',' ','TU');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (6,'SET','Solar circuit expansion tank',' ','MSE');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (7,'SPA','Solar pump assembly',' ','CPS');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (8,'SHO','Solar heat-exchanger outlet',' ','TK2SS');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (9,'SHI','Solar heat-exchanger inlet',' ','TV2SS');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (10,'SST','Solar circuit security tank','Jeepdunk..','WST');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (11,'ATM','Ackumulator tank, mid level','Dykr&ouml;r','TMA');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (12,'ATL','Ackumulator tank, low level','Dykr&ouml;r mellan SHI och SHO','TKA_D');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (13,'TER','Technology room',' ','WF');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (14,'ARP','Ackumulator tank, HE return pipe',' ','TKI');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (15,'HTO','Heatexchanger tankwater out',' ','TDU');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (16,'HDI','Heatexchanger dom. water in',' ','TVI');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (17,'HDO','Heatexchanger dom. water out',' ','TVU');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (18,'HTI','Heatexchanger tankwater in',' ','TDI');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (19,'ATO','Ackumulator tank out',' ','TVA');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (20,'VOD','Valve overheat dump',' ','TVD');
INSERT INTO location (location_id,abbreviation,location_description,comment,old_label) VALUES (21,'DHT','Dom. water heater',' ','TVVB_PT');
INSERT INTO location (location_id,abbreviation,location_description,comment) VALUES (22,'CCC','Control Computers Cupboard',' ');
INSERT INTO location (location_id,abbreviation,location_description,comment) VALUES (23,'OHL','Other House Locations',' ');

-- **********************************
-- *********** devicetype ********************************************
-- **********************************


CREATE TABLE devicetype (
 devicetype_id INTEGER PRIMARY KEY,
 device_class varchar, -- sensor, pump, speedcontroller, valve, flowdetector 
 description text
);

GRANT INSERT ON devicetype TO solar;
GRANT SELECT ON devicetype TO public;

INSERT INTO devicetype (devicetype_id, device_class) VALUES (1,'Sensor');
INSERT INTO devicetype (devicetype_id, device_class) VALUES (2,'Valve');
INSERT INTO devicetype (devicetype_id, device_class) VALUES (3,'Pump');
INSERT INTO devicetype (devicetype_id, device_class) VALUES (4,'Tank');
INSERT INTO devicetype (devicetype_id, device_class) VALUES (5,'FqInverter');
INSERT INTO devicetype (devicetype_id, device_class) VALUES (6,'Computer');
INSERT INTO devicetype (devicetype_id, device_class) VALUES (7,'Solar panel');
INSERT INTO devicetype (devicetype_id, device_class) VALUES (8,'Pipe section');
INSERT INTO devicetype (devicetype_id, device_class) VALUES (9,'Heat exchanger');
-- INSERT INTO devicetype (devicetype_id, device_class) VALUES (1,'');
-- INSERT INTO devicetype (devicetype_id, device_class) VALUES (1,'');

-- **********************************
-- *********** device ************************************************
-- **********************************


CREATE TABLE device (
 device_id INTEGER PRIMARY KEY,
 devicetype INTEGER REFERENCES devicetype,
 serial_no varchar, -- Can be used e.g. for the ID code
 manufacturer varchar, -- ex. Dallas Semiconductor
 typedes varchar,  -- ex. 18B20
 control varchar, -- no control, manual, electric, autonomous, automatic
 information_protocol varchar, -- ex. 1-wire, I2C, 4-20 currentloop.
 deployment_date date
);

GRANT INSERT ON device TO solar;
GRANT SELECT ON device TO public;

INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (1,1,'TV1SS','Dallas','18B20','no','1-wire -> RS-232','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (2,1,'TKA_D','Dallas','18B20','no','1-wire -> RS-232','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (3,1,'TM1SS','Dallas','18B20','no','1-wire -> RS-232','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (4,1,'TK1SS','Dallas','18B20','no','1-wire -> RS-232','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (5,1,'TV2SS','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (6,1,'TK2SS','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (7,1,'TA','Dallas','18B20','no','1-wire -> RS-232','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (8,1,'TU','Dallas','18B20','no','1-wire -> RS-232','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (9,1,'TMA','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (10,1,'TVA','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (11,1,'TDI','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (12,1,'TDU','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (13,1,'TVU','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (14,1,'TVI','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (15,1,'TR','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (16,1,'TGR','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (17,1,'TKI','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (18,1,'TVD','Dallas','18B20','no','1-wire','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (19,1,'TVVB_PT','Pt100','Pt100','no','4-20mA','2015-01-01');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (20,1,'TV1SS_PT','Pt100','Pt100','no','4-20mA','2015-01-01');
-- INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (,,'','','','','','');
-- INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (,,'','','','','','');
-- INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (,,'','','','','','');
INSERT INTO device (device_id,devicetype,serial_no,manufacturer,typedes,control,information_protocol,deployment_date) VALUES (100,6,'RPi-','Farnell','2B','Is the control','I2C,RS232','2015-01-01');


-- **********************************
-- *********** sensor ************************************************
-- **********************************


CREATE TABLE sensor (
-- sensor_id INTEGER PRIMARY KEY, -- NÄ!? det är ju exakt EN device...
 device_id INTEGER NOT NULL REFERENCES device, -- all sensors are devices...
 quantity INTEGER REFERENCES quantity, -- if it is a sensor; what quantity is measured
 range_min float,
 range_max float,
 decimals_resolution integer
);

GRANT INSERT ON sensor TO solar;
GRANT SELECT ON sensor TO public;

INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (1,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (2,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (3,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (4,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (5,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (6,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (7,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (8,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (9,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (10,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (11,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (12,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (13,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (14,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (15,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (16,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (17,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (18,1,-55,125,1);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (19,1,-50,150,2);
INSERT INTO sensor (device_id,quantity,range_min,range_max,decimals_resolution) VALUES (20,1,-50,150,2);
-- INSERT INTO sensor (sensor_id,device_id,quantity,range_min,range_max,decimals_resolution) VALUES (,,,,,,);
-- INSERT INTO sensor (sensor_id,device_id,quantity,range_min,range_max,decimals_resolution) VALUES (,,,,,,);
-- INSERT INTO sensor (sensor_id,device_id,quantity,range_min,range_max,decimals_resolution) VALUES (,,,,,,);

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


-- **********************************
-- *********** measurement *******************************************
-- **********************************


CREATE TABLE measurement (
  measurement_id SERIAL PRIMARY KEY,
--  location_id INTEGER REFERENCES location,  -- Behövs inte här!!
  device_id INTEGER NOT NULL REFERENCES device,
  num_value float,
  status boolean, -- false for e.g. failed validation, testing etc.
  credtm timestamp(0) DEFAULT NOW()
);

GRANT INSERT ON measurement TO solar;
GRANT UPDATE ON measurement_measurement_id_seq TO solar;
GRANT SELECT ON measurement TO public;

INSERT INTO measurement (device_id,num_value,status) VALUES (1,0,false);

-- **********************************
-- *********** eventtype *********************************************
-- **********************************


CREATE TABLE eventtype (  -- This should be control events
  eventtype_id INTEGER PRIMARY KEY,
  event_description varchar
);

GRANT INSERT ON eventtype TO solar;
GRANT SELECT ON eventtype TO public;

INSERT INTO eventtype (eventtype_id,event_description) VALUES (1,'Unspecified event');
INSERT INTO eventtype (eventtype_id,event_description) VALUES (2,'Valve opened');
INSERT INTO eventtype (eventtype_id,event_description) VALUES (3,'Valve closed');
INSERT INTO eventtype (eventtype_id,event_description) VALUES (4,'Pump started');
INSERT INTO eventtype (eventtype_id,event_description) VALUES (5,'Pump stopped');
INSERT INTO eventtype (eventtype_id,event_description) VALUES (6,'Pump speed increased');
INSERT INTO eventtype (eventtype_id,event_description) VALUES (7,'Pump speed decreased');
-- INSERT INTO eventtype (event_description) VALUES (8,'');
-- INSERT INTO eventtype (event_description) VALUES (9,'');
-- INSERT INTO eventtype (event_description) VALUES (10,'');
-- INSERT INTO eventtype (event_description) VALUES (11,'');
-- INSERT INTO eventtype (event_description) VALUES (12,'');
-- INSERT INTO eventtype (event_description) VALUES (13,'');
-- INSERT INTO eventtype (event_description) VALUES (14,'');
-- INSERT INTO eventtype (event_description) VALUES (15,'');
-- INSERT INTO eventtype (event_description) VALUES (16,'');
-- INSERT INTO eventtype (event_description) VALUES (17,'');

-- **********************************
-- *********** event *************************************************
-- **********************************


CREATE TABLE event ( -- control events
  event_id SERIAL PRIMARY KEY,
  eventtype INTEGER REFERENCES eventtype,
  device_id INTEGER REFERENCES device,    -- ??
  credtm timestamp(0) DEFAULT NOW()
);

GRANT INSERT ON event TO solar;
GRANT UPDATE ON event_event_id_seq TO solar;
GRANT SELECT ON event TO public;


INSERT INTO event (eventtype,device) VALUES (1,100);
-- INSERT INTO event (eventtype,device) VALUES (,);
-- INSERT INTO event (eventtype,device) VALUES (,);
-- INSERT INTO event (eventtype,device) VALUES (,);
-- INSERT INTO event (eventtype,device) VALUES (,);


-- **********************************
-- *********** message **********************************************
-- **********************************


CREATE TABLE message (  -- control happenings in event table, other system things in this table, e.g. things regarding software, database, tech. communication etc. = things not interesting from a solar energy point-of-view
  message_id SERIAL PRIMARY KEY,
  messagetext varchar,
  credtm timestamp(0) DEFAULT NOW()
);


INSERT INTO message (messagetext) VALUES ('db was created');
-- INSERT INTO message (messagetext) VALUES ('');
-- INSERT INTO message (messagetext) VALUES ('');
-- INSERT INTO message (messagetext) VALUES ('');
-- INSERT INTO message (messagetext) VALUES ('');

GRANT INSERT ON message TO solar;
GRANT UPDATE ON message_message_id_seq TO solar;
GRANT SELECT ON message TO public;

-- **********************************
-- *********** tables ******************************************
-- ** Describes database tables
-- **********************************


CREATE TABLE tables (
 table_id INTEGER PRIMARY KEY,
 table_name VARCHAR,
 table_description VARCHAR
);

GRANT INSERT ON tables TO solar;
GRANT SELECT ON tables TO public;

INSERT INTO tables (table_id,table_name, table_description) VALUES (1,'tables','lists all tables in the database');
INSERT INTO tables (table_id,table_name, table_description) VALUES (2,'quantity','quantities and units');
INSERT INTO tables (table_id,table_name, table_description) VALUES (3,'location','system locations or sites; points of interest to measure, monitor or control');
INSERT INTO tables (table_id,table_name, table_description) VALUES (4,'devicetype','type of devices in the system');
INSERT INTO tables (table_id,table_name, table_description) VALUES (5,'device','list of all devices in the system');
INSERT INTO tables (table_id,table_name, table_description) VALUES (6,'sensor','extended properties of the devicetype "sensor"');
INSERT INTO tables (table_id,table_name, table_description) VALUES (7,'devloc','mapping of devices to locations');
INSERT INTO tables (table_id,table_name, table_description) VALUES (8,'measurement','measurements; data or state information');
INSERT INTO tables (table_id,table_name, table_description) VALUES (9,'eventtype','different types of events that can occur in the system');
INSERT INTO tables (table_id,table_name, table_description) VALUES (10,'event','list of events related to ssytem control or state, with timestamps. Often to be associated with measurements');
INSERT INTO tables (table_id,table_name, table_description) VALUES (11,'event_measurement','relation between events and measurements');
INSERT INTO tables (table_id,table_name, table_description) VALUES (12,'message','notices about things not directly related to the control of solar energy harvesting, e.g. database, software or communication things.');
INSERT INTO tables (table_id,table_name, table_description) VALUES (13,'tables','list of tables in this database');
INSERT INTO tables (table_id,table_name, table_description) VALUES (14,'xferlog','a log of data transfers from/to this system and other datasites');
-- INSERT INTO tables (table_id,table_name, table_description) VALUES (15,'','');
-- INSERT INTO tables (table_id,table_name, table_description) VALUES (16,'','');
-- INSERT INTO tables (table_id,table_name, table_description) VALUES (17,'','');

-- **********************************
-- *********** transferlog ******************************************
-- ** Describes data transfer to external systems, e.g. publication servers
-- **********************************


CREATE TABLE xferlog (
 xfer_id SERIAL PRIMARY KEY,
 xferred_table INTEGER REFERENCES tables,
 last_xferred_id INTEGER,
 xferdtm timestamp(0) DEFAULT NOW(),
 success boolean,
 credtm timestamp(0) DEFAULT NOW()
);

GRANT INSERT ON xferlog TO solar;
GRANT UPDATE ON xferlog_xfer_id_seq TO solar;
GRANT SELECT ON xferlog TO public;

-- **********************************
-- *********** css_position_data ******************************************
-- ** Defines position of elements on the graphical display
-- **********************************


CREATE TABLE css_position_data (
device_id INTEGER REFERENCES device,
leftpos INTEGER,
rightpos INTEGER
);

INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (1,270,96);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (2,308,843);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (3,420,96);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (4,550,96);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (5,230,810);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (6,230,890);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (7,100,120);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (8,100,170);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (9,308,760);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (10,372,440);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (11,410,590);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (12,410,720);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (13,502,590);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (14,502,700);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (15,310,350);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (16,310,365);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (17,600,850);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (18,580,450);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (19,495,280);
INSERT INTO css_position_data (device_id,leftpos,rightpos) VALUES (20,270,80);

CREATE OR REPLACE FUNCTION sensorlabel(integer) RETURNS varchar AS
'SELECT abbreviation||''_''||q_letter||enumeration FROM device JOIN devloc USING (device_id) 
JOIN location USING (location_id) JOIN sensor USING (device_id) JOIN quantity ON 
(sensor.quantity=quantity.quantity_id) WHERE device_id=$1;' LANGUAGE SQL;