



void StoreSENSORS(){
  int bSize = sizeof(SENSORS);
  byte *pBuf = (byte*)SENSORS;
  for(int i=0; i<bSize; i++){
    WireEepromWriteByte(STARTADDRESS_SENSORS + i, *pBuf++);
  }
  uint16_t checksum = fletcher16((byte*)SENSORS, bSize);
  WireEepromWriteByte(STARTADDRESS_SENSORS + bSize, (checksum & 0xFF00) >> 8);
  WireEepromWriteByte(STARTADDRESS_SENSORS + bSize +1, (checksum & 0xFF));
  WireEepromWriteByte(STARTADDRESS_SENSORS + bSize +2, 0xA5);
}

void StoreSensor(teValIndex index){
  if(index<NR_OF_SENSORS){
    byte offset = index*sizeof(t_Value);
    byte *pBuf = (byte*)(SENSORS + offset);
    for(int i=0; i<sensorIDLen; i++){
      WireEepromWriteByte(STARTADDRESS_SENSORS + offset++, *pBuf++);
    }
  }    
}
  
void LoadSENSORS(){
  int bSize = sizeof(SENSORS);
  byte* pBuf = (byte*)SENSORS;
  RaspiPrint("Load Sensors : ");
  RaspiPrintln(bSize);
  for(int i=0; i<bSize; i++){
    *pBuf=WireEepromRead(STARTADDRESS_SENSORS + i);
    RaspiPrint(*pBuf);
    RaspiPrint(" ");
    pBuf++;
  }
  uint16_t E2checksum = (WireEepromRead(STARTADDRESS_SENSORS + bSize) << 8 ) | WireEepromRead(STARTADDRESS_SENSORS + bSize+1);
  byte CTRL = WireEepromRead(STARTADDRESS_SENSORS + bSize + 2);
  uint16_t checksum = fletcher16((byte*)SENSORS, bSize);
  RaspiPrint("E2checksum: ");
  RaspiPrintln(E2checksum);
  RaspiPrint("checksum: ");
  RaspiPrintln(checksum);
  if(E2checksum != checksum || CTRL != 0xA5){
    sendErrorToRaspi("Memory Error EEPROM");
    ResetAllSensorIDs();
  }
}

void UpdateSensorAddress(teValIndex Index, byte *Addr){
  if(Index<NR_OF_SENSORS){ 
    for(int i = 0; i<sensorIDLen; i++){
      SENSORS[Index].ADDR[i] = Addr[i];
    }
    StoreSensor(Index);
  }
}

teValIndex SensorIndex(byte SensorID[]){
  int i = 0;
  bool found = false;
  do{
    found = IsEqualSensorID(SENSORS[i].ADDR, SensorID);
    if(!found) i++;
  }while(!found && i<NR_OF_SENSORS);  
  if(!found){
    sendErrorToRaspi("SensorIndex not found: ");
    RaspiPrint(byteAsHexString(SensorID[0],false));
    for(int j=1; j<8; j++){
      RaspiPrint("-");
      RaspiPrint(byteAsHexString(SensorID[j],false));
    }
    RaspiPrintln();
    return NR_OF_SENSORS;
  }
  return (teValIndex)i;  
}

boolean IsEqualSensorID(byte Sensor1[], byte Sensor2[]){
  boolean IsEqual = false; 
  int LengthOfID=sensorIDLen;
  do{
    LengthOfID--;
    IsEqual = Sensor1[LengthOfID] == Sensor2[LengthOfID];
  }while(LengthOfID>0 && IsEqual);
  return IsEqual;
}  


