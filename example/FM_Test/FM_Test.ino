/*
FM Test

This sketch is used to test SIM800'FM function.

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/

#include <gprs.h>
#include <SoftwareSerial.h>

int channelButton = 5; //used for changing channel
FM fm(8,7,9,19200);

void setup() {
  pinMode(channelButton,INPUT);
  Serial.begin(9600);
  fm.preInit();
  while(0 != fm.powerOn()){
    delay(2000);
  }
  fm.setVolume(6); //0,1,2,3,4,5,6
  fm.scanChannel();
}

void loop() {
  while(LOW == digitalRead(channelButton)){
    delay(50);
  }
  Serial.print("change Channel\r\n");
  fm.channelNext();
  delay(200);
}





