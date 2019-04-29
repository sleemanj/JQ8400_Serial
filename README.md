*warning* This is a work in progress still, examples need to be rewritten, docs regenerated, and this readme isn't correct yet either :-)

JQ8400_Serial
=======================

Simple to use Arduino library to interface to JQ8400 (JQ8400-FL, JQ8400-SD) Mp3 Player Modules

For complete documentation about the JQ8400 Mp3 Player Module, see: 
   http://sparks.gogo.co.nz/jq8400/index.html
   
For a library methods reference see:
   https://rawcdn.githack.com/sleemanj/JQ8400_Serial/48d9a2ea1c10bec7a9bf69f397898daec2b4101c/docs/html/class_j_q8400___serial.html

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [JQ8400_Serial](#jq8400_serial)
  - [Download, Install and Example](#download-install-and-example)
  - [Connecting To Your Arduino](#connecting-to-your-arduino)
    - [For a SoftwareSerial Connection](#for-a-softwareserial-connection)
      - [SoftwareSerial **5v** Arduino, connect as follows.](#softwareserial-5v-arduino-connect-as-follows)
      - [SoftwareSerial **3v3** Arduino, connect as follows...](#softwareserial-3v3-arduino-connect-as-follows)
    - [For a built in additional hardware serial connection (eg ESP32)](#for-a-built-in-additional-hardware-serial-connection-eg-esp32)
      - [Built In Additional Serial on **5v** Arduino, connect as follows.](#built-in-additional-serial-on-5v-arduino-connect-as-follows)
      - [Built In Additional Serial on **3v3** Arduino, connect as follows.](#built-in-additional-serial-on-3v3-arduino-connect-as-follows)
      - [Example for ESP32 using Built In Serial2](#example-for-esp32-using-built-in-serial2)
      - [Example Code For "Serial2"](#example-code-for-serial2)
  - [Power Demands](#power-demands)
  - [Usage](#usage)
  - [JQ6500 to JQ8400 Breaking Changes](#jq6500-to-jq8400-breaking-changes)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

   
Download, Install and Example
-----------------------------

* Download: http://sparks.gogo.co.nz/JQ8400_Serial.zip
* Open the Arduino IDE (1.6+)
* Select the menu item Sketch > Import Library > Add Library
* Choose to install the JQ8400_Serial.zip file you downloaded
* Now you can choose File > Examples > JQ8400_Serial > HelloWorld
  
Connecting To Your Arduino
--------------------------

The JQ8400_Serial library can be configured to talk through any "Serial" port, including "SoftwareSerial", anything that is a Stream.  

Most commonly you will use a SoftwareSerial with typical "AVR" based Arduinos that only have one hardware seral port.  For Arduino's (and related, like ESP32) that have more than one hardware serial port you will typically want to use one of those instead of SoftwareSerial. 

The most important consideration is that if your Arduino is 5 volt, you will want to put a 1k resistor on the RX pin of the JQ8400 and the Arduino's appropriate TX pin you are using.

Below are some examples to help you.

### For a SoftwareSerial Connection

This is the typical setup where you have an Arduino Uno or Nano etc which only has one "Hardware" serial port and you use that to upload and communicate to the Arduino yourself, therefore we use a SoftwareSerial connection to communicate between the Arduino and the JQ8400 Module.

#### SoftwareSerial **5v** Arduino, connect as follows.

| JQ8400 Module | Arduino |
| ------------- | ------- |
| RX            | through a 1K Resistor then to pin 9 |
| TX            | pin 8   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

#### SoftwareSerial **3v3** Arduino, connect as follows...

| JQ8400 Module | Arduino |
| ------------- | ------- |
| RX            | pin 9   |
| TX            | pin 8   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

You can use pins other than 9 and 8 if you wish, simply set them in your code, which looks something like this...

    #include <SoftwareSerial.h>
    SoftwareSerial mySerial(8,9);
    
    #include <JQ8400_Serial.h>
    JQ8400_Serial mp3(mySerial);
    
    void setup()
    {
      mySerial.begin(9600); // We must talk to the JQ8400 at 9600
      mp3.reset();          // Reset the device (optional, recommended)
    }

    void loop()
    {
      // You should do more interesting stuff than just this...
      mp3.next();
      while(mp3.busy()); // Wait until finished.
    }
    
### For a built in additional hardware serial connection (eg ESP32)

This is a typical setup where your Arduino environment has more than one "Serial", and you want to use one of those to communicate to the JQ8400, leaving the normal "Serial" free for uploading, communication to the Arduino etc...

#### Built In Additional Serial on **5v** Arduino, connect as follows.

| JQ8400 Module | Arduino |
| ------------- | ------- |
| RX            | through a 1K Resistor then to appropriate TX pin |
| TX            | RX pin   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

#### Built In Additional Serial on **3v3** Arduino, connect as follows.

| JQ8400 Module | Arduino |
| ------------- | ------- |
| RX            | appropriate TX pin   |
| TX            | appropriate RX pin   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

#### Example for ESP32 using Built In Serial2

| JQ8400 Module | ESP32 |
| ------------- | ------- |
| RX            | GPIO17   |
| TX            | GPIO16   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |


#### Example Code For "Serial2"

    #include <JQ8400_Serial.h>
    JQ8400_Serial mp3(Serial2);
    
    void setup()
    {
      Serial2.begin(9600);  // We must talk to the JQ8400 at 9600 - maybe you need to add some extra parameters to begin, consult your specific board's documents if you have trouble.
      mp3.reset();          // Reset the device (optional, recommended)
    }

    void loop()
    {
      // You should do more interesting stuff than just this...
      mp3.next();
      while(mp3.busy()); // Wait until finished.
    }


Power Demands
--------------------------

If using the on-board speaker driver, then naturally the power
demands are significant, and your USB power may not be sufficient
at more 1/3rd level of volume or so, the symptom is the audo 
breaking up and potentially resetting when volume increases.

You should use either an external power source, an external amp, or a lower
volume if you experience this problem.

Usage
--------------------------

Open the HelloWorld example.


JQ6500 to JQ8400 Breaking Changes
--------------------------

This section is for users of previous JQ6500 devices and the JQ6500_Serial library who are transitioning to a JQ8400 with JQ8400_Serial library.

As much as possible the JQ8400_Serial library should follow the same "api", with some additions, however there are some "breaking changes" you should be aware of...
 
  * The initialisation has changed so that you can (must) point the JQ8400 library to a specific serial connection to the JQ8400 player, you can use SoftwareSerial, or any of your other serial ports (or any Stream).  This enables you to use devices that do not support SoftwareSerial.
  
  **Example for software serial on pins 8 and 9...**
      
    #include <SoftwareSerial.h>
    SoftwareSerial  mySerial(8,9);
      
    #include <JQ8400_Serial.h>
    JQ8400_Serial   mp3(mySerial);
    
    void setup()
    {
      mySerial.begin(9600);
      mp3.reset();
    }

  **Example over microcontroller's default hardware serial port...**

    #include <JQ8400_Serial.h>
    JQ8400_Serial   mp3;
    
    void setup()
    {
      Serial.begin(9600);
      mp3.begin(Serial);
    }

  **Example over microcontroller's other hardware serial port...**

    #include <JQ8400_Serial.h>
    JQ8400_Serial   mp3;
    
    void setup()
    {
      Serial2.begin(9600);
      mp3.begin(Serial2);
    }

  * JQ8400 does not support `countFolders()` at all (AFAIK) therefore this method is not available..
  * JQ8400 is not given a source for `countFiles` - that is use `mp3.countFiles()` not `mp3.countFiles(MP3_SRC_SDCARD)`, use `mp3.setSource(MP3_SRC_SDCARD)` to change the source before counting if you are not already on that source.
  * JQ8400 does not support MP3_EQ_BASS
