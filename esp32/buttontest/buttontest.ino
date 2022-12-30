#include "src/button.h"

void setup()
{
  Serial.begin(115200);
  initButton();
}

void loop()
{
//  for(int i = 0 ; i<30; i++){
  delay(1);
  if(readButton()==singleTap){
  Serial.println("read");
//  Serial.println(i);
  }
//  }
//  enterSleep();
  
}
