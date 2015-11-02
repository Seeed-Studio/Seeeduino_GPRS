/*
Sketch: Bluetooth AT Command 

Function: This sketch is used to test SIM800's Bluetooth function,you can connect
your mobile phone with SIM800 module in SPP profile. then you can send AT command
to control SIM800' Bluetooth.
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

#include <bluetooth.h>
#include <SoftwareSerial.h>

#define DEFAULT_TIMEOUT 5
#define BT_BUF_LEN      32
BlueTooth bluetooth;

char bluetoothBuffer[BT_BUF_LEN];
int start = 0;
    
void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Bluetooth AT Command Test...");
  bluetooth.preInit();
  delay(3*1000);
  while(0 != bluetooth.powerOn()){ //bluetooth PowerOn
    Serial.println("bluetooth power on failed, try again...");
    delay(2000);
  }
}

void loop() {
  if(bluetooth.serialSIM800.available()) {
    start = 1;
  }else{
    delay(500);
  }

  if(start){
    //bluetooth.cleanBuffer(bluetoothBuffer,64);
    bluetooth.readBuffer(bluetoothBuffer,BT_BUF_LEN,DEFAULT_TIMEOUT);
    if(NULL != strstr(bluetoothBuffer,"+BTPAIRING:")){
      bluetooth.acceptPairing();
    }
    if(NULL != strstr(bluetoothBuffer,"+BTCONNECTING:")){
      bluetooth.acceptConnect();
    }
    start = 0;
   }
}
