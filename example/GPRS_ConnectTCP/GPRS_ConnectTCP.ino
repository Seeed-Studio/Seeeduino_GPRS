/*
GPRS Connect TCP

This sketch is used to test seeeduino GPRS's TCP Connect function.
to make it work, you should insert SIM card to Seeeduino GPRS 
and replace the phoneNumber.besides this, we upload sensor value to xively,
so you have to be familiar with xively. enjoy it!

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/
#include <gprs.h>
#include <SoftwareSerial.h>

#define PHONE_NUMBER    "13925257382"
#define IP_ADDRSS       "216.52.233.120" //api.xively.com
#define PORT            80 //port addr
#define FEED_ID         "259586273"
#define SENSOR_ID       "LED"
#define XIVELY_KEY      "d3V6YQek8iBSxoFg0xZYkZBhpBhXTH485XFY0ubPqZIvm0e8"
#define REQUEST_LENGTH  36
#define DATA_LENGTH     90
#define HEAD_LEN        270
int soundSensor = A4;
GPRS gprsTest(8,7,9,9600,PHONE_NUMBER); //TX,RX,PWR,BaudRate,PhoneNumber

int putDataToXively(char* sensorValue)
{
  char request[REQUEST_LENGTH];
  char dataStream[DATA_LENGTH];
  char head[HEAD_LEN];
  snprintf(request,REQUEST_LENGTH,"PUT /v2/feeds/%s HTTP/1.1\r\n",FEED_ID);
  snprintf(dataStream,DATA_LENGTH,"{\"version\":\"1.0.0\", \"datastreams\" : [{ \"id\" : \"%s\", \"current_value\" : \"%s\"}]}\r\n",SENSOR_ID,sensorValue);
  int dataLen = strlen(dataStream);
  snprintf(head,HEAD_LEN,"%sHost: api.xively.com\r\nX-ApiKey: %s\r\nUser-Agent: Xively-Arduino-Lib/1.0\r\nContent-Length: %d\r\n\r\n%s",request,XIVELY_KEY,dataLen,dataStream);
  if(0 != gprsTest.connectTCP(IP_ADDRSS,PORT)){
    Serial.println("connect TCP error");
    goto STOP;
  }
  if(0 != gprsTest.sendTCPData(head)){
    Serial.println("send TCP data error");
    goto STOP;
  }
STOP:
#if 0
  while(gprsTest.serialSIM800.available()){
     gprsTest.serialSIM800.read();
  }
#endif
  gprsTest.closeTCP();
  return 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(soundSensor,INPUT);
  gprsTest.preInit();
  while(0 != gprsTest.init()) {
     delay(1000);
     Serial.println("init error");
  }  
}

void loop() {
  double value = analogRead(soundSensor);
  char sensorValue[6];
  dtostrf(value,5,2,sensorValue);
  putDataToXively(sensorValue);
  delay(1000);
  Serial.println("loop");
}


