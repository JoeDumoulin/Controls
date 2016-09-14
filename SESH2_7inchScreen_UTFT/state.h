#ifndef STATE_H
#define STATE_H

#include "Region.h"
#include "control.h"

enum State {
  INIT,
  WARMUP,
  TEMPSETPOINT,
  READY,
  PRESSURESETPOINT,
  RUNNING,
  RAMPDOWN
};

// This is a prototype for passing state transition functions.
typedef State (*StateTransition)(State Origin, SelectValueRegion *regions, ButtonRegion *buttons, PinControl *controls);

extern State warmUp(State origin, SelectValueRegion* regions, ButtonRegion* buttons, PinControl *controls);
extern State resetTransition(State origin, SelectValueRegion* regions, ButtonRegion* buttons, PinControl *controls);
extern State setPointReached(State origin, SelectValueRegion* regions, ButtonRegion* buttons, PinControl *controls);
extern State timerAndPressureSet(State origin, SelectValueRegion* regions, ButtonRegion* buttons, PinControl *controls);
extern State runProcess(State origin, SelectValueRegion* regions, ButtonRegion* buttons, PinControl *controls);



#endif // STATE_H
