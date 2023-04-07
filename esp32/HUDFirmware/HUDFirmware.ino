#include "src/stateMachine.h"
#include "src/screen.h"
#include "src/measurement.h"
#include "src/basicTimer.h"
#include "src/button.h"
#include "src/storage.h"
#include "src/battery.h"
#include "src/connectivity.h"

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
  initBattery();
}

int heading;
float depth;
bool cal;
int vbatt;

void loop() {

//set state machine behavior
switch (currentState){
  case modeSleep:
    //if the case is sleep and we are not asleep...we should go to sleep
    oled.powerDown();
    measSys.powerDown();
    stopServer();
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

    case modeChg:
    if(newState()){
      timer.stop();
      timer.start(60000);
      stopServer();
    }
    oled.display_ep_mode(isCharging(),0);
    break;

    case modeAP:
    if(newState()){
      timer.stop();
      timer.start(60000);
      startServer();
    }
    oled.display_ep_mode(isCharging(),1);
    break;

    case modeServer:
    oled.display_ep_mode(isCharging(),2);
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
if(extPow())
{
  stateMachineUpdate(onExtpowConn);
}
if(!extPow() && (currentState == modeChg || currentState == modeAP || currentState == modeServer))
{
  stateMachineUpdate(onExtpowDC);
}
if(currentState == modeAP && hasConnection()){
  stateMachineUpdate(onWifiConnect);
}
if(currentState == modeServer && !hasConnection()){
  stateMachineUpdate(onWifiDisconnect);
}

if(!extPow()){
vbatt = measBatt();
Serial.println(vbatt);
}

}
