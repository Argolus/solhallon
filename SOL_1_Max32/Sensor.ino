boolean IsEqualSensorID(byte Sensor1[], byte Sensor2[], int LengthOfID){
  boolean IsEqual = false; 
  do{
    LengthOfID--;
    IsEqual = Sensor1[LengthOfID] == Sensor2[LengthOfID];
  }while(LengthOfID>0 && IsEqual);
}    
