#include "Pipe.h"
#include "COMValue.h"
// 
//  Konstanter och globala variabler f�r Johans solsystem
//

bool DESIGN_TEST = false;

// Dallas 1-wire adresser == Index i DallasNetwork[]
//#define _TKA   0      // Värmevatten inne i acktanken (dykr�r mellan solslinga in/ut)
//#define _SolarPanel_In_Temp_2 1      // Solslinga ut (nere) fr�n acktank (dykr�r)
//#define _SolarPanel_Out_Temp_2 2      // Solslinga in till acktank (givare p� r�ret)
//#define _TVA   3      // V�rmevatten ut (uppe) fr�n pannan p� varmvattensidan
//#define _TDI   4      // V�rmevatten in (uppe) till tappvarmvattenautomat
//#define _TDU   5      // V�rmevatten ut (nere) fr�n tappvarmvattenautomat
//#define _TVI   6      // Tappvarmvatten in (nere) till tappvarmvattenautomat
//#define _TVU   7      // Tappvarmvatten ut (uppe) fr�n tappvarmvattenautomat
//#define _TR    8      // Rumstemperatur
//#define _TVD   9      // Temp v�rmedump
//#define _TKI   10     // P� r�r in i acktank l�ngst ner till h�ger
//#define _TGR   11     // Temp i grunden
//#define _TMA   12     // Dykr�r ovanf�r solslinga i acktank
//
//// I/O pinnar
#define Sol_CP       29    // RG7    Cirkpunp SolSlinga
#define HeatEx_CP    31    // RE6    Cirkpump PlattV�rmeV�xlare
#define DumpValve    35    // RE2    Ventil VarmvattenDump
#define HeatExValve  37    // RE0    Ventil PlattV�rmeV�xlare
#define AI_PIN_G1    54    // RB0
#define AI_PIN_G2    55    // RB1
#define AI_PIN_G3    56    // RB2
#define BUS          77    // RD7    1-wire
#define FVV          75    // RC13   fl�desvakt tappvarmvatten

#define MUX_ADR_A  49    // RD11
#define MUX_ADR_B  51    // RG8
#define MUX_ADR_C  53    // RG9
#define FL_VV_IN   02    // RE8 - INT1  fl�desm�tare tappvarmvatten inkommande
#define FL_PVV     03    // RD0 - INT0  fl�desm�tare tanksida plattv�rmev�xlare
#define FL_SOL     07    // RE9 - INT2  fl�desm�tare solslinga

#define ON          true
#define OFF         false
#define OPEN        HIGH
#define CLOSE       LOW
#define IS_OPEN     true
#define IS_CLOSED   false
#define FLOW_EXISTS HIGH


//#define PCF8591 (0x90 >> 1) // I2C bus address

const char SEPARATOR = '-';
unsigned int raw;
float temp, AckTank_Bot_Temp_1, HeatEx_TankWater_Out_Temp, HeatEx_TapWater_In_Temp; //, SolarPanel_Mid_Temp, SolarPanel_In_Temp 1, Roof_EqBox_Temp, Roof_Air_Temp // , SolarPanel_Out_Temp_2, SolarPanel_In_Temp_2, , TVA, TDI, TVU, TR, TVD;
uint16_t SolarPanel_Out_Temp_1_PT;
uint16_t TapWater_Temp_PT;               // PT100 givare p� varmvattenberendare
uint16_t SolHeatEx_Out_Pressure, AckTank_Out_Pressure;
float TapWater_Flow, AckTank_HeatEx_Flow, SolHeatEx_Flow;      // fl�de f�r ink vv, tankvatten genom v�rmev�xlare, solslinga 
const int TapWater_Temp_PT_Address = 1;    // adress f�r PT100-givare TapWater_Temp_PT (LimnoTip-kort kanal 1, grupp 1)
const int SolarPanel_Out_Temp_1_PT_Address = 2;    // adress f�r PT100-givare SolarPanel_Out_Temp_1_PT (LimnoTip-kort kanal 0, grupp 1) 
const int SolHeatEx_Out_Pressure_Address  = 3;        // adress f�r tryckgivare solslinga
const int AckTank_Out_Pressure_Address = 4;       // adress f�r tryckgivare acktank
 
