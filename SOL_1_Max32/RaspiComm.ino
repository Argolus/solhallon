
void RaspiPrint(String str){
    tx_0.print(str);  
}
void RaspiDebugPrint(String str){
  if(dbgPrint == 1)
    tx_0.print(str);  
}
void RaspiPrint(float value){
    tx_0.print(value);
}
void RaspiDebugPrint(float value){
  if(dbgPrint == 1)
    tx_0.print(value);
}
void RaspiPrint(float value, int frac){
    tx_0.print(value,frac);
}
void RaspiDebugPrint(float value, int frac){
  if(dbgPrint == 1)
    tx_0.print(value,frac);
}
void RaspiPrint(uint16_t value){
    tx_0.print(value);
}
void RaspiDebugPrint(uint16_t value){
  if(dbgPrint == 1)
    tx_0.print(value);
}
void RaspiPrint(byte value){
    tx_0.print(value);
}
void RaspiDebugPrint(byte value){
  if(dbgPrint == 1)
    tx_0.print(value);
}
void RaspiPrint(int value){
    tx_0.print(value);
}
void RaspiDebugPrint(int value){
  if(dbgPrint == 1)
    tx_0.print(value);
}
void RaspiPrint(unsigned long value){
    tx_0.print(value);
}
void RaspiDebugPrint(unsigned long value){
  if(dbgPrint == 1)
    tx_0.print(value);
}
void RaspiPrint(int32_t value){
    tx_0.print(value);
}
void RaspiDebugPrint(int32_t value){
  if(dbgPrint == 1)
    tx_0.print(value);
}
void RaspiDebugPrint(COMValue& obj){
  if(dbgPrint == 1){
    tx_0.print(obj.Value());
    tx_0.print(" ** ");
    tx_0.print(obj.TimeStamp());
  }
}

void RaspiPrintln(){
    tx_0.println();
}
void RaspiDebugPrintln(){
  if(dbgPrint == 1)
    tx_0.println();
}
void RaspiPrintln(String str){
    tx_0.println(str);
}
void RaspiDebugPrintln(String str){
  if(dbgPrint == 1)
    tx_0.println(str);
}
void RaspiPrintln(float value){
    tx_0.println(value);
}
void RaspiDebugPrintln(float value){
  if(dbgPrint == 1)
    tx_0.println(value);
}
void RaspiPrintln(float value, int frac){
    tx_0.println(value,frac);
}
void RaspiDebugPrintln(float value, int frac){
  if(dbgPrint == 1)
    tx_0.println(value,frac);
}
void RaspiPrintln(uint16_t value){
    tx_0.println(value);
}
void RaspiDebugPrintln(uint16_t value){
  if(dbgPrint == 1)
    tx_0.println(value);
}
void RaspiPrintln(byte value){
    tx_0.println(value);
}
void RaspiDebugPrintln(byte value){
  if(dbgPrint == 1)
    tx_0.println(value);
}
void RaspiPrintln(int value){
    tx_0.println(value);
}
void RaspiDebugPrintln(int value){
  if(dbgPrint == 1)
    tx_0.println(value);
}
void RaspiPrintln(unsigned long value){
    tx_0.println(value);
}
void RaspiDebugPrintln(unsigned long value){
  if(dbgPrint == 1)
    tx_0.println(value);
}
void RaspiPrintln(int32_t value){
    tx_0.println(value);
}
void RaspiDebugPrintln(int32_t value){
  if(dbgPrint == 1)
    tx_0.println(value);
}
void RaspiDebugPrintln(COMValue& obj){
  if(dbgPrint == 1){
    tx_0.print(obj.Value());
    tx_0.print(" ** ");
    tx_0.println(obj.TimeStamp());
  }
}

void sendValueToRaspi(String sensorID, float temp){
  RaspiPrint(sensorID); RaspiPrint("  "); RaspiPrintln(temp);
}

void sendErrorToRaspi(String Message){
  RaspiPrint("Error: "); RaspiPrintln(Message);
}


byte COM_0_incoming;
byte COM_0_data[COM_MAX_DATA_LENGTH];
int COM_0_datalen = 0;
int COM_0_frameDataLen = 0;
uint16_t COM_0_checksum = 0;
bool COM_0_Available = false;
int COM_0_loopEmergencyExit; 

void COM_0_resetBuffer(){
  COM_0_STATE = WAITING_FOR_NEW_MESS;
  COM_0_datalen = 0;
}
#define COMMAND COM_0_data[3]
#define DATALENGTH COM_0_data[2]
#define PRINTTYPE COM_0_data[4]
#define COM_SENSOR_INDEX COM_0_data[4]
#define COM_SENSOR_ADDRESS &COM_0_data[5]
#define PRINT 1
#define NUMBER_OF_PRINTTYPES 3
#define RASPI 1
#define DEBUG 2
#define SET_NEW_SENSOR 2
#define RUN_DESIGN_TEST 3
#define RESET_ALL_SENSOR_ID 4
#define STORE_SENSORS_TO_E2 5

