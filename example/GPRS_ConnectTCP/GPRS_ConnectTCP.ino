/*
GPRS Connect TCP

This sketch is used to test seeeduino GPRS's TCP Connect function.
to make it work, you should insert SIM card to Seeeduino GPRS 
and replace the phoneNumber.besides this, you have to define the 
ip address and port address that you want to communicate with. enjoy it!

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/
#include <gprs.h>
#include <SoftwareSerial.h>

#define IP_ADDRSS     "42.96.164.52" //ip address that you want to communicate with
#define PORT          "80" //port addr

GPRS gprsTest(8,7,9,9600,"139****382"); //TX,RX,PWR,BaudRate,PhoneNumber

void setup() {
  Serial.begin(9600);
  gprsTest.preInit();
  while(0 != gprsTest.init()) {
     delay(1000);
     Serial.print("init error\r\n");
  }  
}

void loop() {
  gprsTest.connectTCP(IP_ADDRSS,PORT); //connect TCP
  gprsTest.sendTCPData("hello,world\n"); //send data to Server
  gprsTest.closeTCP();//close connection
  while(1){delay(1000);}
}
