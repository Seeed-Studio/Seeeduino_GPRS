/*
* gprs.cpp 
* A library for SeeedStudio seeeduino GPRS shield 
*  
* Copyright (c) 2013 seeed technology inc. 
* Author      	: 	lawliet.zou(lawliet.zou@gmail.com)
* Create Time	: 	Dec 23, 2013 
* Change Log 	: 	
*
* The MIT License (MIT)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <stdio.h>
#include "gprs.h"

#define TRUE 					1
#define FALSE 					0

#define UART_DEBUG

#ifdef UART_DEBUG
#define ERROR(x)			Serial.print(x)
#define DEBUG(x)			Serial.print(x);
#else
#define ERROR(x)
#define DEBUG(x)
#endif

#define DEFAULT_TIMEOUT   	5

void SIM800::preInit(void)
{
#if 0
	delay(800);
	pinMode(powerPin,OUTPUT);
	digitalWrite(powerPin,LOW);
	delay(100);
	digitalWrite(powerPin,HIGH);
	delay(3000);
	digitalWrite(powerPin,LOW);
	delay(2200);
#endif

#if 1
	delay(800);
	digitalWrite(powerPin,HIGH);
	delay(200);
	digitalWrite(powerPin,LOW);
	delay(2000);
	digitalWrite(powerPin,HIGH);
	delay(3000); //delay 3s 
	//sim800.begin(9600);
	//sim800.print("AT\r\n");
#endif
}

int SIM800::readBuffer(char *buffer,int count, unsigned int timeOut)
{
    int i = 0;
	unsigned long timerStart,timerEnd;
	timerStart = millis();
    while(1) {
        while (sim800.available()) {
            char c = sim800.read();
            if (c == '\r' || c == '\n') c = '$';
            buffer[i++] = c;
            if(i > count-1)break;
        }
        if(i > count-1)break;
		timerEnd = millis();
        if(timerEnd - timerStart > 1000 * timeOut) {
            break;
        }
    }
    delay(500);
    while(sim800.available()) {	// display the other thing..
		sim800.read();
    }
    return 0;
}

void SIM800::cleanBuffer(char *buffer, int count)
{
    for(int i=0; i < count; i++) {
        buffer[i] = '\0';
    }
}

void SIM800::sendCmd(const char* cmd)
{
    sim800.write(cmd);
}

void SIM800::sendATTest(void)
{
	sendCmdAndWaitForResp("AT\r\n","OK",DEFAULT_TIMEOUT);
}

int SIM800::waitForResp(const char *resp, unsigned int timeout)
{
    int len = strlen(resp);
	int sum=0;
	unsigned long timerStart,timerEnd;
	timerStart = millis();
    
    while(1) {
        if(sim800.available()) {
            char c = sim800.read();
            sum = (c==resp[sum]) ? sum+1 : 0;
            if(sum == len)break;
        }
		timerEnd = millis();
        if(timerEnd - timerStart > 1000 * timeout) {	// time out
            ERROR("time out");
            return -1;
        }
    }

    while(sim800.available()) {  	// display the other thing..
		sim800.read();
    }

    return 0;
}

void SIM800::sendEndMark(void)
{
	sim800.println((char)26);//the ASCII code of the ctrl+z is 26
}


int SIM800::sendCmdAndWaitForResp(const char* cmd, const char *resp, unsigned timeout)
{
    sendCmd(cmd);
    return waitForResp(resp,timeout);
}

void SIM800::serialDebug(void)
{
    while(1) {
		if(sim800.available()){
			Serial.write(sim800.read());
		}
		if(Serial.available()){     
			sim800.write(Serial.read()); 
		}
    }
}


/*************************************************************************
 ***********************************GPRS**********************************
 *************************************************************************/
int GPRS::init(void)
{	
	for(int i = 0; i < 2; i++){
		sendCmd("AT\r\n");
		delay(100);
	}
    
	sendCmd("AT+CFUN=1\r\n");  //full function

    if(0 != checkSIMStatus()) { //check SIM card status
        ERROR("ERROR:checkSIMStatus\r\n");
        return -1;
    }else{
		DEBUG("checkSIMStatus OK\r\n");
	}
	delay(2000); //wait for GPRS getting Signal
    if(checkSignalStrength()<1) { //check Signal Strength
        ERROR("ERROR:Signal too weak\r\n");
        return -1;
    }else{
		DEBUG("checkSignalStrength OK\r\n");
	}

    return 0;
}

