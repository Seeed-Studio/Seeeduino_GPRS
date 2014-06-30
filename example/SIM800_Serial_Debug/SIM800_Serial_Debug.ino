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
create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/

#include <sim800.h>
#include <SoftwareSerial.h>

SIM800 sim800(9600);//BaudRate

void setup(){
  Serial.begin(9600);
  sim800.preInit();//Power on SIM800
}

void loop(){
  sim800.serialDebug();
}



