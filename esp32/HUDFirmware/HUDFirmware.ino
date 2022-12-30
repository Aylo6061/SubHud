#include "src/stateMachine.h"
#include "src/screen.h"
#include "src/measurement.h"
#include "src/basicTimer.h"
#include "src/button.h"
#include "src/storage.h"

screen oled;
measure measSys;
basicTimer timer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
  log_v("Logging started");
  oled.begin();
  measSys.begin();
  measSys.startMeasurementTask();
  timer.init();
  initButton();
  initLog();
}

int heading;
float depth;
bool cal;

void loop() {

//set state machine behavior
switch (currentState){
  case modeSleep:
    //if the case is sleep and we are not asleep...we should go to sleep
    enterSleep();
    break;
  case modeWake:
    if(newState()){
      timer.stop();
      timer.start(5000);
    }
    oled.display_wake_mode();
    break;
    
  case modeSurf:
    if(newState()){
      timer.stop();
      timer.start(60000);
    }
    measSys.measurementTask();
    measSys.exportMeasurement(&heading, &depth, &cal);
    oled.display_surf_mode(heading ,depth ,cal);
    break;
    
  case modeStartDive:
    if(newState()){
      timer.stop();
      startNewLog();
    }
    stateMachineUpdate(onExit);
    break;
    
  case modeDiveA:
    if(newState()){
      timer.stop();
    }
    measSys.measurementTask();
    measSys.exportMeasurement(&heading, &depth, &cal);
    doLogTask(depth, 0);
    oled.display_dive_mode(heading ,depth ,cal);
    break;

    case modeDiveB:
    if(newState()){
      timer.stop();
    }
    measSys.measurementTask();
    measSys.exportMeasurement(&heading, &depth, &cal);
    doLogTask(depth, 0);
    oled.display_dive_mode(100 ,3 ,cal);
    break;

    case modeFinishDive:
    if(newState()){
      timer.stop();
      endLog();
      printLog();
    }
    stateMachineUpdate(onExit);
    break;
    
  default:
    log_e("fell through state switch");
    break;
}

//do state machine activity tasks (get data,display stuff)



//do state machine event generation tasks
if(timer.expired())
{
  timer.clear();
  stateMachineUpdate(onTimeout);
}
if(readButton()){
  stateMachineUpdate(onButton);
}
if(depth>.5 && currentState == modeSurf){
  stateMachineUpdate(onDive);
}
if(depth<.1 && currentState == modeDiveA){
  stateMachineUpdate(onSurface);
}

Serial.print(currentState);

delay(30);
}
