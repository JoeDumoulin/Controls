 // control.cpp
#include <Arduino.h>

#include "control.h"

void PinControl::init() 
{
    pinMode(_inpin, INPUT);
    pinMode(_outpin, OUTPUT);
}

bool PinControl::checkAnalog(int threshValue, int overUnder)
{
  if (!isActive()) {
    return false;
  }
  int p = analogRead(_inpin);
  // 10% above/below
  int excess = (_maxValue - _minValue)/10;
  int m = map(p, 0, 680, _minValue - excess, _maxValue + excess);
  //Serial.println(m);
  if (m >= threshValue && overUnder == 1) {
    digitalWrite(_outpin, HIGH);
    return false;
  }
  if (m <= threshValue && overUnder == 0) {
    digitalWrite(_outpin, HIGH);
    return false;
  }
  digitalWrite(_outpin, LOW);
  return true;
}

void PinControl::resetPin()
{
  digitalWrite(_outpin, LOW);
}


