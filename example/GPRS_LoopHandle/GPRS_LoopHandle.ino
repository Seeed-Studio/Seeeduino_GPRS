/*
Sketch: GPRS Loop Handle

Function: This sketch is used to test seeeduino GPRS's call answering and reading
SMS function.to make it work, you should insert SIM card to Seeeduino GPRS and 
replace the phoneNumber,enjoy it!
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
#include <gprs.h>
#include <SoftwareSerial.h>
#include <stdio.h>

char gprsBuffer[64];
int i = 0;
char *s = NULL;
int inComing = 0;

GPRS gprs;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("GPRS - LoopHandle Test...");
  gprs.preInit();
  while(0 != gprs.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  }  
  Serial.println("Init success, start to monitor your call or message...");
}

void loop() {
   if(gprs.serialSIM800.available()) {
       inComing = 1;
   }else{
       delay(100);
   }
   
   if(inComing){
      gprs.readBuffer(gprsBuffer,32,DEFAULT_TIMEOUT);
      Serial.print(gprsBuffer);
      
      if(NULL != strstr(gprsBuffer,"RING")) {
          gprs.answer();
      }else if(NULL != (s = strstr(gprsBuffer,"+CMTI: \"SM\""))) { //SMS: $$+CMTI: "SM",24$$
          char message[MESSAGE_LENGTH];
          int messageIndex = atoi(s+12);
          gprs.readSMS(messageIndex, message,MESSAGE_LENGTH);
          Serial.print(message);
     }
     gprs.cleanBuffer(gprsBuffer,32);  
     inComing = 0;
   }
}
