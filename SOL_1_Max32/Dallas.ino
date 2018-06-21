#include <OneWire.h>
#define SIZE_OF_DALLAS_NETWORK 17

OneWire  ds(BUS);  // on pin 77
struct t_OneWireID{
/*  char oldID[7]; */
  byte ADDR[8];
  float temp;
  char ID[25];
};

t_OneWireID DallasNetwork[SIZE_OF_DALLAS_NETWORK] = {
  {/*{'A','T','L','T','1',' ','\0'},*/ { 0x28, 0xFF, 0xC9, 0x01, 0x05, 0x15, 0x03, 0xD2 }, 0.0, {'2','8','-','F','F','-','C','9','-','0','1','-','0','5','-','1','5','-','0','3','-','D','2',' ','\0',}},  // TKA_D tidigare... bytt 11/6
  {/*{'S','H','O','T','1',' ','\0'},*/ { 0x28, 0xFF, 0x14, 0x04, 0x05, 0x15, 0x03, 0xE1 }, 0.0, {'2','8','-','F','F','-','1','4','-','0','4','-','0','5','-','1','5','-','0','3','-','E','1',' ','\0',}},
  {/*{'S','H','I','T','1',' ','\0'},*/ { 0x28, 0xFF, 0xA2, 0x04, 0x05, 0x15, 0x03, 0x32 }, 0.0, {'2','8','-','F','F','-','A','2','-','0','4','-','0','5','-','1','5','-','0','3','-','3','2',' ','\0',}},
  {/*{'A','T','O','T','1',' ','\0'},*/ { 0x28, 0x9C, 0x55, 0xDD, 0x06, 0x00, 0x00, 0xD8 }, 0.0, {'2','8','-','9','C','-','5','5','-','D','D','-','0','6','-','0','0','-','0','0','-','D','8',' ','\0',}},
  {/*{'H','T','I','T','1',' ','\0'},*/ { 0x28, 0x54, 0x0C, 0xDE, 0x06, 0x00, 0x00, 0x08 }, 0.0, {'2','8','-','5','4','-','0','C','-','D','E','-','0','6','-','0','0','-','0','0','-','0','8',' ','\0',}},
  {/*{'H','T','O','T','1',' ','\0'},*/ { 0x28, 0x82, 0xFB, 0xDC, 0x06, 0x00, 0x00, 0x8E }, 0.0, {'2','8','-','8','2','-','F','B','-','D','C','-','0','6','-','0','0','-','0','0','-','8','E',' ','\0',}},
  {/*{'H','D','I','T','1',' ','\0'},*/ { 0x28, 0x90, 0xFB, 0xDD, 0x06, 0x00, 0x00, 0x34 }, 0.0, {'2','8','-','9','0','-','F','B','-','D','D','-','0','6','-','0','0','-','0','0','-','3','4',' ','\0',}},
  {/*{'H','D','O','T','1',' ','\0'},*/ { 0x28, 0xEC, 0x3D, 0xDD, 0x06, 0x00, 0x00, 0x6F }, 0.0, {'2','8','-','E','C','-','3','D','-','D','D','-','0','6','-','0','0','-','0','0','-','6','F',' ','\0',}},
  {/*{'T','E','R','T','1',' ','\0'},*/ { 0x28, 0x4B, 0xA4, 0xDE, 0x06, 0x00, 0x00, 0x7A }, 0.0, {'2','8','-','4','B','-','A','4','-','D','E','-','0','6','-','0','0','-','0','0','-','7','A',' ','\0',}},
  {/*{'V','O','D','T','1',' ','\0'},*/ { 0x28, 0xFF, 0x9F, 0xED, 0x04, 0x15, 0x03, 0x2B }, 0.0, {'2','8','-','F','F','-','9','F','-','E','D','-','0','4','-','1','5','-','0','3','-','2','B',' ','\0',}},
  {/*{'A','R','P','T','1',' ','\0'},*/ { 0x28, 0xFF, 0x3E, 0x5B, 0x6F, 0x14, 0x04, 0x6E }, 0.0, {'2','8','-','F','F','-','3','E','-','5','B','-','6','F','-','1','4','-','0','4','-','6','E',' ','\0',}},
  {/*{'O','H','L','T','1',' ','\0'},*/ { 0x28, 0xFF, 0x72, 0x63, 0x6F, 0x14, 0x04, 0xF2 }, 0.0, {'2','8','-','F','F','-','7','2','-','6','3','-','6','F','-','1','4','-','0','4','-','F','2',' ','\0',}},
  {/*{'A','T','M','T','1',' ','\0'},*/ { 0x28, 0xFF, 0x68, 0x74, 0x6F, 0x14, 0x04, 0x8D }, 0.0, {'2','8','-','F','F','-','6','8','-','7','4','-','6','F','-','1','4','-','0','4','-','8','D',' ','\0',}},
  {/*{'A','T','H','T','1',' ','\0'},*/ { 0x28, 0xFF, 0x35, 0x5F, 0x53, 0x14, 0x01, 0x4A }, 0.0, {'2','8','-','F','F','-','3','5','-','5','F','-','5','3','-','1','4','-','0','1','-','4','A',' ','\0',}},
  {/*{'A','T','H','T','2',' ','\0'},*/ { 0x28, 0xFF, 0x52, 0xCA, 0x53, 0x14, 0x01, 0x6A }, 0.0, {'2','8','-','F','F','-','5','2','-','C','A','-','5','3','-','1','4','-','0','1','-','6','A',' ','\0',}},
  {/*{'A','T','L','T','2',' ','\0'},*/ { 0x28, 0x16, 0x22, 0x08, 0x00, 0x00, 0x80, 0x1C }, 0.0, {'2','8','-','1','6','-','2','2','-','0','8','-','0','0','-','0','0','-','8','0','-','1','C',' ','\0',}},
  {/*{'A','T','L','T','3',' ','\0'},*/ { 0x28, 0xB7, 0x20, 0x1F, 0x00, 0x00, 0x80, 0x4A }, 0.0, {'2','8','-','B','7','-','2','0','-','1','F','-','0','0','-','0','0','-','8','0','-','4','A',' ','\0',}}
//  {{'A','D','D','R','3',' ','\0'}, { 0x28, 0xFF, 0x97, 0x6B, 0x54, 0x14, 0x00, 0x98 }, 0.0},
// Suck!!!  Vaffö ä de så svårt å klicka på SAVE...   28:16:22:08:00:00:80:1C  & 28:B7:20:1f:00:00:80:40 ATM_T2 resp ATM_T3...
};


