
void setAdr(int adr)
{
  digitalWrite(MUX_ADR_A, bitRead(adr,0));
  digitalWrite(MUX_ADR_B, bitRead(adr,1));
  digitalWrite(MUX_ADR_C, bitRead(adr,2)); 
}

int ReadAnalog(int port, int address)
{
  setAdr(address);
  return analogRead(port);
}
/*
void setAnalogOut(byte value){
//  analogWrite(pin, value); //pwm output
  Wire.beginTransmission(PCF8591); // transmit to device #
  Wire.send(value);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
} */
