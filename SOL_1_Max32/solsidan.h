#include <Pipe.h>
#include <COMValue.h>
// 
//  Konstanter och globala variabler f�r Johans solsystem
//

bool DESIGN_TEST = false;

// Dallas 1-wire adresser == Index i DallasNetwork[]
#define _TKA   0      // Värmevatten inne i acktanken (dykr�r mellan solslinga in/ut)
//#define _TK2SS 1      // Solslinga ut (nere) fr�n acktank (dykr�r)
//#define _TV2SS 2      // Solslinga in till acktank (givare p� r�ret)
#define _TVA   3      // V�rmevatten ut (uppe) fr�n pannan p� varmvattensidan
//#define _TDI   4      // V�rmevatten in (uppe) till tappvarmvattenautomat
#define _TDU   5      // V�rmevatten ut (nere) fr�n tappvarmvattenautomat
#define _TVI   6      // Tappvarmvatten in (nere) till tappvarmvattenautomat
//#define _TVU   7      // Tappvarmvatten ut (uppe) fr�n tappvarmvattenautomat
//#define _TR    8      // Rumstemperatur
//#define _TVD   9      // Temp v�rmedump
//#define _TKI   10     // P� r�r in i acktank l�ngst ner till h�ger
//#define _TGR   11     // Temp i grunden
//#define _TMA   12     // Dykr�r ovanf�r solslinga i acktank
//
//// I/O pinnar
#define CSS        29    // RG7    Cirkpunp SolSlinga
#define CPVV       31    // RE6    Cirkpump PlattV�rmeV�xlare
#define VVD        35    // RE2    Ventil VarmvattenDump
#define VPVV       37    // RE0    Ventil PlattV�rmeV�xlare
#define AI_PIN_G1  54    // RB0
#define AI_PIN_G2  55    // RB1
#define AI_PIN_G3  56    // RB2
#define BUS        77    // RD7    1-wire
#define FVV        75    // RC13   fl�desvakt tappvarmvatten

#define MUX_ADR_A  49    // RD11
#define MUX_ADR_B  51    // RG8
#define MUX_ADR_C  53    // RG9
#define FL_VV_IN   02    // RE8 - INT1  fl�desm�tare tappvarmvatten inkommande
#define FL_PVV     03    // RD0 - INT0  fl�desm�tare tanksida plattv�rmev�xlare
#define FL_SOL     07    // RE9 - INT2  fl�desm�tare solslinga
 

//#define PCF8591 (0x90 >> 1) // I2C bus address

const char SEPARATOR = '-';
unsigned int raw;
float temp, TKA_T, TDU_T, TVI_T; //, TM1SS, TK1SS, TU, TA // , TV2SS, TK2SS, , TVA, TDI, TVU, TR, TVD;
uint16_t TV1SS_PT;
uint16_t TVVB_PT;               // PT100 givare p� varmvattenberendare
uint16_t PSS, PTANK;
float FVVB, FTANK, FSOL;      // fl�de f�r ink vv, tankvatten genom v�rmev�xlare, solslinga 
const int TVVB_PT_CH  = 1;    // adress f�r PT100-givare TVVB_PT (LimnoTip-kort kanal 1, grupp 1)
const int TV1SS_PT_CH = 2;    // adress f�r PT100-givare TV1SS_PT (LimnoTip-kort kanal 0, grupp 1) 
const int PSS_CH  = 3;        // adress f�r tryckgivare solslinga
const int PTANK_CH = 4;       // adress f�r tryckgivare acktank
 
//----------------------------------------
// Temperaturkonstanter
//----------------------------------------
const float SS_DIFF_START = 3.0;
const float SS_DIFF_STOP  = 1.0;
const float VV_DUMP_OPEN = 88.0;
const float VV_DUMP_CLOSE = 83.0;
//const float MAX_DIFF_PVV = 3.0;    // Max till�ten tempdiff mellan kallsida plattv�rmev�xlare och inkommande kallvatten
const float START_TEMP_PVV = 18.0; // Starta cirkpump n�r ikommande kallvatten �r kallare �r denna temp

unsigned long CPVV_MIN_RUNTIME = 4000; // Minimal g�ngtid f�r CPVV (enhet ms)
unsigned long CPVV_MIN_OFFTIME = 4000; // Minimal stopptid f�r CPVV (enhet ms)
float TakTolerans = 2.0;

boolean FFV_ON = false;        
boolean CSS_ON = false;
boolean DUMP_OPEN = false;
boolean CPVV_ON = false;
int iFVV = LOW;

volatile int FlowCount_FVVB;
volatile unsigned long FlowCount_FTANK;
volatile unsigned long FlowCount_FSOL;
unsigned long oldFlowConvTime;
volatile int currentFlowCount_FVVB;
volatile unsigned long currentFlowCount_FTANK;
volatile unsigned long currentFlowCount_FSOL;

//----------------------------------------
// Kommunikation med taket
//----------------------------------------

COMValue COM_1_TV1SS, COM_1_TM1SS, COM_1_TK1SS, COM_1_TU, COM_1_TA;
COMValue COM_2_TV1SS, COM_2_TM1SS, COM_2_TK1SS; //, COM_2_TU, COM_2_TA;
COMValue COM_3_TV1SS, COM_3_TM1SS, COM_3_TK1SS; //, COM_3_TU, COM_3_TA;
float TV1SS, TM1SS, TK1SS, TU, TA;

const byte ID_TV1SS = 0x08;  // Seriellt - Solslinga varmsida, ut fr�n solf�ngare
const byte ID_TM1SS = 0xE8;  // Seriellt - Solslinga mellan solf�ngarna
const byte ID_TK1SS = 0x57;  // Seriellt - Solslinga kallsida in till solf�ngare
const byte ID_TU = 0x5A;     // Seriellt - Utetemp p� taket
const byte ID_TA = 0x7C;     // Seriellt - Temp i Arduino-dosan p� taket

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

Pipe tx_0(TXSIZE,true,tx_1_Buffer);   // NA
Pipe tx_1(TXSIZE,true,tx_1_Buffer);   // NA
Pipe tx_2(TXSIZE,true,tx_1_Buffer);   // Raspberry
Pipe tx_3(TXSIZE,true,tx_1_Buffer);   // NA

Pipe rx_0(RXSIZE,false,rx_1_Buffer);   // B0
Pipe rx_1(RXSIZE,false,rx_1_Buffer);   // B1
Pipe rx_2(RXSIZE,false,rx_1_Buffer);   // NA (Raspberry)
Pipe rx_3(RXSIZE,false,rx_1_Buffer);   // B2

volatile bool tx_0_Running = false;
volatile bool tx_1_Running = false;
volatile bool tx_2_Running = false;
volatile bool tx_3_Running = false;