int GPRS::checkSIMStatus(void)
{
    char gprsBuffer[30];
    int count = 0;
    cleanBuffer(gprsBuffer,30);
    while(count < 3) {
        sendCmd("AT+CPIN?\r\n");
        readBuffer(gprsBuffer,30,DEFAULT_TIMEOUT);
        DEBUG(gprsBuffer);
        if((NULL != strstr(gprsBuffer,"+CPIN: READY"))) {
            break;
        }
        count++;
        delay(1000);
    }

    if(count == 3) {
        ERROR("Bad SIM Status");
        return -1;
    }
    return 0;
}

int GPRS::checkSignalStrength(void)
{
    char gprsBuffer[100];
    int index,count = 0;
    cleanBuffer(gprsBuffer,100);
    while(count < 3) {
        sendCmd("AT+CSQ\r\n");
        readBuffer(gprsBuffer,25,DEFAULT_TIMEOUT);
        DEBUG(gprsBuffer);
        if(sscanf(gprsBuffer, "AT+CSQ$$$+CSQ: %d", &index)>0) {
            break;
        }
        ERROR("checking signal strength error, try again...");
        count++;
        delay(1000);
    }
    if(count == 3) {
        ERROR("AT+CSQ");
        return -1;
    }
    return index;
}

int GPRS::networkCheck(void)
{
    //for GPRS
	delay(1000);
    if(0 != sendCmdAndWaitForResp("AT+CGREG?\r\n","+CGREG: 0,1",DEFAULT_TIMEOUT*3)) { 
        ERROR("CGREG");
        return -1;
    }
    delay(1000);
    if(0 != sendCmdAndWaitForResp("AT+CGATT?\r\n","+CGATT: 1",DEFAULT_TIMEOUT)) {
        ERROR("CGATT");
        return -1;
    }
    return 0;
}

int GPRS::sendSMS(char *number, char *data)
{
    char cmd[64];
    if(0 != sendCmdAndWaitForResp("AT+CMGF=1\r\n", "OK", DEFAULT_TIMEOUT)) { // Set message mode to ASCII
        ERROR("CMGF");
        return -1;
    }
    delay(500);
    // Set the phone number
    snprintf(cmd, sizeof(cmd),"AT+CMGS=\"%s\"\r\n", number);
	DEBUG(cmd);
    if(0 != sendCmdAndWaitForResp(cmd,">",DEFAULT_TIMEOUT)) {
        ERROR("CMGS");
        return -1;
    }
    delay(1000);
	DEBUG(data);
    sim800.write(data);// Send Message
    delay(500);
	sendEndMark();
	return 0;
}

int GPRS::readSMS(int messageIndex, char *message, bool newMessage)
{
	int i = 0;
    char gprsBuffer[100];
	char cmd[16];
    char *p,*s;
	
	sendCmdAndWaitForResp("AT+CMGF=1\r\n","OK",DEFAULT_TIMEOUT);
	delay(1000);
	
	sprintf(cmd,"AT+CMGR=%d\r\n",messageIndex);
	DEBUG(cmd);
	sim800.write(cmd);
    cleanBuffer(gprsBuffer,100);
    readBuffer(gprsBuffer,100,DEFAULT_TIMEOUT);
    //pc.printf(gprsBuffer); //AT+CMGR=17$$$$+CMGR: "REC UNREAD","+8613925257382","","13/11/11,10:36:13+32"$$Hello$$$$OK$$
	DEBUG(gprsBuffer);
	DEBUG("\r\nDEBUG OVER\r\n");
	if(newMessage){
		if(NULL == ( s = strstr(gprsBuffer,"+CMGR: \"REC UNREAD\""))) {
			ERROR("\r\n get CMGR error\r\n");
			return -1;
		}
		p = s + 64;
	} else {
		if(NULL == ( s = strstr(gprsBuffer,"+CMGR: \"REC READ\""))) {
			ERROR("\r\n get CMGR error\r\n");
			return -1;
		}
		p = s + 62;
	}
	
    while(*p != '$') {
        message[i++] = *(p++);
    }
    message[i] = '\0';
	
    return 0;
}

int GPRS::deleteSMS(int index)
{
    char cmd[64];
    snprintf(cmd,sizeof(cmd),"AT+CMGD=%d\r\n",index);
    sendCmd(cmd);
    return 0;
}

