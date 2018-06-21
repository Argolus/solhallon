
//**************************************************************************
//
// |H1|H2|Len|      ID      | Temp | CRC |
//  EE-FF-0A-28ffc901051503d2-3354-cccc
//
//**************************************************************************

byte COM_3_incoming;
byte COM_3_data[COM_MAX_DATA_LENGTH];
int COM_3_datalen = 0;
int COM_3_frameDataLen = 0;
uint16_t COM_3_checksum = 0;

void COM_3_resetBuffer(){
  COM_3_STATE = WAITING_FOR_NEW_MESS;
  COM_3_datalen = 0;
}

bool COM_3_Available = false;
int COM_3_loopEmergencyExit; 

void COM_3_pollSerial(){
//  int bytesFromRX3 = 0;
  COM_3_loopEmergencyExit = 0;
  if(COM_3_STATE == VALID_FRAME_EXIST) COM_3_resetBuffer(); //discard unhandled message
  COM_3_Available=rx_3.available();
  while(COM_3_Available && COM_3_loopEmergencyExit < COM_MAX_DATA_LENGTH ){ //&& COM_3_STATE != VALID_MESS_EXIST ){
    COM_3_loopEmergencyExit++;
    COM_3_incoming = rx_3.read(); // bytesFromRX3++;
    COM_3_data[COM_3_datalen++] = COM_3_incoming;
    switch(COM_3_STATE){
      case WAITING_FOR_NEW_MESS:
        if(COM_3_incoming == COM_HEADER_1) 
        { COM_3_STATE = WAITING_FOR_HEADER2; }
        else COM_3_resetBuffer();
        break; 
      case WAITING_FOR_HEADER2:
        if(COM_3_incoming == COM_HEADER_2) { COM_3_STATE = WAITING_FOR_LENGTH; }
        else COM_3_resetBuffer();
        break; 
      case WAITING_FOR_LENGTH:
        COM_3_frameDataLen = COM_3_incoming; 
        COM_3_STATE = RECIEVING_DATA;
        break; 
      case RECIEVING_DATA:
        COM_3_frameDataLen = COM_3_frameDataLen - 1;
        if(COM_3_frameDataLen == 0){ COM_3_STATE = WAITING_FOR_CHECKSUM_1; }
        break;
      case WAITING_FOR_CHECKSUM_1:
        COM_3_STATE = WAITING_FOR_CHECKSUM_2;
        break;
      case WAITING_FOR_CHECKSUM_2:
        // TODO: Verify Fletcher-16
        COM_3_checksum = COM_3_data[COM_3_datalen-2] << 8 | COM_3_data[COM_3_datalen-1];
        if(COM_3_checksum == fletcher16(COM_3_data, COM_3_datalen-2)){
          COM_3_STATE = VALID_FRAME_EXIST; 
          COM_3_getID();
          temp = COM_3_getTemp();
          RaspiPrint("COM_3: ");
          sendValueToRaspi(COM_3_ID_String, COM_3_getTemp());
          switch(COM_3_getData(10)){
            case ID_TV1SS:
              COM_3_TV1SS = temp;
//              RaspiPrintln("COM_3 TV1SS"); 
              break;
            case ID_TM1SS:
              COM_3_TM1SS = temp;
//              RaspiPrintln("COM_3 TM1SS"); 
              break;
            case ID_TK1SS:
              COM_3_TK1SS = temp;
//              RaspiPrintln("COM_3 TK1SS"); 
              break;
            case ID_TA:
              COM_3_TA = temp;
//              RaspiPrintln("COM_3 TA"); 
              break;
            case ID_TU:  
              COM_3_TU = temp;
//              RaspiPrintln("COM_3 TU"); 
              break;
           default:
              RaspiPrintln("COM_3 Unknown"); 
              break;
          }
          COM_3_resetBuffer();
        }
        else{
          COM_3_STATE = WAITING_FOR_NEW_MESS;
          sendErrorToRaspi("COM_3 Bad Checksum");
          COM_3_resetBuffer();
        }
        break;
    } // switch(COM_STATE) 
    COM_3_Available = rx_3.available();
  } // while(COM_3_Available)
  if(!(COM_3_loopEmergencyExit < COM_MAX_DATA_LENGTH)){
    sendErrorToRaspi("COM_3 EmergencyExit");
  } 
//  RaspiPrint("COM3 bytes read from rx3: ");
//  RaspiPrintln(bytesFromRX3);
//  if(!B0Available){
//    sendErrorToRaspi("COM_3 no data");
//  }
} // void pollSerial()

boolean COM_3_NewValidMessageExists(){
  return COM_3_STATE == VALID_FRAME_EXIST;
}

float COM_3_getTemp(){
  float temp = 0.0;
  if (COM_3_NewValidMessageExists()) {
    raw = (COM_3_getData(11) << 8) | COM_3_getData(12);
    temp = (float)raw / 16.0;
  }
  return temp;
}

byte COM_3_getData(int index){
  return COM_3_data[index];
}

void COM_3_getID(){
  const char * hex = "0123456789ABCDEF";
  char * pout = COM_3_ID_String;
  for(byte * pin = &COM_3_data[3]; pin < &COM_3_data[11]; pout +=3, pin++){
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
