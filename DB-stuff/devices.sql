-- List devices with some basic attributes

select device_id, device_class, serial_no, sensorlabel(device_id), quantity.quantity FROM device LEFT JOIN sensor USING (device_id)  JOIN devicetype USING (devicetype_id) LEFT JOIN quantity ON (sensor.quantity=quantity.quantity_id) ORDER BY device_class,device_id;
