/*
    bluetooth.h
    A library for SeeedStudio seeeduino GPRS shield

    Copyright (c) 2013 seeed technology inc.
    Author        :   lawliet zou
    Create Time   :   Dec 2013
    Change Log    :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "sim800.h"
/** BlueTooth class.
    used to communicate with BlueTooth Module
*/
class BlueTooth: public SIM800 {
  public:
    /** Create BlueTooth instance
    */
    BlueTooth(int baudRate = 9600): SIM800(baudRate) {
        bluetoothPower = 0;
    };

    /** power on BlueTooth module
        @returns
            0 on success
            -1 on error
    */
    int powerOn(void);

    /** power off BlueTooth module
        @returns
            0 on success
            -1 on error
    */
    int powerOff(void);

    /** get host device name of BlueTooth module
        @param  deviceName  buffer array to save device name.
        @returns
            0 on success
            -1 on error
    */
    int getHostDeviceName(char* deviceName);

    /** scan for target device according to device name
        @param  deviceName  device which will be scanned for
        @returns
            0 on success
            -1 on error
    */
    int scanForTargetDevice(char* deviceName);

    /** send pairing request to device according to deviceID
        @param  deviceID    device ID
        @returns
            0 on success
            -1 on error
    */
    int sendPairingReqstToDevice(int deviceID);

    /** accept other BlueTooth module's pairing request
        @returns
            0 on success
            -1 on error
    */
    int acceptPairing(void);

    /** unPair with paired BlueTooth device
        @returns
            0 on success
            -1 on error
    */
    int unPair(void);

    /** accept other BlueTooth device's connecting request
        @returns
            0 on success
            -1 on error
    */
    int acceptConnect(void);

    /** wait to handle other BlueTooth device's pairing or connecting  request
        @returns
            0 on success
            -1 on error
    */
    int loopHandle(void);

    /** disconnect with connected BlueTooth device
        @param  deviceID    device that will be disconnected
        @returns
            0 on success
            -1 on error
    */
    int disconnect(int deviceID);

    /** connect with other BlueTooth device in SPP profile
        @param  deviceID    device that will be connect in SPP profile
        @returns
            0 on success
            -1 on error
    */
    int connectInSPP(int deviceID);

    /** receive data in SPP profile
        @param  data    buffer array to receive data from other BlueTooth device in SPP profile
        @returns
            0 on success
            -1 on error
    */
    int recvInSPP(char* data);

    /** send data in SPP profile
        @param  data    buffer array to send data to other BlueTooth device in SPP profile
        @returns
            0 on success
            -1 on error
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

#endif