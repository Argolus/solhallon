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
  RaspiPrint("     ATL_T1 : ");RaspiPrintln(TKA_T);
  RaspiPrint("     HDI_T1 : ");RaspiPrintln(TVI_T);
  RaspiPrint("     HTO_T1 : ");RaspiPrintln(TDU_T);
  RaspiPrintln();
  RaspiPrint("     SPO : ");RaspiPrintln(TV1SS);
  RaspiPrint("     SPM : ");RaspiPrintln(TM1SS);
  RaspiPrint("     SPI : ");RaspiPrintln(TK1SS);
  RaspiPrint("     RFA : ");RaspiPrintln(TU);
  RaspiPrint("     REB : ");RaspiPrintln(TA);
  RaspiPrintln();
  RaspiPrint("     SPO_T1 : ");RaspiPrintln(COM_1_TV1SS);
  RaspiPrint("     SPO_T2 : ");RaspiPrintln(COM_2_TV1SS);
  RaspiPrint("     SPO_T3 : ");RaspiPrintln(COM_3_TV1SS);
  RaspiPrint("     SPM_T1 : ");RaspiPrintln(COM_1_TM1SS);
  RaspiPrint("     SPM_T2 : ");RaspiPrintln(COM_2_TM1SS);
  RaspiPrint("     SPM_T3 : ");RaspiPrintln(COM_3_TM1SS);
  RaspiPrint("     SPI_T1 : ");RaspiPrintln(COM_1_TK1SS);
  RaspiPrint("     SPI_T2 : ");RaspiPrintln(COM_2_TK1SS);
  RaspiPrint("     SPI_T3 : ");RaspiPrintln(COM_3_TK1SS);
  RaspiPrintln();
  RaspiPrint("     SPA_CP    : ");RaspiPrintln(CSS_ON);
  RaspiPrint("     HTO_CP    : ");RaspiPrintln(CPVV_ON);
  RaspiPrint("     HDI_S1    : ");RaspiPrintln(FFV_ON);
  RaspiPrint("     HDI_S1    : ");RaspiPrintln(DUMP_OPEN);
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
  RaspiPrintln(" - Index  -  ID   -   -   -  -  -   Temp");
  for(int i=0; i<SIZE_OF_DALLAS_NETWORK; i++){
    RaspiPrint("   "); RaspiPrint(i); RaspiPrint("     ");
    RaspiPrint(DallasSensorID(DallasNetwork[i].Sensor));
    RaspiPrint("   ");
    RaspiPrintln(DallasNetwork[i].temp);
  }
  RaspiPrintln(" ----        SENSORS       ------ ");
  RaspiPrintln(" - Index  -  ID   -   -   -  -  - ");
  for(int i=0; i<NR_OF_SENSORS; i++){
    RaspiPrint("   "); RaspiPrint(i); RaspiPrint("     ");
    RaspiPrintln(DallasSensorID((teValIndex)i));
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

