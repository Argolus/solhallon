const int NoOfTests = 7;
typedef void (*FP)(void);
FP TESTCASES[NoOfTests] = {&TestCase_1, &TestCase_2, &TestCase_3, &TestCase_4, &TestCase_5, &TestCase_6 , &TestCase_7};

void RUN_DT(){
  if(DESIGN_TEST){
    for(int i=0; i<NoOfTests; i++){
      TESTCASES[i]();
    }
    DESIGN_TEST = false;
  }
}

void TestCase_1(){
  COMValue A, B, C;
  float Tolerance = 2.0;
  bool useLow = true;  
  float result;
  A.setSafeValue(5.0, 3000);
  B.setSafeValue(5.0, 3000);
  C.setSafeValue(5.0, 3000);

  A.setValue(11.0);
  B.setValue(12.0);
  C.setValue(13.0);
  result = COMValue::Vote(A, B, C, Tolerance, useLow);
  if(result > 11.9 && result < 12.1){
    RaspiPrintln("TC_1 : OK");  
  }
  else{
    RaspiPrint("TC_1 : NOK - Expected: 12.0 got ");
    RaspiPrintln(result);
  }
}

void TestCase_2(){
  COMValue A, B, C;
  float Tolerance = 2.0;
  bool useLow = true;  
  float result;
  A.setSafeValue(5.0, 3000);
  B.setSafeValue(5.0, 3000);
  C.setSafeValue(5.0, 3000);

  A.setValue(20.0);
  B.setValue(21.0);
  C.setValue(25.0);
  result = COMValue::Vote(A, B, C, Tolerance, useLow);
  if(result > 20.4 && result < 20.6){
    RaspiPrintln("TC_2 : OK");  
  }
  else{
    RaspiPrint("TC_2 : NOK - Expected: 20.5 got ");
    RaspiPrintln(result);
  }
}

void TestCase_3(){
  COMValue A, B, C;
  float Tolerance = 2.0;
  bool useLow = true;  
  float result;
  A.setSafeValue(5.0, 3000);
  B.setSafeValue(5.0, 3000);
  C.setSafeValue(5.0, 3000);

  A.setValue(30.0);
  B.setValue(34.0);
  C.setValue(35.0);
  result = COMValue::Vote(A, B, C, Tolerance, useLow);
  if(result > 34.4 && result < 34.6){
    RaspiPrintln("TC_3 : OK");  
  }
  else{
    RaspiPrint("TC_3 : NOK - Expected: 34.5 got ");
    RaspiPrintln(result);
  }
}

void TestCase_4(){
  COMValue A, B, C;
  float Tolerance = 2.0;
  bool useLow = true;  
  float result;
  A.setSafeValue(5.0, 3000);
  B.setSafeValue(5.0, 3000);
  C.setSafeValue(5.0, 3000);

  A.setValue(40.0);
  B.setValue(43.0);
  C.setValue(46.0);
  result = COMValue::Vote(A, B, C, Tolerance, useLow);
  if(result > 39.9 && result < 40.1){
    RaspiPrintln("TC_4 : OK");  
  }
  else{
    RaspiPrint("TC_4 : NOK - Expected: 40.0 got ");
    RaspiPrintln(result);
  }
}


void TestCase_5(){
  COMValue A, B, C;
  float Tolerance = 2.0;
  bool useLow = false;  
  float result;
  A.setSafeValue(5.0, 3000);
  B.setSafeValue(5.0, 3000);
  C.setSafeValue(5.0, 3000);

  A.setValue(40.0);
  B.setValue(43.0);
  C.setValue(46.0);
  result = COMValue::Vote(A, B, C, Tolerance, useLow);
  if(result > 45.9 && result < 46.1){
    RaspiPrintln("TC_5 : OK");  
  }
  else{
    RaspiPrint("TC_5 : NOK - Expected: 46.0 got ");
    RaspiPrintln(result);
  }
}

void TestCase_6(){
  COMValue A, B, C;
  float Tolerance = 2.0;
  bool useLow = false;  
  float result;
  A.setSafeValue(5.0, 3000);
  B.setSafeValue(5.0, 3000);
  C.setSafeValue(5.0, 3000);

  A.setValue(50.0);
  B.setValue(51.0);
  C.setValue(55.0);
  result = COMValue::Vote(A, B, C, Tolerance, useLow);
  if(result > 50.4 && result < 50.6){
    RaspiPrintln("TC_6 : OK");  
  }
  else{
    RaspiPrint("TC_6 : NOK - Expected: 50.5 got ");
    RaspiPrintln(result);
  }
}

void TestCase_7(){
  bool TC_OK = true;
  int fail = 0;
  t_Value DT_TEMPS[NR_OF_SENSORS];
  StoreSENSORS();
//  delay(100);
//  TestE2();
  byte* pBuf = (byte*)DT_TEMPS;
  byte* pB2 = (byte*)SENSORS;
  for(int i=0; i<sizeof(DT_TEMPS) && TC_OK; i++){
    *pBuf=WireEepromRead(STARTADDRESS_SENSORS + i);
    if(*pBuf != *pB2){
      TC_OK = false;
      fail = i;
    }
    pBuf++; pB2++;
  }

  if(TC_OK){
    RaspiPrintln("TC_7 : OK");
  } 
  else{
    RaspiPrint("TC_7 : NOK - byte ");
    RaspiPrintln(fail);
  }
}

