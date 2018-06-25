const int NoOfTests = 4;
typedef void (*FP)(void);
FP TESTCASES[NoOfTests] = {&TestCase_1, &TestCase_2, &TestCase_3, &TestCase_4};

void RUN_DT(){
  if(DESIGN_TEST){
    for(int i=0; i<NoOfTests; i++){
      TESTCASES[i];
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
  if(result > 19.9 && result < 20.1){
    RaspiPrintln("TC_1 : OK");  
  }
  else{
    RaspiPrint("TC_1 : NOK - Expected: 20.0 got ");
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
    RaspiPrintln("TC_1 : OK");  
  }
  else{
    RaspiPrint("TC_1 : NOK - Expected: 34.5 got ");
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
  if(result > 42.9 && result < 43.1){
    RaspiPrintln("TC_1 : OK");  
  }
  else{
    RaspiPrint("TC_1 : NOK - Expected: 43.0 got ");
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

  A.setValue(50.0);
  B.setValue(51.0);
  C.setValue(55.0);
  result = COMValue::Vote(A, B, C, Tolerance, useLow);
  if(result > 50.4 && result < 50.6){
    RaspiPrintln("TC_1 : OK");  
  }
  else{
    RaspiPrint("TC_1 : NOK - Expected: 50.5 got ");
    RaspiPrintln(result);
  }
}

