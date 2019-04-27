*warning* This is a work in progress still, examples need to be rewritten, docs regenerated, and this readme isn't correct yet either :-)

JQ8400_Serial
=======================

Simple to use Arduino library to interface to JQ8400 (JQ8400-FL, JQ8400-SD) Mp3 Player Modules

For complete documentation about the JQ8400 Mp3 Player Module, see: 
   http://sparks.gogo.co.nz/jq8400/index.html
   
For a library methods reference see:
   https://rawcdn.githack.com/sleemanj/JQ8400_Serial/48d9a2ea1c10bec7a9bf69f397898daec2b4101c/docs/html/class_j_q8400___serial.html

Download, Install and Example
-----------------------------

* Download: http://sparks.gogo.co.nz/JQ8400_Serial.zip
* Open the Arduino IDE (1.6+)
* Select the menu item Sketch > Import Library > Add Library
* Choose to install the JQ8400_Serial.zip file you downloaded
* Now you can choose File > Examples > JQ8400_Serial > HelloWorld
  
Connecting To Your Arduino
--------------------------

<img src="http://sparks.gogo.co.nz/assets/_site_/images/jq6500/kq6500-16p.jpeg" align="right" title="JQ6500-16p" alt="Pinout image of JQ6500-16p MP3 Player Module For Arduino"/>
<img src="http://sparks.gogo.co.nz/assets/_site_/images/jq6500/jq6500-28.jpeg" align="right" title="JQ6500-28p" alt="Pinout image of JQ6500-28p MP3 Player Module For Arduino"/>

There are two varients of the JQ6500 module as shown.

To use this library with a *5v Arduino*, connect as follows.

| JQ6500 Module | Arduino |
| ------------- | ------- |
| RX            | through a 1K Resistor then to pin 9 |
| TX            | pin 8   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

To use this library with a *3v3 Arduino*, connect as follows...

| JQ6500 Module | Arduino |
| ------------- | ------- |
| RX            | pin 9   |
| TX            | pin 8   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

You can use pins other than 9 and 8 if you wish, simply set them in your code.

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
