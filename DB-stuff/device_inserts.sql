INSERT INTO device (device_id,serial_no) VALUES(21,'28-FF-3C-BD-53-14-01-F7');
INSERT INTO device (device_id,serial_no) VALUES(22,'28-FF-16-1E-50-14-01-77');
INSERT INTO device (device_id,serial_no) VALUES(23,'28-FF-52-CA-53-14-01-6A');
INSERT INTO device (device_id,serial_no) VALUES(24,'28-FF-BD-C7-53-14-01-42');
INSERT INTO device (device_id,serial_no) VALUES(25,'28-FF-97-4B-53-14-01-CC');
INSERT INTO device (device_id,serial_no) VALUES(26,'28-FF-5C-5B-6F-14-04-FC');
INSERT INTO device (device_id,serial_no) VALUES(27,'28-FF-E0-58-6F-14-04-1A');
INSERT INTO device (device_id,serial_no) VALUES(28,'28-FF-E4-74-6F-14-04-67');
INSERT INTO device (device_id,serial_no) VALUES(29,'28-FF-35-5F-53-14-01-4A');
INSERT INTO device (device_id,serial_no) VALUES(30,'28-FF-D2-B7-53-14-01-94');
INSERT INTO device (device_id,serial_no) VALUES(31,'28-FF-EA-BE-53-14-01-BD');
INSERT INTO device (device_id,serial_no) VALUES(32,'28-FF-A9-34-53-14-01-D8');
INSERT INTO device (device_id,serial_no) VALUES(33,'28-FF-1D-68-53-14-01-42');
INSERT INTO device (device_id,serial_no) VALUES(34,'28-FF-FC-74-6F-14-04-25');
UPDATE device SET manufacturer='Dallas' where manufacturer is null;
UPDATE device SET devicetype=1 where devicetype is null;
UPDATE device SET typedes='18B20' where typedes is null;
UPDATE device SET control='no' where control is null;
UPDATE device SET information_protocol='1-wire' where information_protocol is null;