//int i,j;
byte present = 0;
byte data[12];
float celsius;
unsigned int dallas_raw;

void ScanOneWire(){
  byte i;
  for (int j=0; j<SIZE_OF_DALLAS_NETWORK; j++){
    ds.reset();
    ds.select(DallasNetwork[j].ADDR);
    ds.write(0x44,1);         // start conversion, with parasite power on at the end  ds.write(0x44);
  }

  delay(1000);     // maybe 750ms is enough, maybe not
  
  for (int j=0; j<SIZE_OF_DALLAS_NETWORK; j++){
    present = ds.reset();
    ds.select(DallasNetwork[j].ADDR);    
    ds.write(0xBE);         // Read Scratchpad
    if(present){
      for ( i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = ds.read();
      }
      // convert the data to actual temperature
       
      celsius = (float)((int16_t)(data[1] << 8) | data[0]) / 16.0;
      if(data[1] == 0x05 && data[0] == 0x50){  // 0x0550 => 85degC == initialvärde på DS18b20 
        RaspiPrint(DallasNetwork[j].ID); 
        RaspiPrintln(" INIT");
      }
      else if (data[1] == 0xFF && data[0] == 0xFF){
        RaspiPrint(DallasNetwork[j].ID); RaspiPrintln("MISSING"); 
      }
      else{
        if (celsius > -40.0 && celsius < 125.0){ 
          DallasNetwork[j].temp = celsius;
          RaspiPrint(DallasNetwork[j].ID); 
          RaspiPrintln(DallasNetwork[j].temp);
        }
        else{
          RaspiPrint(DallasNetwork[j].ID); 
           RaspiPrint(" "); RaspiPrintln(celsius);
        }
      }
    }
    else{
      sendErrorToRaspi("DALLAS NETWORK MISSING"); }
  }
}  

float DallasTemp(int Sensor){
  return DallasNetwork[Sensor].temp;
}

String SensorID(int index){
  String str = byteAsHexString(DallasNetwork[index].ADDR[0],false);
  for(int i=1;i<8;i++){
    str = str +'-'+ byteAsHexString(DallasNetwork[index].ADDR[i],false);
  }
  return str;  
}

const char *hex = "0123456789ABCDEF";
String byteAsHexString(byte value, bool usePrefix){
  char pout[5];
  int index = 0;
  if(usePrefix){
    pout[index++]='0';
    pout[index++]='x';
  }
  pout[index++] = hex[(value>>4) & 0xF];
  pout[index++] = hex[ value     & 0xF];
  pout[index] = '\0';    
  String str(pout);
  return str;
}