int GPRS::callUp(char *number)
{
	char cmd[30];
    if(0 != sendCmdAndWaitForResp("AT+COLP=1\r\n","OK",5)) {
        ERROR("COLP");
        return -1;
    }
    delay(1000);
	sprintf(cmd,"\r\nATD%s;\r\n", number);
	sim800.write(cmd);
    return 0;
}

int GPRS::answer(void)
{
    sim800.write("ATA\r\n");
    return 0;
}

/****************************************GPRS TCP CONNECT************************************/

int GPRS::connectTCP(char *ip, char *port)
{
    char cipstart[50];
    char ipaddr[20];

    if(0 != sendCmdAndWaitForResp("AT+CSTT\r\n","OK",20)) {
        ERROR("\r\nERROR:CSTT\r\n");
        return -1;
    }
    delay(1000);
    if(0 != sendCmdAndWaitForResp("AT+CIICR\r\n","OK",10)) {
        ERROR("\r\nERROR:CIICR\r\n");
        return -1;
    }
    delay(2000);
    sendCmd("AT+CIFSR\r\n");
    readBuffer(ipaddr,20,DEFAULT_TIMEOUT);
    delay(1000);
    sprintf(cipstart, "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r\n", ip, port);
    //DEBUG(cipstart);
    if(0 != sendCmdAndWaitForResp(cipstart, "CONNECT OK", 10)) {	// connect tcp
        ERROR("\r\nERROR:CIPSTART\r\n");
        return -1;
    }

    return 0;
}
int GPRS::sendTCPData(char *data)
{
    char cmd[64];
    int len = strlen(data);
    snprintf(cmd,sizeof(cmd),"AT+CIPSEND=%d\r\n",len);
    DEBUG(cmd);
    if(0 != sendCmdAndWaitForResp(cmd,">",DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR:CIPSEND\r\n");
        return -1;
    }
    if(0 != sendCmdAndWaitForResp(data,"SEND OK",DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR:SendTCPData\r\n");
        return -1;
    }
    return 0;
}

int GPRS::closeTCP(void)
{
    sendCmd("AT+CIPCLOSE\r\n");
    return 0;
}

int GPRS::shutTCP(void)
{
    sendCmd("AT+CIPSHUT\r\n");
    return 0;
}

/********************************************************************************************
 *************************************BlueTooth**********************************************
 ********************************************************************************************/
int Bluetooth::powerOn(void)
{
	sendATTest();
	sendATTest();
	if(0 == bluetoothPower){
		if(0 != sendCmdAndWaitForResp("AT+BTPOWER=1\r\n", "OK", DEFAULT_TIMEOUT)) {
			ERROR("\r\nERROR:bluetoothPowerOn\r\n");
			return -1;
		}else{
			DEBUG("\r\nBluetooth Power On Success\r\n");
			bluetoothPower = 1;
		}
    }
	return 0;
}

int Bluetooth::powerOff(void)
{
	if(1 == bluetoothPower){
		if(0 != sendCmdAndWaitForResp("AT+BTPOWER=0\r\n", "OK", DEFAULT_TIMEOUT)){
			ERROR("\r\nERROR:bluetoothPowerOff\r\n");
			return -1;
		}else {
			DEBUG("\r\nBluetooth Power Off Success\r\n");
			bluetoothPower = 0;
		}
    }
	return 0;
}

int Bluetooth::getHostDeviceName(char* deviceName)
{
	char gprsBuffer[40];
	char *s,*p;
	int i = 0;
	sendCmd("AT+BTHOST?\r\n");
	readBuffer(gprsBuffer,40,DEFAULT_TIMEOUT);
	if(NULL == (s = strstr(gprsBuffer,"+BTHOST:"))) {
        ERROR("\r\nERROR: get host device name error\r\n");
        return -1;
    }
	p = s + 9;//+BTHOST: SIM800H,33:7d:77:18:62:60
	while(*(p) != ','){
		deviceName[i++] = *p;
		p++;
	}
	deviceName[i] = '\0';
	return i;
}

int Bluetooth::scanForTargetDevice(char* deviceName)
{
	char gprsBuffer[100];
	char *s;
	sendCmd("AT+BTSCAN=1,20\r\n"); //scan 20s
	cleanBuffer(gprsBuffer,100);
	readBuffer(gprsBuffer,100,20);//+BTPAIR: 1,"ME863",5c:6b:32:91:00:d1
	DEBUG(gprsBuffer);
	if(NULL == (s = strstr(gprsBuffer,deviceName))) {
        ERROR("\r\nERROR: scan For Target Device error\r\n");
        return -1;
    }
	targetDeviceID = atoi(s-3);
	//DEBUG(targetDeviceID);
	return targetDeviceID;
}

int Bluetooth::sendPairingReqstToDevice(int deviceID)
{
	char cmd[30];
	if(0 == deviceID)
		return -1;
	sprintf(cmd,"AT+BTPAIR=0,%d\r\n",deviceID);
	sendCmd(cmd);
	return 0;
}

int Bluetooth::unPair(void)
{
	char cmd[30];
	if(0 == targetDeviceID)
		return -1;
	sprintf(cmd,"AT+BTUNPAIR=%d\r\n",targetDeviceID);
	if(0 != sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT)){
		ERROR("\r\nERROR: AT+BTUNPAIR\r\n");
		return -1;
	}
	return 0;
}

int Bluetooth::acceptPairing(void)
{
	if(0 != sendCmdAndWaitForResp("AT+BTPAIR=1,1\r\n", "OK", DEFAULT_TIMEOUT)){
		ERROR("\r\nERROR:AT+BTPAIR\r\n");
		return -1;
	}
	return 0;
}

int Bluetooth::acceptConnect(void)
{
	if(0 != sendCmdAndWaitForResp("AT+BTACPT=1\r\n", "OK", DEFAULT_TIMEOUT)){
		ERROR("\r\nERROR:AT+BTACPT\r\n");
		return -1;
	}
	return 0;
}

int Bluetooth::disconnect(int deviceID)
{
	char cmd[30];
	if(0 == targetDeviceID)
		return -1;
	sprintf(cmd,"AT+BTDISCONN=%d\r\n",targetDeviceID);
	if(0 != sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT)){
		ERROR("\r\nERROR: AT+BTDISCONN\r\n");
		return -1;
	}
	return 0;
}

