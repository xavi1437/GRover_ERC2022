#include "Arduino.h"
#include "robArm.h"


robArm::robArm(int sPin, int dPin)
{
    _sPin = sPin;
    _dPin = dPin;
    robArm::InitPin();
}

void robArm::InitPin()
{
    pinMode(_sPin, OUTPUT);
    pinMode(_dPin, OUTPUT);
}

void robArm::Move(int speed, int dir)
{
    analogWrite(_sPin, speed);
    digitalWrite(_dPin, dir);
}

void robArm::Stop()
{
    analogWrite(_sPin, 0);
    digitalWrite(_dPin, LOW);
}
