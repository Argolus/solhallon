



void StoreSENSORS(){
  int bSize = sizeof(SENSORS);
  byte *pBuf = (byte*)SENSORS;
  for(int i=0; i<bSize; i++){
    WireEepromWriteByte(STARTADDRESS_SENSORS + i, *pBuf++);
  }
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
  byte* pBuf = (byte*)SENSORS;
  for(int i=0; i<sizeof(SENSORS); i++){
    *pBuf=WireEepromRead(STARTADDRESS_SENSORS + i);
    pBuf++;
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
  return (teValIndex)i;  
}

boolean IsEqualSensorID(byte Sensor1[], byte Sensor2[]){
  boolean IsEqual = false; 
  int LengthOfID=sensorIDLen;
  do{
    LengthOfID--;
    IsEqual = Sensor1[LengthOfID] == Sensor2[LengthOfID];
  }while(LengthOfID>0 && IsEqual);
}  

/*
byte adSPO_T1[8] = {0x28, 0xB8, 0x46, 0xDE, 0x06, 0x00, 0x00, 0x08}; 
byte adSPO_T2[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPO_T3[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPM_T1[8] = {0x28, 0x9D, 0x70, 0xE8, 0x05, 0x00, 0x00, 0xE8}; 
byte adSPM_T2[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPM_T3[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPI_T1[8] = {0x28, 0x48, 0x51, 0xDE, 0x06, 0x00, 0x00, 0x57}; 
byte adSPI_T2[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPI_T3[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
byte adRFA_T1[8] = {0x28, 0x37, 0x72, 0xDC, 0x06, 0x00, 0x00, 0x5A};
byte adREB_T1[8] = {0x28, 0x49, 0xB2, 0xDD, 0x06, 0x00, 0x00, 0x7C}; 


void UpdateRoofAdresses(){
  UpdateAddr(SOLAR_OUTLET_COM1, adSPO_T1);
  UpdateAddr(SOLAR_MID_COM1, adSPM_T1);
  UpdateAddr(SOLAR_INLET_COM1, adSPI_T1);
  UpdateAddr(FREE_AIR_COM1, adRFA_T1);
  UpdateAddr(EQU_BOX_COM1, adREB_T1);
  UpdateAddr(ACK_TANK_LOW_DALLAS, DallasNetwork[0].ADDR);
  UpdateAddr(SOLAR_HEATEX_OUT_DALLAS, DallasNetwork[1].ADDR);
  UpdateAddr(SOLAR_HEATEX_IN_DALLAS, DallasNetwork[2].ADDR);
  UpdateAddr(ACK_TANK_OUT_DALLAS, DallasNetwork[3].ADDR);
  UpdateAddr(HEATEX_TANK_IN_DALLAS, DallasNetwork[4].ADDR);
  UpdateAddr(HEATEX_TANK_OUT_DALLAS, DallasNetwork[5].ADDR);
  UpdateAddr(HEATEX_DOM_IN_DALLAS, DallasNetwork[6].ADDR);
  UpdateAddr(HEATEX_DOM_OUT_DALLAS, DallasNetwork[7].ADDR);
  UpdateAddr(TECH_ROOM_DALLAS, DallasNetwork[8].ADDR);
  UpdateAddr(DUMP_VALVE_DALLAS, DallasNetwork[9].ADDR);
  UpdateAddr(ACK_TANK_HEATEX_RETURN_DALLAS, DallasNetwork[10].ADDR);
//  UpdateAddr(, DallasNetwork[11].ADDR);
  UpdateAddr(ACK_TANK_MID_DALLAS, DallasNetwork[12].ADDR);
//  UpdateAddr(, DallasNetwork[13].ADDR);
//  UpdateAddr(, DallasNetwork[14].ADDR);
//  UpdateAddr(, DallasNetwork[15].ADDR);
//  UpdateAddr(, DallasNetwork[16].ADDR);
}
*/
