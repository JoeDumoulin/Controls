// control.h
#ifndef CONTROL_H
#define CONTROL_H

class Control
{
  bool _active;

public:
  Control():_active(false)
  {}

  void activate() { _active = true; }
  void deactivate() { _active = false; }
  bool isActive() const { return _active; }
};

class PinControl 
{
  int _minValue, _maxValue;
//  int _threshValue; // limit
//  int _overUnder;   // 1 = over, 0 = under
   
  int _inpin;  // something that controls (analog)
  int _outpin; // something controlled (on/off)
  bool _active;

  void init();
public:
  PinControl(int inpin, int outpin, int minValue, int maxValue)
    : _active(false), _inpin(inpin), _outpin(outpin), 
      _minValue(minValue), _maxValue(maxValue)
  { 
    init();
  }

  void activate() { _active = true; }
  void deactivate() { _active = false; }
  bool isActive() const { return _active; }

  bool checkAnalog(int threshValue, int overUnder);
  void resetPin();
};


#endif // CONTROL_H
