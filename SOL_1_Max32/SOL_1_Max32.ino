#include <Wire.h>
#include "solsidan.h"

const float CURRENT_VERSION = 0.75;

/************************************************************************
 *   Initializations
************************************************************************/

void setup(void) {
  pinMode(FVV, INPUT);
//  mPORTBSetPinsAnalogIn(0x0007);   // RB0,1,2 (D54 (AN0), D55 (AN1), D56 (AN2))
  pinMode(BUS, OUTPUT);
  pinMode(HeatEx_CP, OUTPUT);
  pinMode(DumpValve, OUTPUT);
  pinMode(HeatExValve, OUTPUT); 
  pinMode(Sol_CP, OUTPUT);
  pinMode(MUX_ADR_A, OUTPUT);
  pinMode(MUX_ADR_B, OUTPUT);
  pinMode(MUX_ADR_C, OUTPUT); 
  pinMode(FL_VV_IN, INPUT);
  pinMode(FL_PVV, INPUT);
  pinMode(FL_SOL, INPUT);
  Serial.begin(115200); // Till Raspberryn  
  Serial1.begin(9600); // Tak 1
  Serial2.begin(9600); // Tak 2 Arduino på taket, SolarPanel_Out_Temp_1, SolarPanel_Mid_Temp, SolarPanel_In_Temp 1, Roof_EqBox_Temp, Roof_Air_Temp
  Serial3.begin(9600); // Tak 3
  configureInterrupts();
  RaspiPrint("\n-------\nB3Labz Solhallon version ");
  RaspiPrint(CURRENT_VERSION);
  RaspiPrintln("\n-------\n");
  oldFlowConvTime = 0;
  Wire.begin(); // join i2c bus (address optional for master)
  delay(20);
  LoadSENSORS();
}

/************************************************************************
 * Main Loop 
 ************************************************************************/
void loop(void) {
  ScanOneWire();  // Uppdatera Dallas sensor
  PollSerial();   // Ta emot seriell data fr taket
  ReadLocalIO();  // Lokala I/O
  DebugPrint();   
  ConvertTemp();  
  ConvertFlow();
  DoControl();   
  
  checkRaspiComm(); //Kolla om nytt kommando fr RaspberryPI?
  delay(200);
  RUN_DT();
}

/************************************************************************
 * Local functions
************************************************************************/

void ReadLocalIO(){
  TapWater_Temp_PT = ReadAnalog(AI_PIN_G1, TapWater_Temp_PT_Address);
  SolarPanel_Out_Temp_1_PT = ReadAnalog(AI_PIN_G1, SolarPanel_Out_Temp_1_PT_Address);
  SolHeatEx_Out_Pressure = ReadAnalog(AI_PIN_G1, SolHeatEx_Out_Pressure_Address);
  AckTank_Out_Pressure = ReadAnalog(AI_PIN_G1, AckTank_Out_Pressure_Address);
  TapWater_State = digitalRead(FVV);  
}

void PollSerial(){
  memset(COM_1_ID_String, 0, SizeOfID); 
  memset(COM_2_ID_String, 0, SizeOfID); 
  memset(COM_3_ID_String, 0, SizeOfID); 
  COM_1_pollSerial();
  COM_2_pollSerial();
  COM_3_pollSerial();  
}