byte adSPO_T1[8] = {0x28, 0xFF, 0xE0, 0x58, 0x6F, 0x14, 0x04, 0x1A}; 
byte adSPO_T2[8] = {0x28, 0xFF, 0x97, 0x4B, 0x53, 0x14, 0x01, 0xCC}; 
byte adSPO_T3[8] = {0x28, 0xFF, 0xEA, 0xBE, 0x53, 0x14, 0x01, 0xBD}; 
byte adSPM_T1[8] = {0x28, 0xFF, 0x5C, 0x5B, 0x6F, 0x14, 0x04, 0xFC}; 
byte adSPM_T2[8] = {0x28, 0xFF, 0xBD, 0xC7, 0x53, 0x14, 0x01, 0x42}; 
byte adSPM_T3[8] = {0x28, 0xFF, 0x16, 0x1E, 0x50, 0x14, 0x01, 0x77}; 
byte adSPI_T1[8] = {0x28, 0xFF, 0xE4, 0x74, 0x6F, 0x14, 0x04, 0x67}; 
byte adSPI_T2[8] = {0x28, 0xFF, 0x3C, 0xBD, 0x53, 0x14, 0x01, 0xF7}; 
byte adSPI_T3[8] = {0x28, 0xFF, 0x1D, 0x68, 0x53, 0x14, 0x01, 0x42};
byte adRFA_T1[8] = {0x28, 0xFF, 0xA9, 0x34, 0x53, 0x14, 0x01, 0xD8};
byte adREB_T1[8] = {0x28, 0xFF, 0xFC, 0x74, 0x6F, 0x13, 0x04, 0x25}; 
byte ad_ACK_TANK_HIGH_1_DALLAS[8]   = {0x28, 0xFF, 0x52, 0xCA, 0x53, 0x14, 0x01, 0x6A}; 
byte ad_ACK_TANK_HIGH_2_DALLAS[8]   = {0x28, 0xFF, 0x35, 0x5F, 0x53, 0x14, 0x01, 0x4A}; 
byte ad_ACK_TANK_OUT_DALLAS[8]     = {0x28, 0x9C, 0x55, 0xDD, 0x06, 0x00, 0x00, 0xD8}; 
byte ad_ACK_TANK_MID_1_DALLAS[8]   = {0x28, 0xFF, 0x68, 0x74, 0x6F, 0x14, 0x04, 0x8D}; 
byte ad_ACK_TANK_LOW_1_DALLAS[8]   = {0x28, 0xFF, 0xC9, 0x01, 0x05, 0x15, 0x03, 0xD2}; 
byte ad_ACK_TANK_LOW_2_DALLAS[8]   = {0x28, 0x16, 0x22, 0x08, 0x00, 0x00, 0x80, 0x1C}; 
byte ad_ACK_TANK_LOW_3_DALLAS[8]   = {0x28, 0xB7, 0x20, 0x1F, 0x00, 0x00, 0x80, 0x4A}; 
byte ad_SOLAR_HEATEX_OUT_DALLAS[8] = {0x28, 0xFF, 0x14, 0x04, 0x05, 0x15, 0x03, 0xE1}; 
byte ad_SOLAR_HEATEX_IN_DALLAS[8]  = {0x28, 0xFF, 0xA2, 0x04, 0x05, 0x15, 0x03, 0x32}; 
byte ad_HEATEX_TANK_IN_DALLAS[8]   = {0x28, 0x54, 0x0C, 0xDE, 0x06, 0x00, 0x00, 0x08}; 
byte ad_HEATEX_TANK_OUT_DALLAS[8]  = {0x28, 0x82, 0xFB, 0xDC, 0x06, 0x00, 0x00, 0x8E}; 
byte ad_HEATEX_DOM_IN_DALLAS[8]    = {0x28, 0x90, 0xFB, 0xDD, 0x06, 0x00, 0x00, 0x34}; 
byte ad_HEATEX_DOM_OUT_DALLAS[8]   = {0x28, 0xEC, 0x3D, 0xDD, 0x06, 0x00, 0x00, 0x6F}; 
byte ad_TECH_ROOM_DALLAS[8]        = {0x28, 0x4B, 0xA4, 0xDE, 0x06, 0x00, 0x00, 0x7A}; 
byte ad_DUMP_VALVE_DALLAS[8]       = {0x28, 0xFF, 0x9F, 0xED, 0x04, 0x15, 0x03, 0x2B}; 
byte ad_ACK_TANK_HEATEX_RETURN_DALLAS[8] = {0x28, 0xFF, 0x3E, 0x5B, 0x6F, 0x14, 0x04, 0x6E}; 

void ResetAllSensorIDs(){
  UpdateSensorAddress(SOLAR_OUTLET_COM1, adSPO_T1);
  UpdateSensorAddress(SOLAR_MID_COM1, adSPM_T1);
  UpdateSensorAddress(SOLAR_INLET_COM1, adSPI_T1);
  UpdateSensorAddress(FREE_AIR_COM1, adRFA_T1);
  UpdateSensorAddress(EQU_BOX_COM1, adREB_T1);
  UpdateSensorAddress(SOLAR_OUTLET_COM2, adSPO_T2);
  UpdateSensorAddress(SOLAR_MID_COM2, adSPM_T2);
  UpdateSensorAddress(SOLAR_INLET_COM2, adSPI_T2);
  UpdateSensorAddress(SOLAR_OUTLET_COM3, adSPO_T3);
  UpdateSensorAddress(SOLAR_MID_COM3, adSPM_T3);
  UpdateSensorAddress(SOLAR_INLET_COM3, adSPI_T3);
  UpdateSensorAddress(ACK_TANK_HIGH_1_DALLAS, ad_ACK_TANK_HIGH_1_DALLAS);
  UpdateSensorAddress(ACK_TANK_HIGH_2_DALLAS, ad_ACK_TANK_HIGH_2_DALLAS);
  UpdateSensorAddress(ACK_TANK_OUT_DALLAS, ad_ACK_TANK_OUT_DALLAS);
  UpdateSensorAddress(ACK_TANK_MID_1_DALLAS, ad_ACK_TANK_HEATEX_RETURN_DALLAS);
  UpdateSensorAddress(ACK_TANK_LOW_1_DALLAS, ad_ACK_TANK_LOW_1_DALLAS);
  UpdateSensorAddress(ACK_TANK_LOW_2_DALLAS,ad_ACK_TANK_LOW_2_DALLAS);
  UpdateSensorAddress(ACK_TANK_LOW_3_DALLAS, ad_ACK_TANK_LOW_3_DALLAS);
  UpdateSensorAddress(SOLAR_HEATEX_OUT_DALLAS, ad_SOLAR_HEATEX_OUT_DALLAS);
  UpdateSensorAddress(SOLAR_HEATEX_IN_DALLAS, ad_SOLAR_HEATEX_IN_DALLAS);
  UpdateSensorAddress(HEATEX_TANK_IN_DALLAS, ad_HEATEX_TANK_IN_DALLAS);
  UpdateSensorAddress(HEATEX_TANK_OUT_DALLAS, ad_HEATEX_TANK_OUT_DALLAS);
  UpdateSensorAddress(HEATEX_DOM_IN_DALLAS, ad_HEATEX_DOM_IN_DALLAS);
  UpdateSensorAddress(HEATEX_DOM_OUT_DALLAS, ad_HEATEX_DOM_OUT_DALLAS);
  UpdateSensorAddress(TECH_ROOM_DALLAS, ad_TECH_ROOM_DALLAS);
  UpdateSensorAddress(DUMP_VALVE_DALLAS, ad_DUMP_VALVE_DALLAS);
  UpdateSensorAddress(ACK_TANK_HEATEX_RETURN_DALLAS, ad_ACK_TANK_HEATEX_RETURN_DALLAS);
}



