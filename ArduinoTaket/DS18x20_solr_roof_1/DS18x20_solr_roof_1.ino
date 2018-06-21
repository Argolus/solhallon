#include <OneWire.h>

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library

OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary)

void setup(void) {
  Serial.begin(38400);
}

void loop(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  byte frame[15];
  uint16_t checksum;
  int j;
  
  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
  
  if (OneWire::crc8(addr, 7) != addr[7]) {
//      Serial.println("CRC is not valid!");
      return;
  }
  
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
  //    Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
   //   Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
   //   Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
   //   Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  frame[0] = 0xEE;
  frame[1] = 0xFF; 
  frame[2] = 10;   //data length
  for(j=0; j<8; j++){
    frame[j+3] = addr[j];
  }
  frame[11] = data[1];
  frame[12] = data[0];
  checksum = fletcher16(frame, 13);
  frame[13] = (checksum & 0xFF00) >> 8;
  frame[14] = checksum & 0xFF;
  
  Serial.write(frame,15);

  

}
