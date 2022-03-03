int dbgPrint = 0;


void DebugPrint(){

  RaspiDebugPrint("TapWater_Temp_PT "); RaspiDebugPrintln(TapWater_Temp_PT);
  RaspiDebugPrint("SolarPanel_Out_Temp_1_PT "); RaspiDebugPrintln(SolarPanel_Out_Temp_1_PT);
  RaspiDebugPrint("SolHeatEx_Out_Pressure "); RaspiDebugPrintln(SolHeatEx_Out_Pressure);
  RaspiDebugPrint("AckTank_Out_Pressure "); RaspiDebugPrintln(AckTank_Out_Pressure);
  RaspiDebugPrint("TapWater_State "); RaspiDebugPrintln(TapWater_State);

  if(dbgPrint!= 2) return;
  dbgPrint = 0;
  RaspiPrintln("<- DEBUGINFO ->");
  RaspiPrint("     AckTank_Bot_Temp_1 : ");RaspiPrintln(AckTank_Bot_Temp_1);
  RaspiPrint("     HeatEx_TapWater_In_Temp : ");RaspiPrintln(HeatEx_TapWater_In_Temp);
  RaspiPrint("     HeatEx_TankWater_Out_Temp : ");RaspiPrintln(HeatEx_TankWater_Out_Temp);
  RaspiPrintln();
  RaspiPrint("     SolarPanel_Out_Temp_1 : ");RaspiPrintln(SolarPanel_Out_Temp_1);
  RaspiPrint("     SolarPanel_Mid_Temp : ");RaspiPrintln(SolarPanel_Mid_Temp);
  RaspiPrint("     SolarPanel_In_Temp_1 : ");RaspiPrintln(SolarPanel_In_Temp_1);
  RaspiPrint("     Roof_EqBox_Temp    : ");RaspiPrintln(Roof_EqBox_Temp);
  RaspiPrint("     Roof_Air_Temp    : ");RaspiPrintln(Roof_Air_Temp);
  RaspiPrintln();
  RaspiPrint("     Sol_CP_State    : ");RaspiPrintln(Sol_CP_State);
  RaspiPrint("     HeatEx_CP_State   : ");RaspiPrintln(HeatEx_CP_State);
  RaspiPrint("     TapWater_State_LocalVar    : ");RaspiPrintln(TapWater_State_LocalVar);
  RaspiPrint("     DumpValve_State : ");RaspiPrintln(DumpValve_State);
  RaspiPrintln();
  RaspiPrint("     SolHeatEx_Flow  : ");RaspiPrintln(SolHeatEx_Flow);
  RaspiPrint("     AckTank_HeatEx_Flow : ");RaspiPrintln(AckTank_HeatEx_Flow);
  RaspiPrint("     TapWater_Flow  : ");RaspiPrintln(TapWater_Flow);
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

