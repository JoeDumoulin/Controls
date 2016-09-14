// Translate SESH code to a bigger screen and the UTFT library.
#include <UTFT.h>
#include <UTFT_Geometry.h>
#include <URTouch.h>
#include <Timer.h>

#include "helpers.h"
#include "Region.h"
#include "control.h"
#include "state.h"

// all state management happens in this module
State state;

// Declare font set
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

// Graphics library objects
UTFT glcd(CTE70,38,39,40,41);
UTFT_Geometry geo(&glcd);

// touch screen
URTouch touch(6,5,4,3,2);

// screen objects
uint16_t marginTop, marginBottom, marginLeft, marginRight;

SelectValueRegion regions[] = {
  SelectValueRegion(0, 0, glcd.getDisplayYSize()/5, glcd.getDisplayXSize()*2/3, 5, 1, 300, 50), // Temperature selection
  SelectValueRegion(glcd.getDisplayYSize()/5, 0, glcd.getDisplayYSize()*2/5, glcd.getDisplayXSize()*2/3, 5, 10, 1400, 500), // Pressure selection
  SelectValueRegion(glcd.getDisplayYSize()*2/5, 0, glcd.getDisplayYSize()*3/5, glcd.getDisplayXSize()*2/3, 5, 1, 100, 10)   // Timer selection
};

ButtonRegion buttons[] = {
  ButtonRegion(0, glcd.getDisplayXSize()*2/3, glcd.getDisplayYSize()/5, glcd.getDisplayXSize()*5/6, 5), // WarmUp
  ButtonRegion(glcd.getDisplayYSize()/5, glcd.getDisplayXSize()*2/3, glcd.getDisplayYSize()*2/5, glcd.getDisplayXSize()*5/6, 5),  // Set Pressure & Timer
  ButtonRegion(glcd.getDisplayYSize()*2/5, glcd.getDisplayXSize()*2/3, glcd.getDisplayYSize()*3/5, glcd.getDisplayXSize()*5/6, 5),  // Go!!
  ButtonRegion(glcd.getDisplayYSize()*3/5, glcd.getDisplayXSize()*2/3, glcd.getDisplayYSize()*4/5, glcd.getDisplayXSize()*5/6, 5)  // Reset
};

// Controls
PinControl controls[] = {
  PinControl(A10, 14, 50, 300),  // temperature monitor
  PinControl(A11, 15, 500, 1200)  // pressure sensor
};

// process timer
Timer processTimer;
Timer timers[] = {processTimer};
uint8_t numTimers = 1;

void setup() {
  //serial console for debugging values
  Serial.begin(9600);
  Serial.println("SESH2_7inScreen_UTFT");
  
  // set up the screen
  marginTop = marginBottom = marginLeft = marginRight = 5;
  glcd.InitLCD();
  glcd.clrScr();
  glcd.setFont(SmallFont);

  // set up the touch interface
  touch.InitTouch();
  touch.setPrecision(PREC_MEDIUM);

  state = resetTransition(state, regions, buttons, controls);
}

void loop() {
  State newState = state;
  
  int pressed = -1;
  // first check for touch events
  if (touch.dataAvailable())
  {
    Serial.println("dataAvailable");
    // get touch location
    touch.read();
    Point touchPoint(touch.getX(), touch.getY());
//    Serial.print("("); Serial.print(touchPoint.x); Serial.print(", "); Serial.print(touchPoint.y); Serial.println(")");

    if (buttons[WarmUpIndex].check(touchPoint)){
      Serial.println("Warm-Up button pressed");
      newState = warmUp(state, regions, buttons, controls);
    }
    else if (buttons[SetPressureTimerIndex].check(touchPoint)) {
      glcd.clrScr();
      Serial.println("Set Pressure & Time Button Pressed");
      newState = timerAndPressureSet(state, regions, buttons, controls);

    }
    else if (buttons[ResetIndex].check(touchPoint)) {
      Serial.println("Reset Button Pressed");
      newState = resetTransition(state, regions, buttons, controls);
    }
    else {
      // Check to see if any region values should change
      if (regions[TempIndex].check(touchPoint)) {
        regions[TempIndex].printValue(VGA_WHITE, BigFont, VGA_BLACK);
//        Serial.println("temperature selected");
      }
      else if (regions[1].check(touchPoint)) {
        Serial.println("pressure selected");
        regions[PressureIndex].printValue(VGA_WHITE, BigFont, VGA_BLACK);
      }
      else if (regions[2].check(touchPoint)) {
        regions[TimerIndex].printValue(VGA_WHITE, BigFont, VGA_BLACK);
      }
    }
  }
  // next check control events
  else if (controls[TempControlIndex].checkAnalog(regions[TempIndex].Value(), 0) && state == WARMUP) {
    Serial.println("temperature control check!");
    newState = setPointReached(state, regions, buttons, controls);
  }
  else if (controls[PressureControlIndex].checkAnalog(regions[PressureIndex].Value(), 0) && state == READY) {
    Serial.println("Run Process!");
    newState = runProcess(state, regions, buttons, controls);
  }

  // next check timer events
  for (uint8_t i = 0; i < numTimers; ++i)
    timers[i].update();
  // finally set the next state
  state = newState;
  
}