int Bluetooth::loopHandle(void)
{
    char gprsBuffer[100];
    cleanBuffer(gprsBuffer,100);
    while(1) {
        if(sim800.available()) {
            break;
        }
        delay(1000);
    }
	readBuffer(gprsBuffer,100,DEFAULT_TIMEOUT);
	DEBUG(gprsBuffer);
	
	if(NULL != strstr(gprsBuffer,"+BTPAIRING:")){
		if(0 != acceptPairing()){
			return -1;
			ERROR("\r\nERROR:bluetoothAcceptPairing\r\n");
		}
	}if((NULL != strstr(gprsBuffer,"+BTCONNECTING:")) && (NULL != strstr(gprsBuffer,"SPP"))){
		if(0 != acceptConnect()){
			return -1;
			ERROR("\r\nERROR:bluetoothAcceptConnecting\r\n");
		}
	}
	return 0;
}


int Bluetooth::connectInSPP(int deviceID) //Serial Port Profile
{
	char gprsBuffer[200];
	char cmd[20];
	char *s;
	sprintf(cmd,"AT+BTGETPROF=%d\r\n",deviceID);
	sendCmd(cmd);
    readBuffer(gprsBuffer,200,DEFAULT_TIMEOUT);
	if(NULL == ( s = strstr(gprsBuffer,"\"SPP\""))) {
        ERROR("\r\nERROR: No SPP Profile\r\n");
        return -1;
    }
	cleanBuffer(cmd,20);
	sprintf(cmd,"AT+BTCONNECT=%d,%c\r\n",deviceID,*(s-2));
	if(0 != sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT)){
		ERROR("\r\nERROR:AT+BTCONNECT\r\n");
		return -1;
	}
	return 0;
}

int Bluetooth::recvInSPP(char* data)
{	
	char bluetoothBuffer[64];
	char * p = NULL;
	int count = 0;
	if(0 != sendCmdAndWaitForResp("AT+BTSPPGET=0\r\n", "OK", DEFAULT_TIMEOUT)){
		ERROR("\r\nERROR:AT+BTSPPGET\r\n");
		return -1;
	}
	while(!sim800.available()){
		delay(200);
	}
	readBuffer(bluetoothBuffer,64,DEFAULT_TIMEOUT);
	
	p = strstr(bluetoothBuffer,"+BTSPPDATA:");
	if(NULL != p){
		p += 11;
		while(count != 2){
			if(*(p++) == ','){
				count++;
			}
		}
		int i = 0;
		while(*(p++) != '$'){
			data[i++] = *p;
		}
    }else{
		ERROR("\r\nERROR: get data error\r\n");
		return -1;
	}
	return 0;
}

