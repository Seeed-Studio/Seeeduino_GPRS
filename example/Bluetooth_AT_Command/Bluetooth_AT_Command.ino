/*
Bluetooth AT Command 

This sketch is used to test SIM800's Bluetooth function,you can connect
your mobile phone with SIM800 module in SPP profile. then you can send 
AT command to control SIM800' Bluetooth.

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/
#include <gprs.h>
#include <SoftwareSerial.h>

#define DEFAULT_TIMEOUT 5

Bluetooth bluetooth(8,7,9,9600); //TX,RX,PWR,BaudRate

char bluetoothBuffer[64];
int start = 0;
    
void setup() {
  Serial.begin(9600);
  bluetooth.preInit();
  delay(3*1000);
  while(0 != bluetooth.powerOn()){ //bluetooth PowerOn
    delay(2000);
  }
}

void loop() {
  if(bluetooth.sim800.available()) {
    start = 1;
  }else{
    delay(500);
    Serial.print("loop\r\n");
  }

  if(start){
    //bluetooth.cleanBuffer(bluetoothBuffer,64);
    bluetooth.readBuffer(bluetoothBuffer,64,DEFAULT_TIMEOUT);
    if(NULL != strstr(bluetoothBuffer,"+BTCONNECTING:")){
      bluetooth.acceptConnect();
    }
    start = 0;
   }
}