void checkRaspiComm(){
//**************************************************************************
// Frame layout:       | H1 | H2 |Len |Cmd | data | CRC     |
// Ex DebugPrint:       0xEE 0xFF 0x02 0x01  0x01  0xB4 0xF3  (RaspiDebugPrint)
// Ex DebugPrint:       0xEE 0xFF 0x02 0x01  0x02  0xcc 0xcc  (DebugPrint)
// Ex Set SensorID:     0xEE 0xFF 0x0A 0x02  DATA  0xcc 0xcc
// Ex Design test:      0xEE 0xFF 0x01 0x03  --    0xC0 0xF2  (run design tests)
// Ex Reset SensorIDs:  0xEE 0xFF 0x01 0x04  --    0xC1 0xF3
// Ex Store SensorIDs:  0xEE 0xFF 0x01 0x05  --    0xC2 0xF4
//
// Set Sensor ID frame layout
// |teValIndex|   Sensor ID                  
//    0x01       0x28 0xB7 0x20 0x1F 0x00 0x00 0x80 0x4A  
//
//**************************************************************************
//  RaspiDebugPrintln("checkRaspiComm");
  COM_0_loopEmergencyExit = 0;
  if(COM_0_STATE == VALID_FRAME_EXIST) COM_0_resetBuffer(); //discard unhandled message
  COM_0_Available=rx_0.available();
  while(COM_0_Available && COM_0_loopEmergencyExit < COM_MAX_DATA_LENGTH ){ 
    COM_0_loopEmergencyExit++;
    COM_0_incoming = rx_0.read(); //bytesFromRX1++;
//    RaspiDebugPrint(COM_0_STATE); RaspiDebugPrint(" : "); RaspiDebugPrintln(COM_0_incoming);
    COM_0_data[COM_0_datalen++] = COM_0_incoming;
    switch(COM_0_STATE){
      case WAITING_FOR_NEW_MESS:
        if(COM_0_incoming == COM_HEADER_1){ 
          COM_0_STATE = WAITING_FOR_HEADER2; 
        }
        else 
          COM_0_resetBuffer();
        break; 
      case WAITING_FOR_HEADER2:
        if(COM_0_incoming == COM_HEADER_2) { 
          COM_0_STATE = WAITING_FOR_LENGTH; 
        }
        else 
          COM_0_resetBuffer();
        break; 
      case WAITING_FOR_LENGTH:
        COM_0_frameDataLen = COM_0_incoming; 
        COM_0_STATE = RECIEVING_DATA;
        break; 
      case RECIEVING_DATA:
        COM_0_frameDataLen = COM_0_frameDataLen - 1;
        if(COM_0_frameDataLen == 0){ COM_0_STATE = WAITING_FOR_CHECKSUM_1; }
        break;
      case WAITING_FOR_CHECKSUM_1:
        COM_0_STATE = WAITING_FOR_CHECKSUM_2;
        break;
      case WAITING_FOR_CHECKSUM_2:
        COM_0_checksum = COM_0_data[COM_0_datalen-2] << 8 | COM_0_data[COM_0_datalen-1];
        if(COM_0_checksum == fletcher16(COM_0_data, COM_0_datalen-2)){
          COM_0_STATE = VALID_FRAME_EXIST;
          switch(COMMAND){
            case PRINT:
              // recieved print command
              if(PRINTTYPE < NUMBER_OF_PRINTTYPES ){
                dbgPrint = PRINTTYPE;
              }
              break;
            case SET_NEW_SENSOR:
              //recieved new Dallas Sensor ID
              if(DATALENGTH==0x0A){
                if(COM_SENSOR_INDEX < NR_OF_SENSORS){
                  UpdateSensorAddress((teValIndex)COM_SENSOR_INDEX, COM_SENSOR_ADDRESS);
//                  for(int i=0;i<8;i++)
//                    DallasNetwork[DALLASINDEX].ADDR[i]=COM_0_data[5+i];
                }
                else{
                  RaspiDebugPrint("COM_0 Set Dallas ID bad index: ");
                  RaspiDebugPrintln(COM_SENSOR_INDEX);
                }
              }
              else{
                RaspiDebugPrint("COM_0 Set Dallas ID bad length: ");
                RaspiDebugPrintln(DATALENGTH);
              }
              break;
            case RUN_DESIGN_TEST:
              DESIGN_TEST = true;
              break;
            case RESET_ALL_SENSOR_ID:
              ResetAllSensorIDs();
              break;
            case STORE_SENSORS_TO_E2:
              StoreSENSORS();
              break;
            default:
              break;
          }
        }
        else{
          sendErrorToRaspi("COM_0 Bad Checksum");
          RaspiPrint("Expected: ");
          RaspiPrintln(fletcher16(COM_0_data, COM_0_datalen-2));
          RaspiPrint("Recieved: ");
          RaspiPrintln(COM_0_checksum);      
        }
        COM_0_resetBuffer();
        break;
    } // switch(COM_STATE) 
    COM_0_Available = rx_0.available();
  } // while(COM_0_Available)
  if(!(COM_0_loopEmergencyExit < COM_MAX_DATA_LENGTH)){
    sendErrorToRaspi("COM_0 EmergencyExit");
  } 

}

