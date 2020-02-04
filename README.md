## Seeeduino GPRS  [![Build Status](https://travis-ci.com/Seeed-Studio/Seeeduino_GPRS.svg?branch=master)](https://travis-ci.com/Seeed-Studio/Seeeduino_GPRS)

![](https://statics3.seeedstudio.com/seeed/file/2018-01/bazaar669815_seeeduinogprs.jpg)

[](https://www.seeedstudio.com/s/Seeeduino-GPRS-p-1909.html)

Seeeduino GPRS is a IoT panel, you can connect to the internet through GPRS wireless network with it. Making/answering voice calls and sending/receiving SMS messages are also supported. Meanwhile, Seeeduino GPRS supports FM radio function and bluetooth communication. For more information, you can visit our [Wiki](http://wiki.seeedstudio.com/Seeeduino_GPRS/).

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

Note：There is another library has better performance [GPRS SIM900](https://github.com/Seeed-Studio/GPRS_SIM900), to use it, exchange the RX and TX pins.  
+ #define PIN_TX    7    ------   #define PIN_TX    8
+ #define PIN_RX    8    ------   #define PIN_RX    7

Sample of using GPRS_SIM900 library - GPRS_TCPConnection.ino
```C
#include <SoftwareSerial.h>
#include <Wire.h>
#include "sim900.h"

#define PIN_TX    8  // Has already exchanged TX and RX pins
#define PIN_RX    7

#define SIM800_POWER_PIN        9  // Define SIM800 power status and control pins
#define SIM800_POWER_STATUS     12

//make sure that the baud rate of SIM800 is 9600!
//you can use the AT Command(AT+IPR=9600) to set it through SerialDebug
#define BAUDRATE  9600

char http_cmd[] = "GET /media/uploads/mbed_official/hello.txt HTTP/1.0\r\n\r\n";
char buffer[512];
GPRS gprs(PIN_TX, PIN_RX, BAUDRATE);
void setup(){
  Serial.begin(9600);
  SIM800_PreInit();
  // use DHCP
  while(!gprs.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  }
  delay(3000);    
  // attempt DHCP
  while(!gprs.join(F("cmnet"))) {
      Serial.println("gprs join network error");
      delay(2000);
  }

  // successful DHCP
  Serial.print("IP Address is ");
  Serial.println(gprs.getIPAddress());

  if(!gprs.connect(TCP,"mbed.org", 80)) {
      Serial.println("connect error");
  }else{
      Serial.println("connect mbed.org success");
  }

  Serial.println("waiting to fetch...");
  gprs.send(http_cmd, sizeof(http_cmd)-1);
  while (true) {
      int ret = gprs.recv(buffer, sizeof(buffer)-1);
      if (ret <= 0){
          Serial.println("fetch over...");
          break;
      }
      buffer[ret] = '\0';
      Serial.print("Recv: ");
      Serial.print(ret);
      Serial.print(" bytes: ");
      Serial.println(buffer);
  }
  gprs.close();
  gprs.disconnect();
}

void loop(){

}

void SIM800_PreInit(void)
{
    Serial.println("SIM800 PreInit...");
    pinMode(SIM800_POWER_STATUS,INPUT);
    delay(10);
    if(LOW == digitalRead(SIM800_POWER_STATUS))
    {
        if(sim900_send_AT() != true)
        {            
            delay(800);
            digitalWrite(SIM800_POWER_PIN,HIGH);
            delay(200);
            digitalWrite(SIM800_POWER_PIN,LOW);
            delay(2000);
            digitalWrite(SIM800_POWER_PIN,HIGH);
            delay(3000);  
        }
        while(sim900_send_AT() != true);                
        Serial.println("Init O.K!");         
    }
    else
    {
        Serial.println("Power check failed!");  
    }
}
```

----
This software is written by lawliet zou (![](http://www.seeedstudio.com/wiki/images/f/f8/Email-lawliet.zou.jpg)) for [Seeed Technology Inc.](http://www.seeed.cc) and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt/LICENSE for the details of MIT license.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed is a hardware innovation platform for makers to grow inspirations into differentiating products. By working closely with technology providers of all scale, Seeed provides accessible technologies with quality, speed and supply chain knowledge. When prototypes are ready to iterate, Seeed helps productize 1 to 1,000 pcs using in-house engineering, supply chain management and agile manufacture forces. Seeed also team up with incubators, Chinese tech ecosystem, investors and distribution channels to portal Maker startups beyond.





[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/Seeeduino_GPRS)](https://github.com/igrigorik/ga-beacon)
