/*
    fm.h
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

#ifndef __FM_H__
#define __FM_H__

#include "sim800.h"

/** FM class.
    used to use FM function
*/
class FM: public SIM800 {

  public:
    FM(int baudRate = 19200) : SIM800(baudRate) {
        fmChannelCount = 0;
        fmCurrentChannel = 0;
    };

    /** power on FM module
        @returns
            0 on success
            -1 on error
    */
    int powerOn(void);

    /** power off FM module
        @returns
            0 on success
            -1 on error
    */
    int powerOff(void);

    /** set radio frequency
        @param  freq    freq that will be chosen
        @returns
            0 on success
            -1 on error
    */
    int setFreq(int freq);

    /** set volume of FM output
        @param  volume  volume:0/1/2/3/4/5/6
        @returns
            0 on success
            -1 on error
    */
    int setVolume(int volume);

    /** search for channel
        @returns
            0 on success
            -1 on error
    */
    int scanChannel(void);

    /** check channel's signal strength
        @param  channelFreq the channel that will be checked
        @returns
            0 on success
            -1 on error
    */
    int checkSignalStrength(int channelFreq);

    /** jump to next channel
        @returns
            0 on success
            -1 on error
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