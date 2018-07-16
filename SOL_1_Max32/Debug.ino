int dbgPrint = 0;


void DebugPrint(){

  RaspiDebugPrint("TVVB_PT "); RaspiDebugPrintln(TVVB_PT);
  RaspiDebugPrint("TV1SS_PT "); RaspiDebugPrintln(TV1SS_PT);
  RaspiDebugPrint("PSS "); RaspiDebugPrintln(PSS);
  RaspiDebugPrint("PTANK "); RaspiDebugPrintln(PTANK);
  RaspiDebugPrint("iFVV "); RaspiDebugPrintln(iFVV);

  if(dbgPrint!= 2) return;
  dbgPrint = 0;
  RaspiPrintln("<- DEBUGINFO ->");
  RaspiPrint("     TKA_T : ");RaspiPrintln(TKA_T);
  RaspiPrint("     TVI_T : ");RaspiPrintln(TVI_T);
  RaspiPrint("     TDU_T : ");RaspiPrintln(TDU_T);
  RaspiPrintln();
  RaspiPrint("     TV1SS : ");RaspiPrintln(TV1SS);
  RaspiPrint("     TM1SS : ");RaspiPrintln(TM1SS);
  RaspiPrint("     TK1SS : ");RaspiPrintln(TK1SS);
  RaspiPrint("     TU    : ");RaspiPrintln(TU);
  RaspiPrint("     TA    : ");RaspiPrintln(TA);
  RaspiPrintln();
  RaspiPrint("     CSS_ON    : ");RaspiPrintln(CSS_ON);
  RaspiPrint("     CPVV_ON   : ");RaspiPrintln(CPVV_ON);
  RaspiPrint("     FFV_ON    : ");RaspiPrintln(FFV_ON);
  RaspiPrint("     DUMP_OPEN : ");RaspiPrintln(DUMP_OPEN);
  RaspiPrintln();
  RaspiPrint("     FSOL  : ");RaspiPrintln(FSOL);
  RaspiPrint("     FTANK : ");RaspiPrintln(FTANK);
  RaspiPrint("     FVVB  : ");RaspiPrintln(FVVB);
  RaspiPrintln();
  RaspiPrint("     COM_0_STATE  : ");RaspiPrintln(ComStateToString(COM_0_STATE));
  RaspiPrint("     COM_1_STATE  : ");RaspiPrintln(ComStateToString(COM_1_STATE));
  RaspiPrint("     COM_2_STATE  : ");RaspiPrintln(ComStateToString(COM_2_STATE));
  RaspiPrint("     COM_3_STATE  : ");RaspiPrintln(ComStateToString(COM_3_STATE));
  RaspiPrintln();
  RaspiPrintln(" ----    Dallas Network    ------ ");
  RaspiPrintln(" - Index  -  ID   -   -   -  -  - ");
  for(int i=0; i<SIZE_OF_DALLAS_NETWORK; i++){
    RaspiPrint("   "); RaspiPrint(i); RaspiPrint("     ");
    RaspiPrintln(DallasSensorID(DallasNetwork[i].Sensor));
  }
  RaspiPrintln("<- END DEBUGINFO ->");   
   
}

String ComStateToString(int state){
  switch(state){
    case WAITING_FOR_NEW_MESS:
      return("WAITING_FOR_NEW_MESS");
    case WAITING_FOR_HEADER2:
      return("WAITING_FOR_HEADER2");
    case WAITING_FOR_LENGTH:
      return("WAITING_FOR_LENGTH");
    case RECIEVING_DATA:
      return("RECIEVING_DATA");
    case WAITING_FOR_CHECKSUM_1:
      return("WAITING_FOR_CHECKSUM_1");
    case WAITING_FOR_CHECKSUM_2:
      return("WAITING_FOR_CHECKSUM_2");
    case VALID_FRAME_EXIST:
      return("VALID_FRAME_EXIST");
  } 
}

