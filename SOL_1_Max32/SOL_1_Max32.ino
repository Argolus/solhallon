#include <Wire.h>
#include "solsidan.h"

const float CURRENT_VERSION = 0.7;

/************************************************************************
 *   Initializations
************************************************************************/

void setup(void) {
  pinMode(FVV, INPUT);
//  mPORTBSetPinsAnalogIn(0x0007);   // RB0,1,2 (D54 (AN0), D55 (AN1), D56 (AN2))
  pinMode(BUS, OUTPUT);
  pinMode(CPVV, OUTPUT);
  pinMode(VVD, OUTPUT);
  pinMode(VPVV, OUTPUT); 
  pinMode(CSS, OUTPUT);
  pinMode(MUX_ADR_A, OUTPUT);
  pinMode(MUX_ADR_B, OUTPUT);
  pinMode(MUX_ADR_C, OUTPUT); 
  pinMode(FL_VV_IN, INPUT);
  pinMode(FL_PVV, INPUT);
  pinMode(FL_SOL, INPUT);
  Serial.begin(38400); // Till Raspberryn  
  Serial1.begin(9600); // Tak 1
  Serial3.begin(9600); // Tak 2 Arduino på taket, TV1SS, TM1SS, TK1SS, TU, TA
  Serial2.begin(9600); // Tak 3
  configureInterrupts();
  RaspiPrint("\n-------\nJohans Solsystem version ");
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
  delay(500);
  RUN_DT();
}

/************************************************************************
 * Local functions
************************************************************************/

void ReadLocalIO(){
  TVVB_PT = ReadAnalog(AI_PIN_G1, TVVB_PT_CH);
  TV1SS_PT = ReadAnalog(AI_PIN_G1, TV1SS_PT_CH);
  PSS = ReadAnalog(AI_PIN_G1, PSS_CH);
  PTANK = ReadAnalog(AI_PIN_G1, PTANK_CH);
  iFVV = digitalRead(FVV);  
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
  TV1SS = COMValue::Vote(COM_1_TV1SS, COM_2_TV1SS, COM_3_TV1SS, TakTolerans, false);
  TM1SS = COMValue::Vote(COM_1_TM1SS, COM_2_TM1SS, COM_3_TM1SS, TakTolerans, false);
  TK1SS = COMValue::Vote(COM_1_TK1SS, COM_2_TK1SS, COM_3_TK1SS, TakTolerans, true);
  TU = COM_1_TU.Value(); //, COM_2_TU, COM_3_TU);
  TA = COM_1_TA.Value(); //, COM_2_TA, COM_3_TA);

//<- Debug Print ->
  RaspiDebugPrint(COM_1_TV1SS);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_2_TV1SS);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_3_TV1SS);  RaspiDebugPrint(" => ");  RaspiDebugPrintln(TV1SS);
  RaspiDebugPrint(COM_1_TM1SS);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_2_TM1SS);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_3_TM1SS);  RaspiDebugPrint(" => ");  RaspiDebugPrintln(TM1SS);
  RaspiDebugPrint(COM_1_TK1SS);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_2_TK1SS);  RaspiDebugPrint(" :: ");  RaspiDebugPrint(COM_3_TK1SS);  RaspiDebugPrint(" => ");  RaspiDebugPrintln(TK1SS);
  RaspiDebugPrint(COM_1_TU);  RaspiDebugPrint(" => ");  RaspiDebugPrintln(TU);
  RaspiDebugPrint(COM_1_TA);  RaspiDebugPrint(" => ");  RaspiDebugPrintln(TA);
//<- End Debug Print -> 
}

float FlowMeterCalibrationFactor = 500;  //  pulses / lit

void ConvertFlow(void){
  if((millis() - oldFlowConvTime) > 1000){
    FVVB = currentFlowCount_FVVB;   // * 60 / FlowMeterCalibrationFactor;
    FTANK = currentFlowCount_FTANK; // * 60 / FlowMeterCalibrationFactor;
    FSOL = currentFlowCount_FSOL;   // * 60 / FlowMeterCalibrationFactor;
    RaspiPrint("HDI_F1  "); RaspiPrintln(FVVB,2);
    RaspiPrint("ARP_F1 ");  RaspiPrintln(FTANK,2);
    RaspiPrint("SHO_F1  "); RaspiPrintln(FSOL,2);
    currentFlowCount_FVVB = 0;
    currentFlowCount_FTANK = 0; 
    currentFlowCount_FSOL = 0;  
    oldFlowConvTime = millis();
  }    
}


