/*#include <EEPROM.h>

int E2ConfigDataSize = 0x22;

void InitEEPROM()
{
  int E2Size = E2ConfigDataSize * (GROUP_SIZE_1 + GROUP_SIZE_2 + GROUP_SIZE_3);
  if(E2Size < 1024)
    EEPROM.setMaxAddress(E2Size);
}

void StoreScaleInEEPROM(int grp, int cont)
{
  if(cbRead > E2ConfigDataSize)
  {
    Serial.print("Config data too big for E2: ");
    Serial.print(cbRead);
    Serial.print(" ");
    Serial.print(E2ConfigDataSize);
    Serial.print(" ");
    Serial.print(grp);
    Serial.print(" ");
    Serial.println(cont);
    return;
  }
  int tunna = 0;
  switch(grp)
  {
  case 0: 
    tunna = cont;
    break;
  case 1:
    tunna = GROUP_SIZE_1 + cont;
    break;
  case 2:
    tunna = GROUP_SIZE_1 + GROUP_SIZE_2 + cont;
    break;
  default:
    return;
  }  
  int addr = tunna * E2ConfigDataSize;
  for(int i=0; i<cbRead; i++)
  {
    if((addr+i)< EEPROM.getMaxAddress())
    {
      EEPROM.write(addr+i, rgbRead[i]);
      Serial.print("Store EEPROM addr: ");
      Serial.print(addr+i);
      Serial.print(" value: ");
      Serial.println(rgbRead[i], DEC);
    }
    else
    {
      Serial.print("Illegal EEPROM addr: ");
      Serial.print(addr+i);
      Serial.print(" ");
      Serial.println(EEPROM.getMaxAddress());
    }
  }
}

void ReadContainerSettingFromEEPROM(int grp, int cont)
{
  int tunna = 0;
  caTunna* caT;
  switch(grp)
  {
  case 0: 
    tunna = cont;
    cg.getTunna(cont, &caT);
    break;
  case 1:
    tunna = GROUP_SIZE_1 + cont;
    mg1.getTunna(cont, &caT);
    break;
  case 2:
    tunna = GROUP_SIZE_1 + GROUP_SIZE_2 + cont;
    mg2.getTunna(cont, &caT);
    break;
  default:
    caT = NULL;
    break;
  }  
  int addr = tunna * E2ConfigDataSize;

  for(int i=0, cbRead = 0; i<E2ConfigDataSize; i++)
  {
    if((addr+i)< EEPROM.getMaxAddress())
      rgbRead[cbRead++] = EEPROM.read(addr+i);
    else
    {
      Serial.print("Illegal EEPROM addr: ");
      Serial.print(addr+i);
      Serial.print(" ");
      Serial.println(EEPROM.getMaxAddress());
    }
  }
  if(cbRead > 0)
    caT->DeSerialize(rgbRead);
  
} 

*/
