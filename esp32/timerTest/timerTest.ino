#include "src/basicTimer.h"

basicTimer timer;

void setup()
{
  timer.init();
  Serial.begin(115200);
}

void loop()
{
  delay(6000);
  log_v("starting test");
  timer.start(5000);
  while(!timer.expired()){
  }
  log_v("timer expired- not cleared");
  Serial.println(timer.expired());
  timer.start(10000);
  log_v("timer now cleared");
  Serial.println(timer.expired());
  
  while(!timer.expired()){
    timer.start(1);
  }
  log_v("timer expired");
  Serial.println(timer.expired()); // this should be 1
  Serial.println(timer.expired()); // this should be 1
  Serial.println(timer.expired()); // this should be 1
  timer.clear();
  Serial.println(timer.expired()); // this should be 0 now
  timer.start(10000);
  timer.stop();
  Serial.println(timer.expired()); // this should be 0 now
  timer.start(10);
  delay(100);
  timer.stop();
  Serial.println(timer.expired()); // this should be 1 now
  timer.clear();
  Serial.println(timer.expired()); // this should be 0 now
//  timer.start(10000);
  
  while(1){
    
  }
  
  
}