void DoControl(void){
  TKA_T = DallasTemp(ACK_TANK_LOW_1_DALLAS); 
  if(TV1SS > (TKA_T + SS_DIFF_START)){
    digitalWrite(CSS, HIGH);
    if(!CSS_ON){ CSS_ON = true; RaspiPrintln("SPA_CP ON"); }
  }
  else if(TV1SS < (TKA_T + SS_DIFF_STOP)){
    digitalWrite(CSS, LOW);
    if(CSS_ON){ CSS_ON = false; RaspiPrintln("SPA_CP OFF"); }
  }
/*  if(COM2_NEW_DATA < 1)
  {
    // ingen data från taket - NÖDKYL!
    digitalWrite(CSS, LOW);
    CSS_ON = false; Serial2.println("SPA_CP OFF - EMERGENCY"); 
    
  } 
  */
  if(TKA_T > VV_DUMP_OPEN) { 
    digitalWrite(VVD, HIGH); 
    digitalWrite(VPVV, HIGH);
    digitalWrite(CPVV, HIGH);
     if(!DUMP_OPEN){ RaspiPrintln("HDO_V1 OPEN"); DUMP_OPEN = true;     }
  }
  else if ((TKA_T < VV_DUMP_CLOSE) && DUMP_OPEN)  { 
    digitalWrite(VVD, LOW);
    digitalWrite(VPVV, LOW);
    digitalWrite(CPVV, LOW); 
    RaspiPrintln("HDO_V1 CLOSED"); DUMP_OPEN = false;  // ->|
  }
  if(iFVV == LOW && FFV_ON){
    RaspiPrintln("HDI_S1 OFF");
    FFV_ON = false;
  }
  else if (iFVV == HIGH && !FFV_ON){
    RaspiPrintln("HDI_S1 ON");
    FFV_ON = true;
  }
  if (!DUMP_OPEN) {
   CPVV_Enable( iFVV, millis());
  }
}

unsigned long lastCPVV_ON; 
unsigned long lastCPVV_OFF; 

void CPVV_Enable(boolean ON, int32_t nutid){
  if(ON){
    TVI_T = DallasTemp(HEATEX_DOM_IN_DALLAS);
    TDU_T = DallasTemp(HEATEX_TANK_OUT_DALLAS);
    
//<- Debug Print ->
    RaspiDebugPrint("TVI_T "); RaspiDebugPrintln(TVI_T);
    RaspiDebugPrint("TDU_T "); RaspiDebugPrintln(TDU_T);
    if(CPVV_ON == true)
      RaspiDebugPrintln("CPVV_ON = true");
    else
      RaspiDebugPrintln("CPVV_ON = false");
    RaspiDebugPrint("lastCPVV_OFF+CPVV_MIN_OFFTIME < nutid == "); RaspiDebugPrintln(lastCPVV_OFF+CPVV_MIN_OFFTIME < nutid);
//<- End Debug Print ->

    if(TVI_T < START_TEMP_PVV ){
      if(!CPVV_ON && 
         (lastCPVV_OFF+CPVV_MIN_OFFTIME < nutid)
          && !(TDU_T > TKA_T) ){
        digitalWrite(VVD, LOW);  //onödigt, för den borde redan vara stängd, men men...  
        digitalWrite(VPVV, HIGH);
        digitalWrite(CPVV, HIGH);
        RaspiPrint("HTO_CP ON - "); RaspiPrint(TVI_T); RaspiPrint(" "); 
        RaspiPrint(lastCPVV_OFF);   RaspiPrint(" ");   RaspiPrintln(nutid);
        CPVV_ON = true;
        lastCPVV_ON = nutid;
        return;
      }
      if(CPVV_ON && nutid > lastCPVV_ON + CPVV_MIN_RUNTIME){
        if(TDU_T > TKA_T) {   //TVI_T + MAX_DIFF_PVV){
          digitalWrite(VVD, LOW);
          digitalWrite(VPVV, LOW);
          digitalWrite(CPVV, LOW);
          lastCPVV_OFF = nutid;
          CPVV_ON = false; RaspiPrintln("HTO_CP OFF A");      
        }
      }
    }
  }
  else{
    digitalWrite(VVD, LOW);
    digitalWrite(VPVV, LOW);
    digitalWrite(CPVV, LOW);
    if(CPVV_ON && nutid > lastCPVV_ON + CPVV_MIN_RUNTIME){

      lastCPVV_OFF = nutid; 
      RaspiPrintln("HTO_CP OFF B");
      CPVV_ON = false; 
    }
  }
}


