/*
* gprs.h 
* A library for SeeedStudio seeeduino GPRS shield 
*  
* Copyright (c) 2013 seeed technology inc. 
* Author  		: 	lawliet.zou(lawliet.zou@gmail.com)
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

#ifndef __GPRS_H__
#define __GPRS_H__

#include "Arduino.h"
#include <SoftwareSerial.h>

/** SIM800 class.
 *  Used for SIM800 communication. attention that SIM800 module communicate with MCU in serial protocol
 */
class SIM800
{

public:
	/**	Create SIM800 Instance 
     *  @param tx	uart transmit pin to communicate with SIM800
     *  @param rx	uart receive pin to communicate with SIM800
     *  @param baudRate	baud rate of uart communication
	 */
	SIM800(int tx, int rx, int power, int baudRate):sim800(tx,rx){
		powerPin = power;
		pinMode(powerPin,OUTPUT);
		sim800.begin(baudRate);
	};
	
	/** Power on SIM800
	 */
	void preInit(void);
	
	/** read from SIM800 module and save to buffer array
	 *  @param  buffer	buffer array to save what read from SIM800 module
	 *  @param  count 	the maximal bytes number read from SIM800 module
	 *  @param  timeOut	time to wait for reading from SIM800 module 
	 *  @returns
	 *      0 on success
	 *      -1 on error
	 */
	int readBuffer(char* buffer,int count, unsigned int timeOut);

	
	/** clean Buffer
	 *	@param buffer	buffer to clean
	 *	@param count	number of bytes to clean
	 */
	void cleanBuffer(char* buffer, int count);
	
	/** send AT command to SIM800 module
	 *  @param cmd	command array which will be send to GPRS module
	 */
	void sendCmd(const char* cmd);

	/**send "AT" to SIM800 module
	 */
	void sendATTest(void);
	
	/**send '0x1A' to SIM800 Module
	 */
	void sendEndMark(void);
	
	/** check SIM800 module response before time out
	 *  @param  *resp   correct response which SIM800 module will return
	 *  @param  *timeout    waiting seconds till timeout
	 *  @returns
	 *      0 on success
	 *      -1 on error
	 */ 
	int waitForResp(const char* resp, unsigned timeout);

	/** send AT command to GPRS module and wait for correct response
	 *  @param  *cmd 	AT command which will be send to GPRS module
	 *  @param  *resp   correct response which GPRS module will return
	 *  @param  *timeout 	waiting seconds till timeout
	 *  @returns
	 *      0 on success
	 *      -1 on error
	 */
	int sendCmdAndWaitForResp(const char* cmd, const char *resp, unsigned timeout);


	/** used for serial debug, you can specify tx and rx pin and then communicate with GPRS module with common AT commands
	 */
	void serialDebug(void);

	SoftwareSerial sim800;
		
private:
	int powerPin;
};
 

/** GPRS class.
 *	used to realize GPRS communication
 */ 
class GPRS : public SIM800
{
public:
    /** Create GPRS instance
     *  @param number default phone number during mobile communication
     */
    GPRS(int tx, int rx, int power, int baudRate, char* number):SIM800(tx,rx,power,baudRate){
        phoneNumber = number;
    };
	
    /** initialize GPRS module including SIM card check & signal strength
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int init(void);

    /** check SIM card' Status
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int checkSIMStatus(void);

    /** check signal strength
     *  @returns
     *      signal strength in number(ex 3,4,5,6,7,8...) on success
     *      -1 on error
     */
    int checkSignalStrength(void);

