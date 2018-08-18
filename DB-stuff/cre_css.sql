

CREATE TABLE css_position_data (
device_id INTEGER REFERENCES device,
leftpos INTEGER,
rightpos INTEGER
);

GRANT SELECT ON css_position_data TO public;

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

