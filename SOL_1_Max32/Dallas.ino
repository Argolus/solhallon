#include <OneWire.h>
#define SIZE_OF_DALLAS_NETWORK 16

OneWire  ds(BUS);  // on pin 77
struct t_OneWireID{
/*  char oldID[7]; */
  teValIndex Sensor;
  float temp;
//  char ID[25];
};

t_OneWireID DallasNetwork[SIZE_OF_DALLAS_NETWORK] = {
  {SOLAR_HEATEX_OUT_DALLAS, 0.0 },
  {ACK_TANK_LOW_1_DALLAS, 0.0   },
  {SOLAR_HEATEX_IN_DALLAS, 0.0  },
  {ACK_TANK_OUT_DALLAS, 0.0     },
  {HEATEX_TANK_IN_DALLAS, 0.0   },
  {HEATEX_TANK_OUT_DALLAS, 0.0  },
  {HEATEX_DOM_IN_DALLAS, 0.0    },
  {HEATEX_DOM_OUT_DALLAS, 0.0   },
  {TECH_ROOM_DALLAS, 0.0        },
  {DUMP_VALVE_DALLAS, 0.0       },
  {ACK_TANK_HEATEX_RETURN_DALLAS, 0.0 },
  {ACK_TANK_MID_1_DALLAS, 0.0   },
  {ACK_TANK_HIGH_1_DALLAS, 0.0  },
  {ACK_TANK_HIGH_2_DALLAS, 0.0  },
  {ACK_TANK_LOW_2_DALLAS, 0.0   },
  {ACK_TANK_LOW_3_DALLAS, 0.0   }
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
    ds.select(SENSORS[DallasNetwork[j].Sensor].ADDR);
    ds.write(0x44,1);         // start conversion, with parasite power on at the end  ds.write(0x44);
  }

  delay(1000);     // maybe 750ms is enough, maybe not
  
  for (int j=0; j<SIZE_OF_DALLAS_NETWORK; j++){
    delay(150);
    present = ds.reset();
    ds.select(SENSORS[DallasNetwork[j].Sensor].ADDR);    
    ds.write(0xBE);         // Read Scratchpad
    if(present){
      for ( i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = ds.read();
      }
      // convert the data to actual temperature
       
      celsius = (float)((int16_t)(data[1] << 8) | data[0]) / 16.0;
      if(data[1] == 0x05 && data[0] == 0x50){  // 0x0550 => 85degC == initialvärde på DS18b20 
        RaspiPrint(DallasSensorID(DallasNetwork[j].Sensor)); 
        RaspiPrintln(" INIT");
      }
      else if ((data[1] == 0xFF && data[0] == 0xFF) || (data[1] == 0x00 && data[0] == 0x00)){
        RaspiPrint(DallasSensorID(DallasNetwork[j].Sensor)); RaspiPrintln(" MISSING"); 
      }
      else{
        if (celsius > -40.0 && celsius < 125.0){ 
          DallasNetwork[j].temp = celsius;
          RaspiPrint(DallasSensorID(DallasNetwork[j].Sensor)); 
          RaspiPrintln(DallasNetwork[j].temp);
        }
        else{
          RaspiPrint(DallasSensorID(DallasNetwork[j].Sensor)); 
          RaspiPrint(" "); RaspiPrintln(celsius);
        }
      }
    }
    else{
//      RaspiPrint(DallasSensorID(DallasNetwork[j].Sensor)); RaspiPrintln("MISSING");
      sendErrorToRaspi("DALLAS SENSOR MISSING"); 
    }
  }
}  

float DallasTemp(teValIndex Sensor){
  bool found = false;
  int index = 0;
  float temp = 0.0;
  while(!found && index < SIZE_OF_DALLAS_NETWORK)
  {
    if(DallasNetwork[index].Sensor == Sensor)
    {
      found = true;
      temp = DallasNetwork[index].temp;
    }
    index++;
  }
  return temp;
}

String DallasSensorID(teValIndex index){
  String str = byteAsHexString(SENSORS[index].ADDR[0],false);
  for(int i=1;i<8;i++){
    str = str +'-'+ byteAsHexString(SENSORS[index].ADDR[i],false);
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

