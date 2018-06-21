
//**************************************************************************
//
// |H1|H2|Len|      ID      | Temp | CRC |
//  EE-FF-0A-28ffc901051503d2-3354-cccc
//
//**************************************************************************

byte COM_2_incoming;
byte COM_2_data[COM_MAX_DATA_LENGTH];
int COM_2_datalen = 0;
int COM_2_frameDataLen = 0;
uint16_t COM_2_checksum = 0;

void COM_2_resetBuffer(){
  COM_2_STATE = WAITING_FOR_NEW_MESS;
  COM_2_datalen = 0;
}
bool COM_2_Available = false;
int COM_2_loopEmergencyExit; 

void COM_2_pollSerial(){
//  int bytesFromRX2 = 0;
  COM_2_loopEmergencyExit = 0;
  if(COM_2_STATE == VALID_FRAME_EXIST) COM_2_resetBuffer(); //discard unhandled message
  COM_2_Available = rx_2.available();
  while(COM_2_Available && COM_2_loopEmergencyExit < COM_MAX_DATA_LENGTH ){ //&& COM_2_STATE != VALID_MESS_EXIST ){
    COM_2_loopEmergencyExit++;
    COM_2_incoming = rx_2.read(); //bytesFromRX2++;
    COM_2_data[COM_2_datalen++] = COM_2_incoming;
    switch(COM_2_STATE){
      case WAITING_FOR_NEW_MESS:
        if(COM_2_incoming == COM_HEADER_1) 
        { COM_2_STATE = WAITING_FOR_HEADER2;         }
        else COM_2_resetBuffer();
        break; 
      case WAITING_FOR_HEADER2:
        if(COM_2_incoming == COM_HEADER_2) { COM_2_STATE = WAITING_FOR_LENGTH; }
        else COM_2_resetBuffer();
        break;
      case WAITING_FOR_LENGTH:
        COM_2_frameDataLen = COM_2_incoming; 
        COM_2_STATE = RECIEVING_DATA; 
        break; 
      case RECIEVING_DATA:
        COM_2_frameDataLen = COM_2_frameDataLen - 1;
        if(COM_2_frameDataLen == 0){ COM_2_STATE = WAITING_FOR_CHECKSUM_1; }
        break;
      case WAITING_FOR_CHECKSUM_1:
        COM_2_STATE = WAITING_FOR_CHECKSUM_2; 
        break;
      case WAITING_FOR_CHECKSUM_2:
        // TODO: Verify Fletcher-16
        COM_2_checksum = COM_2_data[COM_2_datalen-2] << 8 | COM_2_data[COM_2_datalen-1];
         if(COM_2_checksum == fletcher16(COM_2_data, COM_2_datalen-2)){
          COM_2_STATE = VALID_FRAME_EXIST; 
        //  COM2_NEW_DATA++;
          COM_2_getID();
          temp = COM_2_getTemp();
          RaspiPrint("COM_2: ");
          sendValueToRaspi(COM_2_ID_String, COM_2_getTemp());
          switch(COM_2_getData(10)){
            case ID_TV1SS:
              COM_2_TV1SS = temp;
//              RaspiPrintln("COM_2 TV1SS"); 
              break;
            case ID_TM1SS:
              COM_2_TM1SS = temp;
//              RaspiPrintln("COM_2 TM1SS"); 
              break;
            case ID_TK1SS:
              COM_2_TK1SS = temp;
//              RaspiPrintln("COM_2 TK1SS"); 
              break;
            case ID_TA:
              COM_2_TA = temp;
//              RaspiPrintln("COM_2 TA"); 
              break;
            case ID_TU:  
              COM_2_TU = temp;
//              RaspiPrintln("COM_2 TU"); 
              break;
           default:
              RaspiPrintln("COM_2 Unknown"); 
              break;
          }
          COM_2_resetBuffer();
        }
        else{
          sendErrorToRaspi("COM_2 Bad Checksum");
          COM_2_resetBuffer();
        }
        break;
    } // switch(COM_STATE) 
    COM_2_Available = rx_2.available();
  } // while(COM_2_Available)
  if(!(COM_2_loopEmergencyExit < COM_MAX_DATA_LENGTH)){
    sendErrorToRaspi("COM_2 EmergencyExit");
  } 
//  RaspiPrint("COM2 bytes read from rx2: ");
//  RaspiPrintln(bytesFromRX2);
//  if(!B2Available){
//    sendErrorToRaspi("COM_2 no data");
//  }
} // void pollSerial()

boolean COM_2_NewValidMessageExists(){
  return COM_2_STATE == VALID_FRAME_EXIST;
}

float COM_2_getTemp(){
  float temp = 0.0;
  if (COM_2_NewValidMessageExists()) {
    raw = (COM_2_data[11] << 8) | COM_2_data[12];
    temp = (float)raw / 16.0;
  }
  return temp;
}

byte COM_2_getData(int index){
  return COM_2_data[index];
}

void COM_2_getID(){
  const char * hex = "0123456789ABCDEF";
  char * pout = COM_2_ID_String;
  for(byte * pin = &COM_2_data[3]; pin < &COM_2_data[11]; pout +=3, pin++){
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
