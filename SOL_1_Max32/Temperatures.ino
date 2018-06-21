
struct t_Value{
  byte ADDR[8];
  float temp;
};

enum ValIndex{
  SPO_T1 = 0, 
  SPO_T2 = 1, 
  SPO_T3 = 2, 
  SPO_T4 = 3, 
  SPM_T1 = 4, 
  SPM_T2 = 5, 
  SPM_T3 = 6, 
  SPI_T1 = 7, 
  SPI_T2 = 8, 
  SPI_T3 = 9,
  RFA_T1 = 10,
  REB_T1 = 11, 
  NR_OF_VALUES
};

t_Value TEMPS[NR_OF_VALUES];


float getTemp(int Index){
  if(Index<NR_OF_VALUES) 
     return TEMPS[Index].temp;
  return 555.0;
}

void UpdateAddr(int Index, byte *Addr){
  if(Index<NR_OF_VALUES){ 
    for(int i = 0; i<8; i++){
      TEMPS[Index].ADDR[i] = Addr[i];
    }
  }
}

void SetNewTemp(struct t_Value *val){
  bool found = false;
  int i = 0, j = 0;
  for(;j<NR_OF_VALUES&&!found;j++){
    for(; i<8 && !found;i++){
      if(val->ADDR[i] != TEMPS[j].ADDR[i])
        break;
    }
    found = i==8;
  } 
  if(found) TEMPS[j].temp = val->temp;
}
byte adSPO_T1[8] = {0x28, 0xB8, 0x46, 0xDE, 0x06, 0x00, 0x00, 0x08}; 
byte adSPO_T2[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPO_T3[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPO_T4[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPM_T1[8] = {0x28, 0x9D, 0x70, 0xE8, 0x05, 0x00, 0x00, 0xE8}; 
byte adSPM_T2[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPM_T3[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPI_T1[8] = {0x28, 0x48, 0x51, 0xDE, 0x06, 0x00, 0x00, 0x57}; 
byte adSPI_T2[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; 
byte adSPI_T3[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
byte adRFA_T1[8] = {0x28, 0x37, 0x72, 0xDC, 0x06, 0x00, 0x00, 0x5A};
byte adREB_T1[8] = {0x28, 0x49, 0xB2, 0xDD, 0x06, 0x00, 0x00, 0x7C}; 


void UpdateRoofAdresses(){
  UpdateAddr(SPO_T1, adSPO_T1);
  UpdateAddr(SPM_T1, adSPM_T1);
  UpdateAddr(SPI_T1, adSPI_T1);
  UpdateAddr(RFA_T1, adRFA_T1);
  UpdateAddr(REB_T1, adREB_T1);
}
