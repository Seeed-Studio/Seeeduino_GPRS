/*
Sketch: Bluetooth Communication

Function: This sketch is used to test SIM800's Bluetooth function,you can connect
your mobile phone with SIM800 module in SPP profile. then you can send 
data to SIM800 or receive data from SIM800
*********************************************************************************
note: the following pins has been used and should not be used for other purposes.
  pin 8   // tx pin
  pin 7   // rx pin
  pin 9   // power key pin
  pin 12  // power status pin
*********************************************************************************

this code has not been test because I have no Bluetooth with SPP 
Profile, I will test it later!

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/

#include <bluetooth.h>
#include <SoftwareSerial.h>

#define DEFAULT_TIMEOUT 5
#define TARGET_DEVICE_NAME  "YOUR_BLUETOOTH_DEVICE_NAME"

BlueTooth bluetooth;
int bluetoothID = 0;
char recv[16];
int start = 0;
    
void setup() {
  Serial.begin(9600);
  Serial.println("Bluetooth Communication Test...");
  bluetooth.preInit();
  delay(3*1000);
  while(0 != bluetooth.powerOn()){ //bluetooth PowerOn
    Serial.println("bluetooth power on failed,try again...");
    delay(2000);
  }
  bluetoothID = bluetooth.scanForTargetDevice(TARGET_DEVICE_NAME);
  
  if(bluetoothID != 0){
    bluetooth.sendPairingReqstToDevice(bluetoothID);  
    bluetooth.connectInSPP(bluetoothID);
  }
}

void loop() {
  if(0 == bluetooth.recvInSPP(recv)){
    Serial.print(recv);
    bluetooth.sendInSPP("\r\nReceive OK\r\n");
  }
}