int Bluetooth::sendInSPP(char* data)
{
	if(0 != sendCmdAndWaitForResp("AT+BTSPPSEND\r\n", ">", DEFAULT_TIMEOUT)){
		ERROR("\r\nERROR:AT+BTSPPSEND\r\n");
		return -1;
	}
	sendCmd(data);
	sendEndMark();
	return 0;
}

/***************************************************************************************
 ****************************************FM*********************************************
 ***************************************************************************************/
int FM::powerOn(void)
{
	if(0 == fmPower){
		if(0 != sendCmdAndWaitForResp("AT+FMOPEN=1\r\n", "OK", DEFAULT_TIMEOUT)) {	// connect tcp
			ERROR("\r\nERROR:fmPowerOn\r\n");
			return -1;
		}else{
			DEBUG("FM Power On\r\n");
		}
		fmPower = 1;
	}
	
	return 0;
}

int FM::powerOff(void)
{
	if(1 == fmPower){
		if(0 != sendCmdAndWaitForResp("AT+FMCLOSE\r\n", "OK", DEFAULT_TIMEOUT)) {
			ERROR("\r\nERROR:fmPowerOff\r\n");
			return -1;
		}
    }
	fmPower = 0;
	return 0;
}

int FM::setFreq(int freq)
{
	char cmd[30];
	if(!fmPower)
		return -1;
		
	if(freq >= 875 && freq <= 1080){ //87.5MHZ~108.0MHZ
		sprintf(cmd,"AT+FMFREQ=%d\r\n",freq);
		DEBUG(cmd);
		if(0 != sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT)){
			ERROR("\r\nERROR:fmSetFreq\r\n");
			return -1;
		}
		
		if(0 != sendCmdAndWaitForResp("AT+CHFA=0\r\n", "OK", DEFAULT_TIMEOUT)){
			ERROR("\r\nERROR:AT+CHFA\r\n");
			return -1;
		}
	}
	return 0;
}

int FM::setVolume(int volume)
{
	char cmd[30];
	if(volume >= 0 && volume <=6){
		sprintf(cmd,"AT+FMVOLUME=%d\r\n",volume);
		if(0 != sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT)){
			ERROR("\r\nERROR:fmSetVolume\r\n");
			return -1;
		}else{
			DEBUG("set Volume Success\r\n");
		}
	}
	return 0;
}

int FM::scanChannel(void)
{
	char gprsBuffer[128];
	char tmp[5];
	int j = 0;
	if(!fmPower)
		return -1;
		
	fmChannelCount = 0;
	fmCurrentChannel = 0;
	cleanBuffer(gprsBuffer,128);
	cleanBuffer(tmp,5);
	sendCmd("AT+FMSCAN\r\n");
	readBuffer(gprsBuffer,128,DEFAULT_TIMEOUT*10); //need more time to scan channel
	DEBUG(gprsBuffer); //AT+FMSCAN$$$898$$$$1012$$$$1043$$$$OK$$
	for(int i = 0; i < 128; i++){
		if(gprsBuffer[i] >= '0' && gprsBuffer[i] <= '9'){
			tmp[j++] = gprsBuffer[i];
		}else{
			if(j){
				int channelFreq = atoi(tmp);
				if(channelFreq <= 1080 && channelFreq >= 875){
					fmChannel[fmChannelCount++] =  channelFreq;
					j = 0;
					cleanBuffer(tmp,5);
				}
			}
		}
	}
	if(fmChannelCount == 0)
		ERROR("\r\nWARNING: no Channel!\r\n");
	return 0;
}

int FM::checkSignalStrength(int channelFreq)
{
	char cmd[30];
	char gprsBuffer[30];
	int signalStrength;
	if(!fmPower)
		return -1;
	
	sprintf(cmd,"AT+FMSIGNAL=%d\r\n",channelFreq);
	sendCmd(cmd);
	readBuffer(gprsBuffer,sizeof(gprsBuffer),DEFAULT_TIMEOUT);
	if(sscanf(gprsBuffer, "$$+FMSIGNAL: freq[%d]:%d",&channelFreq, &signalStrength) > 0) {
		if(signalStrength >= 0 && signalStrength <= 112){
			return signalStrength;
		}
	}
	return -1;
}

int FM::channelNext(void)
{
	if(fmChannelCount == 0)
		return -1;
	if(fmCurrentChannel == fmChannelCount)
		fmCurrentChannel = 0;
	setFreq(fmChannel[fmCurrentChannel++]);
	return 0;
}
