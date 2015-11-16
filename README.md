##Seeeduino GPRS
[![Seeeduino GPRS](http://www.seeedstudio.com/wiki/images/2/2c/IMG_20140630_153006.jpg)](http://www.seeedstudio.com/wiki/Seeeduino_GPRS)
<br>
Note：There is another library has better performance [GPRS SIM900](https://github.com/Seeed-Studio/GPRS_SIM900), to use it, exchange the RX and TX pins.<br>
+ #define PIN_TX    7    ------   #define PIN_TX    8
+ #define PIN_RX    8    ------   #define PIN_RX    7

Seeeduino GPRS is a IoT panel, you can connect to the internet through GPRS wireless network with it. Making/answering voice calls and sending/receiving SMS messages are also supported. Meanwhile, Seeeduino GPRS supports FM radio function and bluetooth communication. For more information, you can visit our [Wiki](http://www.seeedstudio.com/wiki/Seeeduino_GPRS)

### Features
+ Compatible with standard Arduino Leonardo
+ Quad-Band 850/900/1800/1900 MHZ
+ Headset Jack
+ Convenient external SIM card holder
+ Control via AT commands
+ Support Bluetooth
+ Support FM


### Applications
+ Call up and answer Call
+ Send SMS and read SMS
+ AT Command Test
+ TCP Connection
+ Bluetooth Communication
+ FM radio

### Getting Started
please follow the example sketch in example folder.
+ SIM800_Serial_Debug —— test SIM800 module with AT commands.
+ GPRS_CallUp ——  give a call to someone.
+ GPRS_SendSMS —— send a message to someone.
+ GPRS_LoopHandle —— answer the call & read the message.
+ GPRS_ConnectTCP —— communicate with xively. 
+ FM_Test —— Listening to FM radio
+ Bluetooth_AT_Command_And_Music_Play —— use your bluetooth device to control seeeduino GPRS with AT Command or use your smartphone to connect it and play music.
+ Bluetooth_Call_PhoneNumber —— Use your smartphone to connect seeeduino GPRS and a auto-call action will be executed.



----
This software is written by lawliet zou (![](http://www.seeedstudio.com/wiki/images/f/f8/Email-lawliet.zou.jpg)) for [Seeed Technology Inc.](http://www.seeed.cc) and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt/LICENSE for the details of MIT license.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed is a hardware innovation platform for makers to grow inspirations into differentiating products. By working closely with technology providers of all scale, Seeed provides accessible technologies with quality, speed and supply chain knowledge. When prototypes are ready to iterate, Seeed helps productize 1 to 1,000 pcs using in-house engineering, supply chain management and agile manufacture forces. Seeed also team up with incubators, Chinese tech ecosystem, investors and distribution channels to portal Maker startups beyond.





[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/Seeeduino_GPRS)](https://github.com/igrigorik/ga-beacon)