void ConvertTemp(void){
  SolarPanel_Out_Temp_1 = COMValue::Vote(COM_1_SolarPanel_Out_Temp_1, COM_2_SolarPanel_Out_Temp_1, COM_3_SolarPanel_Out_Temp_1, TakTolerans, false);
  SolarPanel_Mid_Temp = COMValue::Vote(COM_1_SolarPanel_Mid_Temp, COM_2_SolarPanel_Mid_Temp, COM_3_SolarPanel_Mid_Temp, TakTolerans, false);
  SolarPanel_In_Temp_1 = COMValue::Vote(COM_1_SolarPanel_In_Temp_1, COM_2_SolarPanel_In_Temp_1, COM_3_SolarPanel_In_Temp_1, TakTolerans, true);
  Roof_EqBox_Temp = COM_1_Roof_Air_Temp.Value(); //, COM_2_Roof_Air_Temp, COM_3_Roof_Air_Temp);
  Roof_Air_Temp = COM_1_Roof_EqBox_Temp.Value(); //, COM_2_Roof_EqBox_Temp, COM_3_Roof_EqBox_Temp);

//<- Debug Print ->
  RaspiDebugPrint(COM_1_SolarPanel_Out_Temp_1);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_2_SolarPanel_Out_Temp_1);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_3_SolarPanel_Out_Temp_1);  RaspiDebugPrint(" => ");  RaspiDebugPrintln(SolarPanel_Out_Temp_1);
  RaspiDebugPrint(COM_1_SolarPanel_Mid_Temp);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_2_SolarPanel_Mid_Temp);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_3_SolarPanel_Mid_Temp);  RaspiDebugPrint(" => ");  RaspiDebugPrintln(SolarPanel_Mid_Temp);
  RaspiDebugPrint(COM_1_SolarPanel_In_Temp_1);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_2_SolarPanel_In_Temp_1);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_3_SolarPanel_In_Temp_1);  RaspiDebugPrint(" => ");  RaspiDebugPrintln(SolarPanel_In_Temp_1);
  RaspiDebugPrint(COM_1_Roof_Air_Temp);  RaspiDebugPrint(" => ");  RaspiDebugPrintln(Roof_EqBox_Temp);
  RaspiDebugPrint(COM_1_Roof_EqBox_Temp);  RaspiDebugPrint(" => ");  RaspiDebugPrintln(Roof_Air_Temp);
//<- End Debug Print -> 
}

float FlowMeterCalibrationFactor = 500;  //  pulses / lit

void ConvertFlow(void){
  if((millis() - oldFlowConvTime) > 1000){
    TapWater_Flow = currentFlowCount_TapWater_Flow;   // * 60 / FlowMeterCalibrationFactor;
    AckTank_HeatEx_Flow = currentFlowCount_AckTank_HeatEx_Flow; // * 60 / FlowMeterCalibrationFactor;
    SolHeatEx_Flow = currentFlowCount_SolHeatEx_Flow;   // * 60 / FlowMeterCalibrationFactor;
    RaspiPrint("HDI_F1  "); RaspiPrintln(TapWater_Flow,2);
    RaspiPrint("ARP_F1 ");  RaspiPrintln(AckTank_HeatEx_Flow,2);
    RaspiPrint("SHO_F1  "); RaspiPrintln(SolHeatEx_Flow,2);
    currentFlowCount_TapWater_Flow = 0;
    currentFlowCount_AckTank_HeatEx_Flow = 0; 
    currentFlowCount_SolHeatEx_Flow = 0;  
    oldFlowConvTime = millis();
  }    
}

void Sol_CP_Enable(){
  digitalWrite(Sol_CP, HIGH);
  if(Sol_CP_State == OFF){ Sol_CP_State = ON; RaspiPrintln("SOL_CP ON"); }
}
void Sol_CP_Disable(){
  digitalWrite(Sol_CP, LOW);
  if(Sol_CP_State == ON){ Sol_CP_State = OFF; RaspiPrintln("SOL_CP OFF"); }
}

unsigned long lastHeatEx_CP_State_ON = 0; 
unsigned long lastHeatEx_CP_State_OFF = 0; 

