#ifndef robArm_h
#define robArm_h
#include "Arduino.h"

#define STEP 400


class robArm{
  public:

    robArm(int sPin, int dPin);
    void Move(int speed, int dir);
    void Stop();
    void InitPin();

  private:  
    int _sPin;
    int _dPin;
  };
#endif