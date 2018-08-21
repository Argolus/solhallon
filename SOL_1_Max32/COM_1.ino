
//**************************************************************************
//
// |H1|H2|Len|      ID      | Temp | CRC |
//  EE-FF-10-28ffc901051503d2-3354-cccc
//
//**************************************************************************


byte COM_1_incoming;
byte COM_1_data[COM_MAX_DATA_LENGTH];
int COM_1_datalen = 0;
int COM_1_frameDataLen = 0;
uint16_t COM_1_checksum = 0;
bool COM_1_Available = false;
int COM_1_loopEmergencyExit; 

void COM_1_resetBuffer(){
  COM_1_STATE = WAITING_FOR_NEW_MESS;
  COM_1_datalen = 0;
}


void COM_1_pollSerial(){
//  int bytesFromRX1 = 0;
  COM_1_loopEmergencyExit = 0;
  if(COM_1_STATE == VALID_FRAME_EXIST) COM_1_resetBuffer(); //discard unhandled message
  COM_1_Available=rx_1.available();
  while(COM_1_Available && COM_1_loopEmergencyExit < COM_MAX_DATA_LENGTH ){ //&& COM_1_STATE != VALID_MESS_EXIST ){
    COM_1_loopEmergencyExit++;
    COM_1_incoming = rx_1.read(); //bytesFromRX1++;
//    RaspiPrint(COM_1_STATE); RaspiPrint(" : "); RaspiPrintln(COM_1_incoming);
    COM_1_data[COM_1_datalen++] = COM_1_incoming;
    switch(COM_1_STATE){
      case WAITING_FOR_NEW_MESS:
        if(COM_1_incoming == COM_HEADER_1){ 
          COM_1_STATE = WAITING_FOR_HEADER2; 
        }
        else 
          COM_1_resetBuffer();
        break; 
      case WAITING_FOR_HEADER2:
        if(COM_1_incoming == COM_HEADER_2) { 
          COM_1_STATE = WAITING_FOR_LENGTH; 
        }
        else 
          COM_1_resetBuffer();
        break; 
      case WAITING_FOR_LENGTH:
        COM_1_frameDataLen = COM_1_incoming; 
        COM_1_STATE = RECIEVING_DATA;
        break; 
      case RECIEVING_DATA:
        COM_1_frameDataLen = COM_1_frameDataLen - 1;
        if(COM_1_frameDataLen == 0){ COM_1_STATE = WAITING_FOR_CHECKSUM_1; }
        break;
      case WAITING_FOR_CHECKSUM_1:
        COM_1_STATE = WAITING_FOR_CHECKSUM_2;
        break;
      case WAITING_FOR_CHECKSUM_2:
        COM_1_checksum = COM_1_data[COM_1_datalen-2] << 8 | COM_1_data[COM_1_datalen-1];
        if(COM_1_checksum == fletcher16(COM_1_data, COM_1_datalen-2)){
          COM_1_STATE = VALID_FRAME_EXIST;
          COM_1_getID();
          temp = COM_1_getTemp();
          RaspiPrint("COM_1: ");
          sendValueToRaspi(COM_1_ID_String, temp);
 //         if(COM_1_data[10] != 0x1A && COM_1_data[10] != 0x67 && COM_1_data[10] != 0xFC){
 //           RaspiPrint("!!!! COM1 Err ");
 //           RaspiPrintln(COM_1_data[10]);
 //         }
          switch(SensorIndex(&COM_1_data[3])){
            case SOLAR_OUTLET_COM1:
              COM_1_TV1SS.setValue(temp);
//              RaspiPrint("SPO_T1 ");
//              RaspiPrintln(temp); 
              break;
            case SOLAR_MID_COM1:
              COM_1_TM1SS.setValue(temp);
//              RaspiPrintln("COM_1 TM1SS"); 
              break;
            case SOLAR_INLET_COM1:
              COM_1_TK1SS.setValue(temp);
//              RaspiPrintln("COM_1 TK1SS"); 
              break;
            case EQU_BOX_COM1:
              COM_1_TA.setValue(temp);
//              RaspiPrintln("COM_1 TA"); 
              break;
            case FREE_AIR_COM1:  
              COM_1_TU.setValue(temp);
//              RaspiPrintln("COM_1 TU"); 
              break;
            default:
              RaspiDebugPrintln("COM_1 Unknown"); 
              break;
          }
          COM_1_resetBuffer();
        }
        else{
          sendErrorToRaspi("COM_1 Bad Checksum");
          COM_1_resetBuffer();
        }
        break;
    } // switch(COM_STATE) 
    COM_1_Available = rx_1.available();
  } // while(COM_1_Available)
  if(!(COM_1_loopEmergencyExit < COM_MAX_DATA_LENGTH)){
    sendErrorToRaspi("COM_1 EmergencyExit");
  } 
//  RaspiPrint("COM1 bytes read from rx1: ");
//  RaspiPrintln(bytesFromRX1);
//  if(!B1Available){
//    sendErrorToRaspi("COM_1 no data");
//  }
} // void pollSerial()

boolean COM_1_NewValidMessageExists(){
  return COM_1_STATE == VALID_FRAME_EXIST;
}

float COM_1_getTemp(){
  float temp = 0.0;
  if (COM_1_NewValidMessageExists()) {
    raw = (COM_1_data[11] << 8) | COM_1_data[12];
    temp = (float)raw / 16.0;
  }
  return temp;
}

byte COM_1_getData(int index){
  return COM_1_data[index];
}

void COM_1_getID(){
  const char *hex = "0123456789ABCDEF";
  char * pout = COM_1_ID_String;
  for(byte * pin = &COM_1_data[3]; pin < &COM_1_data[11]; pout +=3, pin++){
        pout[0] = hex[(*pin>>4) & 0xF];
        pout[1] = hex[ *pin     & 0xF];
        pout[2] = SEPARATOR; //':';
  } 
  pout[-1] = '\0';

}
/*
void tohex(byte * in, size_t insz, char * out, size_t outsz)
{
    const char * hex = "0123456789ABCDEF";
    char * pout = out;
    for(byte * pin = in; pin < in+insz; pout +=3, pin++){
        pout[0] = hex[(*pin>>4) & 0xF];
        pout[1] = hex[ *pin     & 0xF];
        pout[2] = ':';
        if (pout + 3 - out > outsz){
            /* Better to truncate output string than overflow buffer * /
            break;
        }
    }
    pout[-1] = 0;
}
*/
