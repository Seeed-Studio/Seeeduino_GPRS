/*
    Sketch: GPRS Connect TCP

    Function: This sketch is used to test seeeduino GPRS's TCP Connect function.to make
    it work, you should insert SIM card to Seeeduino GPRS and replace the phoneNumber.
    besides this, we upload sensor value to xively,so you have to be familiar with xively. enjoy it!
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

#define IP_ADDRSS       "216.52.233.120" //api.xively.com
#define PORT            80 //port addr
#define FEED_ID         "YOUR FEED ID"
#define SENSOR_ID       "YOUR SENSOR ID"
#define XIVELY_KEY      "YOUR XIVELY KEY"
#define REQUEST_LENGTH  36
#define DATA_LENGTH     90
#define HEAD_LEN        270
int soundSensor = A4;
GPRS gprs;

int putDataToXively(char* sensorValue) {
    char request[REQUEST_LENGTH];
    char dataStream[DATA_LENGTH];
    char head[HEAD_LEN];
    snprintf(request, REQUEST_LENGTH, "PUT /v2/feeds/%s HTTP/1.1\r\n", FEED_ID);
    snprintf(dataStream, DATA_LENGTH,
             "{\"version\":\"1.0.0\", \"datastreams\" : [{ \"id\" : \"%s\", \"current_value\" : \"%s\"}]}\r\n", SENSOR_ID,
             sensorValue);
    int dataLen = strlen(dataStream);
    snprintf(head, HEAD_LEN,
             "%sHost: api.xively.com\r\nX-ApiKey: %s\r\nUser-Agent: Xively-Arduino-Lib/1.0\r\nContent-Length: %d\r\n\r\n%s", request,
             XIVELY_KEY, dataLen, dataStream);
    if (0 != gprs.connectTCP(IP_ADDRSS, PORT)) {
        Serial.println("connect TCP error");
        goto STOP;
    }
    if (0 != gprs.sendTCPData(head)) {
        Serial.println("send TCP data error");
        goto STOP;
    }
STOP:
    gprs.closeTCP();
    return 0;
}

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("GPRS - TCP Connection Test...");
    pinMode(soundSensor, INPUT);
    gprs.preInit();
    while (0 != gprs.init()) {
        delay(1000);
        Serial.println("init error");
    }
    while (!gprs.join("cmnet")) { //change "cmnet" to your own APN
        Serial.println("gprs join network error");
        delay(2000);
    }
    // successful DHCP
    Serial.print("IP Address is ");
    Serial.println(gprs.getIPAddress());

    Serial.println("Init success, start to upload data to xively...");
}

void loop() {
    double value = analogRead(soundSensor);
    char sensorValue[6];
    dtostrf(value, 5, 2, sensorValue);
    putDataToXively(sensorValue);
    delay(1000);
    Serial.println("loop");
}
