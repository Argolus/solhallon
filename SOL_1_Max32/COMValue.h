/*
 * Copyright (c) 2018, B3Labz
 * All rights reserved.
 * 
 * Class to handle 2oo3 voting
 * 
 */

#ifndef _COMVALUE_H
#define _COMVALUE_H

#if (ARDUINO >= 100) 
# include "Arduino.h"
#else
# include "WProgram.h"
#endif

class COMValue{
  private:
      float _Value;
      float _safeValue;
      unsigned long _timestamp;
      unsigned long _timeout;
  public:
      COMValue();
	    COMValue(float Value);
      COMValue(float safeValue, unsigned long timeout);
      COMValue(const COMValue &obj);
      void setValue(float newValue);
      void updateSafeValue(float newValue);
      void setSafeValue(float newValue, unsigned long timeout);
      bool inSafeState() const;
      float Value() const;
      float Value(bool* safeValueUsed) const;
      unsigned long TimeStamp() const;
      static float Vote(COMValue& a, COMValue& b, COMValue& c, float Tolerance, bool useLow);
};

/*
class CV_Voter{
  private:
    int _nbr;
    float _val;
    float _min;
    float _max;
    float _backupvalue;
    float _tolerance;
    bool _isSafeValue;
    
  public:
    void addValue(const COMValue&);
    void setTolerance(const float value); 
    float VotedValue();
    void resetValue();
};
*/
#endif
