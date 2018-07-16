
#define EE24LC_MAXBYTES  8000 // LC512->64000  LC64->8000
const int DEVICEADDRESS = 0x50; // 0b0101 0 a2 a1 a0

#define STARTADDRESS_SENSORS 0x20


void TestE2(){
  for(int memAdr=0; memAdr<512; memAdr++){
//    RaspiPrint("W:");
//    WireEepromWriteByte(memAdr, (byte)memAdr);
    RaspiPrint(memAdr);
    RaspiPrint(" , ");
    RaspiPrintln(byteAsHexString(WireEepromRead(memAdr),true));
    delay(1);   
  }
}

// ----------------------------------------------------------------
// NOTE! Wire handles max 32 bytes in a buffer
void WireEepromWriteBuffer(uint16_t E2_address, uint8_t Size, uint8_t* buf){
  uint16_t adr = E2_address;
  if(adr < (EE24LC_MAXBYTES-Size)){
    Wire.beginTransmission(DEVICEADDRESS);
    Wire.write( (adr >> 8) & 0xFF );
    Wire.write( (adr >> 0) & 0xFF );
    Wire.write(buf,Size);
    Wire.endTransmission();
    delay(5);
  } 
}

// ----------------------------------------------------------------
void WireEepromReadBuffer(uint16_t E2_address, int Size, uint8_t* buf){
  Wire.beginTransmission(DEVICEADDRESS);
  Wire.write( (E2_address >> 8) & 0xFF );
  Wire.write( (E2_address >> 0) & 0xFF );
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(DEVICEADDRESS, Size);
  while(Wire.available()){
    *buf = Wire.read();
    buf++;
  }
}

// ----------------------------------------------------------------
void WireEepromWriteByte(uint16_t theMemoryAddress, uint8_t u8Byte) 
{
    Wire.beginTransmission(DEVICEADDRESS);
    Wire.write( (theMemoryAddress >> 8) & 0xFF );
    Wire.write( (theMemoryAddress >> 0) & 0xFF );
    Wire.write(u8Byte);
    Wire.endTransmission();
    delay(5);

}

// ----------------------------------------------------------------
uint8_t WireEepromRead(uint16_t theMemoryAddress) 
{
  uint8_t u8retVal = 0;
  Wire.beginTransmission(DEVICEADDRESS);
  Wire.write( (theMemoryAddress >> 8) & 0xFF );
  Wire.write( (theMemoryAddress >> 0) & 0xFF );
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(DEVICEADDRESS, 1);
  u8retVal = Wire.read();
  return u8retVal ;
}


