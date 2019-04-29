/** Bare minimum example sketch for MP3 player.
 *
 *  Simply plays all tracks in a loop.
 *
 * | JQ8400 Module | ESP32    |
 * | ------------- | -------- |
 * | RX            | GPIO17   |
 * | TX            | GPIO16   |
 * | GND (any of)  | GND      |
 * | VCC (any of)  | VCC      |

 * @author James Sleeman,  http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */
 

// Create the mp3 connection itself, notice how we give it the 
//  serial object we want it to use to talk to the JQ8400 module.

#include <JQ8400_Serial.h>
JQ8400_Serial mp3(Serial2);

void setup() 
{  
  Serial2.begin(9600);
  mp3.reset();
  mp3.setVolume(20);
  mp3.setLoopMode(MP3_LOOP_ALL);
  mp3.play();  
}

void loop() {
  // Do nothing, it's already playing and looping :-)
}