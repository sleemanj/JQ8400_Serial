/** Demonstrate how to interrupt a currently playing file 
 *   with another file and then return to the interrupted file.
 * 
 * @author James Sleeman,  http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */
 
// This example uses SoftwareSerial on pin 8 and 9
#include <SoftwareSerial.h>
SoftwareSerial mySoftwareSerial(8,9);

// Create the mp3 connection itself, notice how we give it the 
//  serial object we want it to use to talk to the JQ8400 module.
// For example you might use mp3(Serial2) instead of a SoftwareSerial
#include <JQ8400_Serial.h>
JQ8400_Serial mp3(mySoftwareSerial);

void setup() 
{  
  mySoftwareSerial.begin(9600);
  mp3.reset();
  mp3.setVolume(20);
  
  // Start the "background music" playing from position 2
  mp3.setLoopMode(MP3_LOOP_ALL);
  mp3.playFileByIndexNumber(2);
}

void loop() 
{
  // Every 6.5 seconds "interject" with the file at position 1
  delay(6500);
  mp3.interjectFileByIndexNumber(1);
  // File #1 will interrupt the current playback, play, and then 
  //  playback will resume where it left off
}
