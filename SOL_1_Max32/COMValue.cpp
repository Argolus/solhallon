#include "COMValue.h"

COMValue::COMValue(){
  _Value = 0.0;
  _timestamp = millis();
}

COMValue::COMValue(float Value){
	_Value = Value;
	_timestamp = millis();
}

COMValue::COMValue(float safeValue, unsigned long timeout){
  _Value = 0.0;
  _safeValue = safeValue;
  _timeout = timeout;
  _timestamp = millis();
}

COMValue::COMValue(const COMValue &obj){
  _Value = obj._Value;
  _safeValue = obj._safeValue;
  _timestamp = obj._timestamp;
  _timeout = obj._timeout;
}


void COMValue::setValue(float Value){
	_Value = Value;
	_timestamp = millis();
}

void COMValue::updateSafeValue(float Value){
  _safeValue = Value;
}

void COMValue::setSafeValue(float Value, unsigned long timeout){
  _safeValue = Value;
  _timeout = timeout;
}

float COMValue::Value() const{
  if((millis()-_timestamp) < _timeout){  // since all are unsigned longs, millis()-_timestamp will (also wrap around and) always return duration as positive value!
    return _Value;
  }
  else{
    return _safeValue;
  }
}

float COMValue::Value(bool* safeValueUsed) const{
  if((millis()-_timestamp) < _timeout){  // since all are unsigned longs, millis()-_timestamp will (also wrap around and) always return duration as positive value!
    if(safeValueUsed != NULL) *safeValueUsed = false;
    return _Value;
  }
  else{
    if(safeValueUsed != NULL) *safeValueUsed = true;
    return _safeValue;
  }
}

bool COMValue::inSafeState() const{
  return (millis()-_timestamp) > _timeout;
}

unsigned long COMValue::TimeStamp() const{
	return _timestamp;
}

float COMValue::Vote(COMValue& a, COMValue& b, COMValue& c, float Tolerance,bool useLow){
  float Data[3], tmp;
  int nbr = 0;
  if(!a.inSafeState()){
    Data[nbr++]=a.Value();
  }
  if(!b.inSafeState()){
    Data[nbr++]=b.Value();
  }
  if(!c.inSafeState()){
    Data[nbr++]= c.Value();
  }
  if(nbr==0){
    // finns inga bra data, returnera safe value från a
    return a.Value();
  }
  if(nbr==1){
    // finns bara ett fräsht värde, använd det...
    return Data[0];
  }
  if(Data[0] > Data[1]){
    tmp = Data[0];
    Data[0] = Data[1];
    Data[1] = tmp;
  }
  if(nbr==2){
    // två bra läsningar, returnera högsta eller lägsta?
    if(useLow)
      return Data[0];
    return Data[1];
  }
  // alla tre läsningarna är bra. Nu kollar vi lite mer
  if(Data[1] > Data[2]){
    tmp = Data[1];
    Data[1] = Data[2];
    Data[2] = tmp;
  }
  if(Data[0] > Data[1]){
    tmp = Data[0];
    Data[0] = Data[1];
    Data[1] = tmp;
  }  
  // Data sortarad i storleksornding, kolla om de ligger inom tolerans.
  if(Data[2] - Data[0] <= Tolerance){
    // japp, alla data är okej. Kör på medelvärde
    return (Data[0] + Data[1] + Data[2]) / 3;  
  }
  // nähä, nåt värde är dåligt... 
  if(useLow && (Data[1] - Data[0] <= Tolerance)){
    return (Data[0] + Data[1]) / 2;
  }
  if((!useLow) && Data[2] - Data[1] <= Tolerance){
    return (Data[2] + Data[1]) / 2;
  }
  if(Data[1] - Data[0] <= Tolerance){
    return (Data[0] + Data[1]) / 2;
  }
  if(Data[2] - Data[1] <= Tolerance){
    return (Data[2] + Data[1]) / 2;
  }
   
  // det skiljer för mycket mellan alla värden..
  if(useLow)
    return Data[0];
  
  return Data[1];

}


/********************** CV_VOTER ************************ /
void CV_Voter::addValue(const COMValue& v){
  if(_nbr==0){
    _val = _min = _max = v.Value(&_isSafeValue);
    _nbr++;
    return;
  }
  if(v.inSafeState()) return;
  if(_isSafeValue){
    _isSafeValue = false;
    _val = _min = _max = v.Value();
    return;
  }
  _val += v.Value();
  _nbr++;
  if(v.Value() < _min) _min = v.Value();
  if(v.Value() > _max) _max = v.Value();
}

float CV_Voter::VotedValue(){
  if(_nbr==0){
    return 0.0;    
  }
  return _val/_nbr;
}

void CV_Voter::resetValue(){
  _nbr = 0;
  _val = 0.0;
  _isSafeValue = false;
}
*/