void HeatEx_CP_Enable(int32_t nutid){
  HeatEx_TapWater_In_Temp = DallasTemp(HEATEX_DOM_IN_DALLAS);
  HeatEx_TankWater_Out_Temp = DallasTemp(HEATEX_TANK_OUT_DALLAS);  
//<- Debug Print ->
  RaspiDebugPrint("HeatEx_TapWater_In_Temp "); RaspiDebugPrintln(HeatEx_TapWater_In_Temp);
  RaspiDebugPrint("HeatEx_TankWater_Out_Temp "); RaspiDebugPrintln(HeatEx_TankWater_Out_Temp);
  if(HeatEx_CP_State == true)
    RaspiDebugPrintln("HeatEx_CP_State = ON");
  else
    RaspiDebugPrintln("HeatEx_CP_State = OFF");
  RaspiDebugPrint("(lastHeatEx_CP_State_OFF+CPVV_MIN_OFFTIME < nutid) =>  "); RaspiDebugPrintln(lastHeatEx_CP_State_OFF+HeatEx_CP_MIN_OFFTIME < nutid);
//<- End Debug Print ->
  if(HeatEx_TapWater_In_Temp < START_TEMP_HEATEX ){
    if(HeatEx_CP_State==OFF && 
       (lastHeatEx_CP_State_OFF + HeatEx_CP_MIN_OFFTIME) < nutid
        && HeatEx_TankWater_Out_Temp < AckTank_Bot_Temp_1 ){
      digitalWrite(DumpValve, CLOSE);  //onödigt, för den borde redan vara stängd, men men...  
      digitalWrite(HeatExValve, OPEN);
      digitalWrite(HeatEx_CP, OPEN);
      RaspiPrint("HTO_CP ON - "); RaspiPrint(HeatEx_TapWater_In_Temp); RaspiPrint(" "); 
      RaspiPrint(lastHeatEx_CP_State_OFF);   RaspiPrint(" ");   RaspiPrintln(nutid);
      HeatEx_CP_State = ON;
      lastHeatEx_CP_State_ON = nutid;
      return;
    }
    if(HeatEx_CP_State==ON && nutid > lastHeatEx_CP_State_ON + HeatEx_CP_MIN_RUNTIME){
      if(HeatEx_TankWater_Out_Temp > AckTank_Bot_Temp_1) {   //HeatEx_TapWater_In_Temp + MAX_DIFF_PVV){
        digitalWrite(DumpValve, CLOSE);
        digitalWrite(HeatExValve, CLOSE);
        digitalWrite(HeatEx_CP, CLOSE);
        lastHeatEx_CP_State_OFF = nutid;
        HeatEx_CP_State = OFF; RaspiPrintln("HeatEx_CP_State_OFF A");      
      }
    }
  }
}

void HeatEx_CP_Disable(int32_t nutid){
  digitalWrite(DumpValve, CLOSE);
  digitalWrite(HeatExValve, CLOSE);
  digitalWrite(HeatEx_CP, CLOSE);
  if(HeatEx_CP_State==ON && nutid > lastHeatEx_CP_State_ON + HeatEx_CP_MIN_RUNTIME){
    lastHeatEx_CP_State_OFF = nutid; 
    RaspiPrintln("HeatEx_CP_State OFF B");
    HeatEx_CP_State = OFF; 
  }
}

void LogTapWaterStateChange(){
  if(TapWater_State == LOW && TapWater_State_LocalVar==ON){
    RaspiPrintln("TapWater_State_LocalVar OFF");
    TapWater_State_LocalVar = OFF;
  }
  else if (TapWater_State == HIGH && TapWater_State_LocalVar==OFF){
    RaspiPrintln("TapWater_State_LocalVar ON");
    TapWater_State_LocalVar = ON;
  }
}

/************************************************************************
 * THE control function
************************************************************************/

void DoControl(void){

  AckTank_Bot_Temp_1 = DallasTemp(ACK_TANK_LOW_1_DALLAS); 
  if(SolarPanel_Out_Temp_1 > (AckTank_Bot_Temp_1 + HeatEx_CP_Temp_Diff_START)){
    Sol_CP_Enable();   // starta cirkpump
  }
  else if(SolarPanel_Out_Temp_1 < (AckTank_Bot_Temp_1 + HeatEx_CP_Temp_Diff_STOP)){
    Sol_CP_Disable();
  }


  if(AckTank_Bot_Temp_1 > DUMP_OPEN_TEMP) { 
    //Acktanken överhettad -KYL
    digitalWrite(DumpValve, OPEN); 
    digitalWrite(HeatExValve, OPEN);
    digitalWrite(HeatEx_CP, OPEN);
     if(DumpValve_State==IS_CLOSED){ RaspiPrintln("DumpValve OPEN"); DumpValve_State = IS_OPEN;     }
  }
  else if ((AckTank_Bot_Temp_1 < DUMP_CLOSE_TEMP) && DumpValve_State == IS_OPEN)  { 
    digitalWrite(DumpValve, CLOSE);
    digitalWrite(HeatExValve, CLOSE);
    digitalWrite(HeatEx_CP, CLOSE); 
    RaspiPrintln("DumpValve CLOSED"); DumpValve_State = IS_CLOSED; 
  }

  LogTapWaterStateChange();  //skicka tappvatten på/av till Raspi...

  if (DumpValve_State==IS_CLOSED) {
    if(TapWater_State == FLOW_EXISTS)
      HeatEx_CP_Enable(millis());
    else
      HeatEx_CP_Disable(millis());
  }
}
