/*
Sketch: SIM800 Serial Debug 

Function: This sketch is used to communicate with SIM800 with AT commands.
*********************************************************************************
note: the following pins has been used and should not be used for other purposes.
  pin 8   // tx pin
  pin 7   // rx pin
  pin 9   // power key pin
  pin 12  // power status pin
*********************************************************************************
created on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/

#include "gprs.h"
#include <SoftwareSerial.h>

GPRS gprs(9600);//BaudRate

void setup(){
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial debug begin...");
  gprs.preInit();//Power on SIM800
  while(0 != gprs.sendATTest())
  {
      Serial.println("sim800 init error!");
  }
  Serial.println("sim800 init O.K!");
}

void loop(){
  gprs.serialDebug();
}
