/*
GPRS Loop Handle

This sketch is used to test seeeduino GPRS's call answering and 
reading SMS function.to make it work, you should insert SIM card
to Seeeduino GPRS and replace the phoneNumber,enjoy it!

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/
#include <gprs.h>
#include <SoftwareSerial.h>
#include <stdio.h>

char gprsBuffer[64];
int i = 0;
char *s = NULL;
int inComing = 0;

GPRS gprsTest(8,7,9,9600,"150****9566");//TX,RX,PWR,BaudRate,PhoneNumber

void setup() {
  Serial.begin(9600);
  gprsTest.preInit();
  while(0 != gprsTest.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  }  
}

void loop() {
   if(gprsTest.serialSIM800.available()) {
       inComing = 1;
   }else{
       delay(100);
   }
   
   if(inComing){
      gprsTest.readBuffer(gprsBuffer,32,DEFAULT_TIMEOUT);
      Serial.print(gprsBuffer);
      
      if(NULL != strstr(gprsBuffer,"RING")) {
          gprsTest.answer();
      }else if(NULL != (s = strstr(gprsBuffer,"+CMTI: \"SM\""))) { //SMS: $$+CMTI: "SM",24$$
          char message[MESSAGE_LENGTH];
          int messageIndex = atoi(s+12);
          gprsTest.readSMS(messageIndex, message,MESSAGE_LENGTH);
          Serial.print(message);
     }
     gprsTest.cleanBuffer(gprsBuffer,32);  
     inComing = 0;
   }
}