//----------------------------------------
// Temperaturkonstanter
//----------------------------------------
const float HeatEx_CP_Temp_Diff_START = 3.0;
const float HeatEx_CP_Temp_Diff_STOP  = 1.0;
const float DUMP_OPEN_TEMP = 88.0;
const float DUMP_CLOSE_TEMP = 83.0;
//const float MAX_DIFF_PVV = 3.0;   // Max till�ten tempdiff mellan kallsida plattv�rmev�xlare och inkommande kallvatten
const float START_TEMP_HEATEX = 18.0;  // Starta cirkpump om inkommande kallvatten kallare 

unsigned long HeatEx_CP_MIN_RUNTIME = 4000; // Minimal g�ngtid f�r HeatEx_CP (enhet ms)
unsigned long HeatEx_CP_MIN_OFFTIME = 4000; // Minimal stopptid f�r HeatEx_CP (enhet ms)
float TakTolerans = 2.0;

boolean TapWater_State_LocalVar = OFF;        
boolean Sol_CP_State = OFF;
boolean DumpValve_State = false;
boolean HeatEx_CP_State = OFF;
int TapWater_State = LOW;

volatile int FlowCount_TapWater_Flow; // FlowCount_TapWater_Flow;
volatile unsigned long FlowCount_AckTank_HeatEx_Flow; // FlowCount_AckTank_HeatEx_Flow;
volatile unsigned long FlowCount_SolHeatEx_Flow; // FlowCount_SolHeatEx_Flow;
unsigned long oldFlowConvTime;
volatile int currentFlowCount_TapWater_Flow; //currentFlowCount_TapWater_Flow;
volatile unsigned long currentFlowCount_AckTank_HeatEx_Flow; //currentFlowCount_AckTank_HeatEx_Flow;
volatile unsigned long currentFlowCount_SolHeatEx_Flow; //currentFlowCount_SolHeatEx_Flow;

//----------------------------------------
// Kommunikation med taket
//----------------------------------------

COMValue COM_1_SolarPanel_Out_Temp_1, COM_1_SolarPanel_Mid_Temp, COM_1_SolarPanel_In_Temp_1, COM_1_Roof_Air_Temp, COM_1_Roof_EqBox_Temp;
COMValue COM_2_SolarPanel_Out_Temp_1, COM_2_SolarPanel_Mid_Temp, COM_2_SolarPanel_In_Temp_1; //, COM_2_Roof_Air_Temp, COM_2_Roof_EqBox_Temp;
COMValue COM_3_SolarPanel_Out_Temp_1, COM_3_SolarPanel_Mid_Temp, COM_3_SolarPanel_In_Temp_1; //, COM_3_Roof_Air_Temp, COM_3_Roof_EqBox_Temp;
float SolarPanel_Out_Temp_1, SolarPanel_Mid_Temp, SolarPanel_In_Temp_1, Roof_EqBox_Temp, Roof_Air_Temp;

const byte ID_SolarPanel_Out_Temp_1 = 0x08;  // Seriellt - Solslinga varmsida, ut fr�n solf�ngare
const byte ID_SolarPanel_Mid_Temp = 0xE8;  // Seriellt - Solslinga mellan solf�ngarna
const byte ID_SolarPanel_In_Temp_1 = 0x57;  // Seriellt - Solslinga kallsida in till solf�ngare
const byte ID_Roof_Air_Temp = 0x5A;     // Seriellt - Utetemp p� taket
const byte ID_Roof_EqBox_Temp = 0x7C;     // Seriellt - Temp i Arduino-dosan p� taket

#define SizeOfID 30
char COM_1_ID_String[SizeOfID];
char COM_2_ID_String[SizeOfID];
char COM_3_ID_String[SizeOfID];
const int WAITING_FOR_NEW_MESS = 1; //First byte is 0xEE 
const int WAITING_FOR_HEADER2 = 2;       //Second byte is 0xFF
const int WAITING_FOR_LENGTH = 3;   //Third byte is datalength
const int RECIEVING_DATA = 4; 
const int WAITING_FOR_CHECKSUM_1 = 5; //Should be Fletcher-16
const int WAITING_FOR_CHECKSUM_2 = 6; //Should be Fletcher-16
const int VALID_FRAME_EXIST = 100;
const byte COM_HEADER_1 = 0xEE;
const byte COM_HEADER_2 = 0xFF;
const int COM_MAX_DATA_LENGTH = 256;
int COM_0_STATE = WAITING_FOR_NEW_MESS;
int COM_1_STATE = WAITING_FOR_NEW_MESS;
int COM_2_STATE = WAITING_FOR_NEW_MESS;
int COM_3_STATE = WAITING_FOR_NEW_MESS;


