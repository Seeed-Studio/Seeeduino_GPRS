/*
    bluetooth.cpp
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

#include "bluetooth.h"

int BlueTooth::powerOn(void) {
    sendATTest();
    sendATTest();
    if (0 == bluetoothPower) {
        if (0 == sendCmdAndWaitForResp("AT+BTSTATUS?\r\n", "+BTSTATUS: 0", DEFAULT_TIMEOUT)) {
            if (0 != sendCmdAndWaitForResp("AT+BTPOWER=1\r\n", "OK", DEFAULT_TIMEOUT)) {
                ERROR("\r\nERROR:bluetoothPowerOn\r\n");
                return -1;
            } else {
                bluetoothPower = 1;
            }
        } else {
            bluetoothPower = 1;
        }
    }
    return 0;
}

int BlueTooth::powerOff(void) {
    if (1 == bluetoothPower) {
        if (0 != sendCmdAndWaitForResp("AT+BTSTATUS?\r\n", "+BTSTATUS: 0", DEFAULT_TIMEOUT)) {
            if (0 != sendCmdAndWaitForResp("AT+BTPOWER=0\r\n", "OK", DEFAULT_TIMEOUT)) {
                ERROR("\r\nERROR:bluetoothPowerOff\r\n");
                return -1;
            } else {
                bluetoothPower = 0;
            }
        }

    }
    return 0;
}

int BlueTooth::getHostDeviceName(char* deviceName) {
    char gprsBuffer[40];
    char* s, *p;
    int i = 0;
    sendCmd("AT+BTHOST?\r\n");
    readBuffer(gprsBuffer, 40, DEFAULT_TIMEOUT);
    if (NULL == (s = strstr(gprsBuffer, "+BTHOST:"))) {
        ERROR("\r\nERROR: get host device name error\r\n");
        return -1;
    }
    p = s + 9;//+BTHOST: SIM800H,33:7d:77:18:62:60
    while (*(p) != ',') {
        deviceName[i++] = *p;
        p++;
    }
    deviceName[i] = '\0';
    return i;
}

int BlueTooth::scanForTargetDevice(char* deviceName) {
    char gprsBuffer[100];
    char* s;
    sendCmd("AT+BTSCAN=1,20\r\n"); //scan 20s
    cleanBuffer(gprsBuffer, 100);
    readBuffer(gprsBuffer, 100, 20); //+BTPAIR: 1,"ME863",5c:6b:32:91:00:d1
    if (NULL == (s = strstr(gprsBuffer, deviceName))) {
        ERROR("\r\nERROR: scan For Target Device error\r\n");
        return -1;
    }
    targetDeviceID = atoi(s - 3);
    return targetDeviceID;
}

int BlueTooth::sendPairingReqstToDevice(int deviceID) {
    char cmd[30];
    if (0 == deviceID) {
        return -1;
    }
    sprintf(cmd, "AT+BTPAIR=0,%d\r\n", deviceID);
    sendCmd(cmd);
    return 0;
}
int BlueTooth::unPair(void) {
    char cmd[30];
    if (0 == targetDeviceID) {
        return -1;
    }
    sprintf(cmd, "AT+BTUNPAIR=%d\r\n", targetDeviceID);
    if (0 != sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR: AT+BTUNPAIR\r\n");
        return -1;
    }
    return 0;
}

int BlueTooth::acceptPairing(void) {
    sendCmd("AT+BTPAIR=1,1\r\n");
    return 0;
}

int BlueTooth::acceptConnect(void) {
    if (0 != sendCmdAndWaitForResp("AT+BTACPT=1\r\n", "OK", DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR:AT+BTACPT\r\n");
        return -1;
    }
    return 0;
}
int BlueTooth::disconnect(int deviceID) {
    char cmd[30];
    if (0 == targetDeviceID) {
        return -1;
    }
    sprintf(cmd, "AT+BTDISCONN=%d\r\n", targetDeviceID);
    if (0 != sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR: AT+BTDISCONN\r\n");
        return -1;
    }
    return 0;
}

int BlueTooth::loopHandle(void) {
    char gprsBuffer[100];
    cleanBuffer(gprsBuffer, 100);
    while (1) {
        if (serialSIM800.available()) {
            break;
        }
        delay(1000);
    }
    readBuffer(gprsBuffer, 100, DEFAULT_TIMEOUT);

    if (NULL != strstr(gprsBuffer, "+BTPAIRING:")) {
        if (0 != acceptPairing()) {
            return -1;
            ERROR("\r\nERROR:bluetoothAcceptPairing\r\n");
        }
    } if ((NULL != strstr(gprsBuffer, "+BTCONNECTING:")) && (NULL != strstr(gprsBuffer, "SPP"))) {
        if (0 != acceptConnect()) {
            return -1;
            ERROR("\r\nERROR:bluetoothAcceptConnecting\r\n");
        }
    }
    return 0;
}
int BlueTooth::connectInSPP(int deviceID) { //Serial Port Profile
    char gprsBuffer[200];
    char cmd[20];
    char* s;
    sprintf(cmd, "AT+BTGETPROF=%d\r\n", deviceID);
    sendCmd(cmd);
    readBuffer(gprsBuffer, 200, DEFAULT_TIMEOUT);
    if (NULL == (s = strstr(gprsBuffer, "\"SPP\""))) {
        ERROR("\r\nERROR: No SPP Profile\r\n");
        return -1;
    }
    cleanBuffer(cmd, 20);
    sprintf(cmd, "AT+BTCONNECT=%d,%c\r\n", deviceID, *(s - 2));
    if (0 != sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR:AT+BTCONNECT\r\n");
        return -1;
    }
    return 0;
}

int BlueTooth::recvInSPP(char* data) {
    char bluetoothBuffer[64];
    char* p = NULL;
    int count = 0;
    if (0 != sendCmdAndWaitForResp("AT+BTSPPGET=0\r\n", "OK", DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR:AT+BTSPPGET\r\n");
        return -1;
    }
    while (!serialSIM800.available()) {
        delay(200);
    }
    readBuffer(bluetoothBuffer, 64, DEFAULT_TIMEOUT);
    p = strstr(bluetoothBuffer, "+BTSPPDATA:");
    if (NULL != p) {
        p += 11;
        while (count != 2) {
            if (*(p++) == ',') {
                count++;
            }
        }
        int i = 0;
        while (*(p++) != '$') {
            data[i++] = *p;
        }
    } else {
        ERROR("\r\nERROR: get data error\r\n");
        return -1;
    }
    return 0;
}

int BlueTooth::sendInSPP(char* data) {
    if (0 != sendCmdAndWaitForResp("AT+BTSPPSEND\r\n", ">", DEFAULT_TIMEOUT)) {
        ERROR("\r\nERROR:AT+BTSPPSEND\r\n");
        return -1;
    }
    sendCmd(data);
    sendEndMark();
    return 0;
}
