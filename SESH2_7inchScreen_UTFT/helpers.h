#ifndef HELPERS_H
#define HELPERS_H

struct Point {
  uint16_t x;
  uint16_t y;

  Point(uint16_t X, uint16_t Y) : x(X), y(Y)
  {}

  Point(const Point &p) : x(p.x), y(p.y)
  {}
};

enum SelectValueRegionIndex {
  TempIndex = 0,
  PressureIndex,
  TimerIndex
};
const int numRegions = 3;

enum ButtonRegionIndex {
  WarmUpIndex = 0,
  SetPressureTimerIndex,
  RunIndex,
  ResetIndex
};
const int numButtons = 4;

enum PinControlIndex {
  TempControlIndex = 0,
  PressureControlIndex
};
const int numControls = 2;


#endif // HELPERS_H