/*  Serial port 0 uses UART1 (aka UART1A)
 *  Serial port 1 uses UART4 (aka UART1B)
 *  Serial port 2 uses UART2 (aka UART3A)
 *  Serial port 3 uses UART5 (aka UART3B)  */

#define TXSIZE 1024
#define RXSIZE 256    // bör vara samma som COM_MAX_DATA_LENGTH  

char tx_0_Buffer[TXSIZE];
char tx_1_Buffer[TXSIZE];
char tx_2_Buffer[TXSIZE];
char tx_3_Buffer[TXSIZE];

char rx_0_Buffer[RXSIZE];
char rx_1_Buffer[RXSIZE];
char rx_2_Buffer[RXSIZE];
char rx_3_Buffer[RXSIZE];

Pipe tx_0(TXSIZE,true,tx_0_Buffer);   // Raspberry
Pipe tx_1(TXSIZE,true,tx_1_Buffer);   // NA
Pipe tx_2(TXSIZE,true,tx_2_Buffer);   // NA
Pipe tx_3(TXSIZE,true,tx_3_Buffer);   // NA

Pipe rx_0(RXSIZE,false,rx_0_Buffer);   // (Raspberry)
Pipe rx_1(RXSIZE,false,rx_1_Buffer);   // 
Pipe rx_2(RXSIZE,false,rx_2_Buffer);   //  
Pipe rx_3(RXSIZE,false,rx_3_Buffer);   // 

volatile bool tx_0_Running = false;
volatile bool tx_1_Running = false;
volatile bool tx_2_Running = false;
volatile bool tx_3_Running = false;

typedef enum {
  SOLAR_OUTLET_COM1 = 0, 
  SOLAR_OUTLET_COM2 = 1, 
  SOLAR_OUTLET_COM3 = 2, 
  SOLAR_MID_COM1 = 3, 
  SOLAR_MID_COM2 = 4, 
  SOLAR_MID_COM3 = 5, 
  SOLAR_INLET_COM1 = 6, 
  SOLAR_INLET_COM2 = 7, 
  SOLAR_INLET_COM3 = 8,
  EQU_BOX_COM1 = 9,
  FREE_AIR_COM1 = 10,
  SOLAR_EXP_TANK_DALLAS = 11,
  SOLAR_PUMP_ASS_DALLAS = 12,
  SOLAR_HEATEX_OUT_DALLAS = 13,
  SOLAR_HEATEX_IN_DALLAS = 14,
  SOLAR_SECURITY_TANK_DALLAS = 15, 
  ACK_TANK_MID_1_DALLAS = 16,
  ACK_TANK_MID_2_DALLAS = 17,
  ACK_TANK_MID_3_DALLAS = 18,
  ACK_TANK_LOW_1_DALLAS = 19,
  ACK_TANK_LOW_2_DALLAS = 20,
  ACK_TANK_LOW_3_DALLAS = 21,
  ACK_TANK_HIGH_1_DALLAS = 22,
  ACK_TANK_HIGH_2_DALLAS = 23,
  ACK_TANK_HIGH_3_DALLAS = 24,
  ACK_TANK_HEATEX_RETURN_DALLAS = 25,
  ACK_TANK_OUT_DALLAS = 26,
  HEATEX_TANK_OUT_DALLAS = 27,
  HEATEX_TANK_IN_DALLAS = 28,
  HEATEX_DOM_IN_DALLAS = 29,
  HEATEX_DOM_OUT_DALLAS = 30,
  DUMP_VALVE_DALLAS = 31,
  DOM_WATER_HEATER_DALLAS = 32,
  TECH_ROOM_DALLAS = 33,
  NR_OF_SENSORS = 34
}teValIndex;

const int sensorIDLen = 8;
struct t_Value{
  byte ADDR[sensorIDLen];
//  teValIndex Index;
};

t_Value SENSORS[NR_OF_SENSORS];

