/*
GPRS Connect TCP

This sketch is used to test seeeduino GPRS's send SMS func.
to make it work, you should insert SIM card to Seeeduino GPRS 
and replace the phoneNumber,enjoy it!

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/
#include <gprs.h>
#include <SoftwareSerial.h>

GPRS gprsTest(8,7,9,9600,"150****9566");//TX,RX,PWR,BaudRate,PhoneNumber

void setup() {
  Serial.begin(9600);
  gprsTest.preInit();
  delay(1000);
  while(0 != gprsTest.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  }  
  gprsTest.sendSMS("150****9566","hello,world"); //define phone number and text
}

void loop() {
  //nothing to do
}
