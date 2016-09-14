// state.cpp - functions for state transition go here.
//
#include <Arduino.h>
#include <stdint.h>
#include <UTFT.h>

#include "helpers.h"
#include "Region.h"
#include "state.h"

extern UTFT glcd;

// transition functions
State warmUp(State origin, SelectValueRegion* regions, ButtonRegion* buttons, PinControl *controls)
{
  // deactivate temerature selection
  regions[TempIndex].draw("TEMPERATURE", VGA_OLIVE, VGA_TEAL, VGA_WHITE);
  regions[TempIndex].deactivate();
  Serial.println(regions[TempIndex].isActive()?"temp is active":"temp is inactive");
  // activate pressure selection
  regions[PressureIndex].draw("PRESSURE", VGA_LIME, VGA_TEAL, VGA_WHITE);
  regions[PressureIndex].activate();
  // activate timer selection
  regions[TimerIndex].draw("TIMER", VGA_LIME, VGA_TEAL, VGA_WHITE);
  regions[TimerIndex].activate();

  // deactivate the Warm Up button
  buttons[WarmUpIndex].draw("WARM UP", VGA_OLIVE, VGA_OLIVE, VGA_WHITE);
  buttons[WarmUpIndex].deactivate();

  // activate the Set button for pressure and timer
  buttons[SetPressureTimerIndex].draw("SET", VGA_LIME, VGA_LIME, VGA_WHITE);
  buttons[SetPressureTimerIndex].activate();

  // deactivate the run button
  buttons[RunIndex].draw("GO!!", VGA_OLIVE, VGA_OLIVE, VGA_WHITE);
  buttons[RunIndex].deactivate();
  
  // activate reset button
  buttons[ResetIndex].draw("RESET", VGA_LIME, VGA_LIME, VGA_WHITE);
  regions[ResetIndex].activate();
  
  // activate temperature monitor region
  controls[TempControlIndex].activate();
  
  return WARMUP;
}

// return to the initial State, turn of all relays and timers, 
State resetTransition(State origin, SelectValueRegion* regions, ButtonRegion* buttons, PinControl *controls)
{
  // draw the screen features
  regions[TempIndex].draw("TEMPERATURE", VGA_LIME, VGA_TEAL, VGA_WHITE);
  regions[TempIndex].activate();

  regions[PressureIndex].draw("PRESSURE", VGA_OLIVE, VGA_TEAL, VGA_WHITE);
  regions[PressureIndex].deactivate();
  regions[TimerIndex].draw("TIMER", VGA_OLIVE, VGA_TEAL, VGA_WHITE);
  regions[TimerIndex].deactivate();

  buttons[WarmUpIndex].draw("WARM UP", VGA_LIME, VGA_LIME, VGA_WHITE);
  buttons[WarmUpIndex].activate();

  buttons[SetPressureTimerIndex].draw("SET", VGA_OLIVE, VGA_OLIVE, VGA_WHITE);
  buttons[SetPressureTimerIndex].deactivate();
  buttons[RunIndex].draw("GO!!", VGA_OLIVE, VGA_OLIVE, VGA_WHITE);
  buttons[RunIndex].deactivate();
  buttons[ResetIndex].draw("RESET", VGA_OLIVE, VGA_OLIVE, VGA_WHITE);
  buttons[ResetIndex].deactivate();

  // Disable Controls
  controls[TempControlIndex].resetPin();
  controls[TempControlIndex].deactivate();
  controls[PressureControlIndex].resetPin();
  controls[PressureControlIndex].deactivate();
  
  return INIT;
}

// At temperature set point.  monitor temp
State setPointReached(State origin, SelectValueRegion* regions, ButtonRegion* buttons, PinControl *controls)
{
  // Turn off temp? yes, but thermostat controls it.
  return TEMPSETPOINT;
}

State timerAndPressureSet(State origin, SelectValueRegion* regions, ButtonRegion* buttons, PinControl *controls)
{
  // draw the screen features
  regions[TempIndex].draw("TEMPERATURE", VGA_OLIVE, VGA_TEAL, VGA_WHITE);
  regions[TempIndex].deactivate();
  regions[PressureIndex].draw("PRESSURE", VGA_OLIVE, VGA_TEAL, VGA_WHITE);
  regions[PressureIndex].deactivate();
  regions[TimerIndex].draw("TIMER", VGA_OLIVE, VGA_TEAL, VGA_WHITE);
  regions[TimerIndex].deactivate();

  buttons[WarmUpIndex].draw("WARM UP", VGA_OLIVE, VGA_OLIVE, VGA_WHITE);
  buttons[WarmUpIndex].deactivate();
  buttons[SetPressureTimerIndex].draw("SET", VGA_OLIVE, VGA_OLIVE, VGA_WHITE);
  buttons[SetPressureTimerIndex].deactivate();
  buttons[RunIndex].draw("GO!!", VGA_GREEN, VGA_GREEN, VGA_WHITE);
  buttons[RunIndex].activate();
  buttons[ResetIndex].draw("RESET", VGA_OLIVE, VGA_OLIVE, VGA_WHITE);
  buttons[ResetIndex].activate();

  // activate the pressure control
  controls[PressureControlIndex].activate();
  
  return READY;
}

State runProcess(State origin, SelectValueRegion* regions, ButtonRegion* buttons, PinControl *controls)
{
  // set Timer
  // enable Reset
  return RUNNING;
}




