#include "src/storage.h"

void setup() {
  delay(1000);
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  log_v("Logging started");
  initLog();
}

int oldtime;

void loop() {
  eraseLog();
  startNewLog();
  
  for(int i=0; i<100; i++)
  {
  oldtime=millis();
  doLogTask(10,10);
  oldtime = oldtime-millis();
  log_v("time elapsed %d",oldtime);
  delay(1000);
  }
  endLog();
  printLog();
  eraseLog();
  startNewLog();
  
  for(int i=0; i<240; i++)
  {
  oldtime=millis();
  doLogTask(10,10);
  oldtime = oldtime-millis();
  log_v("time elapsed %d",oldtime);
  delay(1);
  }
  endLog();
  printLog();
  while(1){
  //do nothing
  }

}
