/** Example sketch which plays files on the media in random order.
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

unsigned int numFiles; // Total number of files on media (autodetected in setup())

void setup() {
  // Start debugging terminal
  Serial.begin(9600);  
  
  // Begin the connection, reset the device, and set volume to a reasonable level
  mySoftwareSerial.begin(9600);
  mp3.reset();
  
  while(numFiles == 0)
  {
    // See if there is an SD Card and switch to it
    if(mp3.sourceAvailable(MP3_SRC_SDCARD))
    {
      mp3.setSource(MP3_SRC_SDCARD);    
      if(mp3.countFiles() == 0)
      {
        mp3.setSource(MP3_SRC_BUILTIN);
      }
    }
    
    numFiles = mp3.countFiles();
    
    if(!numFiles)
    {
      Serial.println(F("Error! No files were found on the media, both SD Card and Built In memory were checked."));
      Serial.println(F("We will try again in 3 seconds."));
      Serial.println(F("If there are files there and we still can't find them, try turning everything off and on again, perhaps the module is confused."));
      Serial.println(F("I think this might happen sometimes if you insert/remove an SD Card while powered up, but not totally sure!"));
      Serial.println(F("In a real application, you might consider powering the JQ8400 module through a suitable MOSFET or BJT controlled from a pin so you can power-cycle the JQ8400 if it starts to act weird like this!"));
      delay(3000);
    }
  }
}

void loop() 
{
  if(!mp3.busy()) 
  {
    // pick a random file, numbered 1 to numFiles (NB: random() returns up to but not including the highest number, hence why we add 1)
    // if the file is the same as the one which was just played, pick a different one
    unsigned int pick;
    do
    { 
      pick = random(1,numFiles+1);
    } while(pick == mp3.currentFileIndexNumber());
    

    Serial.print("Randomly selected #");
    Serial.print(pick);
    Serial.print(" of ");
    Serial.println(numFiles);

    
    // and play it
    mp3.playFileByIndexNumber(pick);
   
    char buffer[20];
    mp3.currentFileName(buffer, sizeof(buffer));
    Serial.print("> ");
    Serial.println(buffer);
  }
}