    /** check network is OK or not
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int networkCheck(void);

    /** send text SMS
     *  @param  *number	phone number which SMS will be send to
     *  @param  *data   message that will be send to
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int sendSMS(char* number, char* data);

    /** read SMS if getting a SMS message
     *  @param  buffer	buffer that get from GPRS module(when getting a SMS, GPRS module will return a buffer array)
     *  @param  message	buffer used to get SMS message
     *  @param  check  	whether to check phone number(we may only want to read SMS from specified phone number)
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int readSMS(int messageIndex, char *message, bool newMessage);

    /** delete SMS message on SIM card
     *  @param  index	the index number which SMS message will be delete
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int deleteSMS(int index);

    /** call someone
     *  @param  number	the phone number which you want to call
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int callUp(char* number);

    /** auto answer if coming a call
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int answer(void);
	
    /** build TCP connect
     *  @param  ip	ip address which will connect to
     *  @param  port	TCP server' port number
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int connectTCP(char* ip, char* port);

    /** send data to TCP server
     *  @param  data	data that will be send to TCP server
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int sendTCPData(char* data);

    /** close TCP connection
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int closeTCP(void);

    /** close TCP service
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int shutTCP(void);
	
private:
	char* phoneNumber;
};

	
/** BlueTooth class.
 * 	used to communicate with BlueTooth Module
 */
class Bluetooth : public SIM800
{
public:
	/** Create BlueTooth instance	
	 */
	Bluetooth(int tx, int rx, int power, int baudRate):SIM800(tx,rx,power,baudRate){
		bluetoothPower = 0;
	};

	/** power on BlueTooth module
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int powerOn(void);
	
	/** power off BlueTooth module
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int powerOff(void);
	
    /** get host device name of BlueTooth module
	 *  @param	deviceName	buffer array to save device name.
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int getHostDeviceName(char* deviceName);
	
	/** scan for target device according to device name
	 *  @param	deviceName	device which will be scanned for
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int scanForTargetDevice(char* deviceName);
	
    /** send pairing request to device according to deviceID
	 *  @param	deviceID	device ID 
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int sendPairingReqstToDevice(int deviceID);
	
	/** accept other BlueTooth module's pairing request
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int acceptPairing(void);
	
    /** unPair with paired BlueTooth device
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int unPair(void);
	
	/** accept other BlueTooth device's connecting request
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int acceptConnect(void);
	
    /** wait to handle other BlueTooth device's pairing or connecting  request
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int loopHandle(void);
	
    /** disconnect with connected BlueTooth device
	 * 	@param	deviceID	device that will be disconnected
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int disconnect(int deviceID);
	
	/** connect with other BlueTooth device in SPP profile
	 * 	@param	deviceID	device that will be connect in SPP profile
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int connectInSPP(int deviceID);
	
    /** receive data in SPP profile
	 * 	@param	data	buffer array to receive data from other BlueTooth device in SPP profile
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int recvInSPP(char* data);
	
	/** send data in SPP profile
	 * 	@param	data	buffer array to send data to other BlueTooth device in SPP profile
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int sendInSPP(char* data);

private:

	/** BlueTooth power flag
	 */
	bool bluetoothPower;
	
	/** target device ID
	 */
	int targetDeviceID;
};

/** FM class.
 *	used to use FM function
 */
class FM : public SIM800
{

public:
	FM(int tx, int rx, int power, int baudRate) : SIM800(tx,rx,power,baudRate){
		fmChannelCount = 0;
		fmCurrentChannel = 0;
	};
	
    /** power on FM module
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int powerOn(void);
	
	/** power off FM module
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int powerOff(void);
	
    /** set radio frequency
	 * 	@param	freq	freq that will be chosen
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int setFreq(int freq);
	
	/** set volume of FM output
	 * 	@param	volume	volume:0/1/2/3/4/5/6
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int setVolume(int volume);
	
	/** search for channel
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int scanChannel(void);
	
	/** check channel's signal strength
	 * 	@param	channelFreq	the channel that will be checked
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int checkSignalStrength(int channelFreq);
	
	/** jump to next channel
	 *	@returns
	 *      0 on success
     *      -1 on error
	 */
	int channelNext(void);
	
private:
	/** FM power flag
	 */
	bool fmPower;
	
	/** channel array to save valid channel
	 */
	int fmChannel[10];
	
	/** FM valid channel count record
	 */
	int fmChannelCount;
	
	/** current channel index
	 */
	int fmCurrentChannel;
};

#endif
