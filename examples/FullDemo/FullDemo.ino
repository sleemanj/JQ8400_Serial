/** General demo of MP3 Controls, after uploading, open your Serial Monitor and enter commands.
 * 
 * Allows you to test most the various controls of the JQ8400 module.
 *
 * @author James Sleeman,  http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */

// This example uses SoftwareSerial
//
//   Arduino Pin 8 is connected to TX of the JQ8400
//   Arduino Pin 9 is connected to one end of a  1k resistor, 
//     the other end of the 1k resistor is connected to RX of the JQ8400
//   If your Arduino is 3v3 powered, you can omit the 1k series resistor
//
// If you have an additional hardware serial, you don't need these two lines

#include <SoftwareSerial.h>
SoftwareSerial mySerial(8,9);

// Create the mp3 connection itself, notice how we give it the 
//  serial object we want it to use to talk to the JQ8400 module.
// For example you might use mp3(Serial2) instead.
#include <JQ8400_Serial.h>
JQ8400_Serial mp3(mySerial);

void setup() 
{  
  // We must communicate at 9600 to the JQ8400, setup the serial port we are using
  //  to do that now.  
  mySerial.begin(9600);

  // Now we cam reset.
  mp3.reset();

  // We output our menu and status etc to the normal serial port
  Serial.begin(9600);
  statusAndHelpOutput();
}

void loop() {
  
    byte b;
    if(Serial.available())
    {
      b = Serial.read();

      switch(b)
      {
        case 'p': Serial.println("Play");   mp3.play();     return;
        case 'r': Serial.println("Restart"); mp3.restart(); return;
        case ' ': Serial.println("Pause"); mp3.pause();     return;
        case '>': Serial.println("Next");  mp3.next();      return;
        case '<': Serial.println("Prev");  mp3.prev();      return;
        
        case ']': Serial.println("Next Folder");  mp3.nextFolder(); return;
        case '[': Serial.println("Prev Folder");  mp3.prevFolder(); return;
    
        case '+': Serial.println("Vol +"); mp3.volumeUp(); return;
        case '-': Serial.println("Vol -"); mp3.volumeDn(); return;
        case 'm': Serial.println("Vol 0"); mp3.setVolume(0); return;
        
        case 'v': 
        {
          char volBuff[10]; 
          memset(volBuff, 0, sizeof(volBuff));          
          Serial.readBytesUntil('\n',volBuff, sizeof(volBuff)-1);
          mp3.setVolume(max(0,min(30, atoi(volBuff))));
          
          Serial.print("Vol ");
          Serial.println(max(0,min(30, atoi(volBuff))));                    
        }
        return;

        case 'e':
        {
          do
          {
            while(!Serial.available()); // Wait
            b = Serial.read();
            if(b != ' ') break; // Allow "e N" or "eN" etc...
          } while(1);
          
          Serial.print("Equalizer ");
          switch(b)
          {
            case 'N': Serial.println("Normal");  mp3.setEqualizer(MP3_EQ_NORMAL);  break;
            case 'P': Serial.println("Pop");     mp3.setEqualizer(MP3_EQ_POP);     break;
            case 'R': Serial.println("Rock");    mp3.setEqualizer(MP3_EQ_ROCK);    break;
            case 'J': Serial.println("Jazz");    mp3.setEqualizer(MP3_EQ_JAZZ);    break;
            case 'C': Serial.println("Classic"); mp3.setEqualizer(MP3_EQ_CLASSIC); break;
          }
        }
        return;

        case 'l':
        {
          do
          {
            while(!Serial.available()); // Wait
            b = Serial.read();
            if(b != ' ') break; // Allow "e N" or "eN" etc...
          } while(1);
          
          Serial.print("Loop ");
          switch(b)
          {
            case 'A': Serial.println(F("All")); mp3.setLoopMode(MP3_LOOP_ALL);      break;  // Plays the tracks one after another and repeats 
            case 'R': Serial.println(F("Random")); mp3.setLoopMode(MP3_LOOP_ALL_RANDOM);       break;
            
            case 'F': Serial.println(F("Folder")); mp3.setLoopMode(MP3_LOOP_FOLDER);       break; // Loop within folder
            case 'r': Serial.println(F("Folder Random")); mp3.setLoopMode(MP3_LOOP_FOLDER_RANDOM);       break;
            
            case 'O': Serial.println(F("One (repeat playing same file)")); mp3.setLoopMode(MP3_LOOP_ONE);       break;
            case 'N': 
            case 'S': Serial.println(F("None (play file and stop)")); mp3.setLoopMode(MP3_LOOP_ONE_STOP); break;  // Default, plays track and stops
          }
        }
        return;

        case 's':
        {
          do
          {
            while(!Serial.available()); // Wait
            b = Serial.read();
            if(b != ' ') break; // Allow "e N" or "eN" etc...
          } while(1);
          
          Serial.print("Source ");
          switch(b)
          {
            case 'S': Serial.println("SD Card (if available)."); mp3.setSource(MP3_SRC_SDCARD);      break;
            case 'B': Serial.println("on board memory.");mp3.setSource(MP3_SRC_BUILTIN);   break;            
          }
        }
        return;

        case 'f':
        {
          char fnumBuff[10]; 
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('\n',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int fnum = strtoul(fnumBuff, NULL, 10);

          Serial.println();
          Serial.print("Play file #");
          Serial.print(fnum);
          Serial.println(F(" (if it exists)."));
          mp3.playFileByIndexNumber(fnum); // 48 == ord('0')   
          return;
          
        }
        return;
        
        case 'i':
        {
          char fnumBuff[10]; 
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('\n',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int fnum = strtoul(fnumBuff, NULL, 10);

          Serial.println();
          Serial.print("Interject file #");
          Serial.print(fnum);
          Serial.println(F(" (if it exists)."));
          mp3.interjectFileByIndexNumber(fnum); // 48 == ord('0')   
          return;
          
        }
        return;
        
        case 'F':
        {
          char fnumBuff[10]; 
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('/',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int folnum = strtoul(fnumBuff, NULL, 10);
          
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('\n',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int fnum = strtoul(fnumBuff, NULL, 10);
          
          fnum   = max(1,min(fnum, 999));
          folnum = max(1,min(folnum, 99));
          
          Serial.print("Play "); 
            if(folnum < 10) Serial.print('0');
          Serial.print(folnum);
          Serial.print('/');
            if(fnum < 10) Serial.print("00");
            else if(fnum < 100) Serial.print('0');
          Serial.print(fnum);
          Serial.println(".mp3 (if it exists).");
          mp3.playFileNumberInFolderNumber(folnum, fnum); // 48 == ord('0')   
        }
        return;

         case 'D':
         {
            char fnumBuff[10]; 
            memset(fnumBuff, 0, sizeof(fnumBuff));          
            Serial.readBytesUntil('/',fnumBuff, sizeof(fnumBuff)-1);
            
            unsigned int folnum = strtoul(fnumBuff, NULL, 10);
            folnum = max(1,min(folnum, 99));
            
            Serial.print("Play "); 
              if(folnum < 10) Serial.print('0');
            Serial.print(folnum);
            Serial.println("/*.mp3 (if it exists).");
            mp3.playInFolderNumber(folnum); // 48 == ord('0')   
         }
         return;
                  
        case '?': statusAndHelpOutput(); return;
        
        case 'S': Serial.println("Sleep"); mp3.sleep(); return;
        case 'z': Serial.println("Reset"); mp3.reset(); return;
        
      }
      
    }
    
    static unsigned long m = millis();
    
    if(millis() > 1000 && m < (millis() - 1000))
    {
      if((mp3.getStatus() == MP3_STATUS_PLAYING))
      {
        Serial.print(F("Playing, Current Position: "));
        Serial.print(mp3.currentFilePositionInSeconds());
        Serial.print(F("s / "));
        Serial.print(mp3.currentFileLengthInSeconds());       
        Serial.println('s');
      }
      m = millis();
    }  
}

void statusAndHelpOutput()
{
   Serial.println();
   Serial.println(F("JQ8400 MP3 Player Demo"));
   Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
   
   Serial.print(F("Status           : "));
   switch(mp3.getStatus())
   {
     case MP3_STATUS_STOPPED: Serial.println(F("Stopped")); break;
     case MP3_STATUS_PLAYING: Serial.println(F("Playing")); break;
     case MP3_STATUS_PAUSED:  Serial.println(F("Paused"));  break;
   }
   
   Serial.print(F("Volume (0-30)    : "));
   Serial.println(mp3.getVolume());
  
   Serial.print(F("Equalizer        : "));
   switch(mp3.getEqualizer())
   {
     case MP3_EQ_NORMAL:     Serial.println(F("Normal"));  break;
     case MP3_EQ_POP:        Serial.println(F("Pop"));     break;
     case MP3_EQ_ROCK:       Serial.println(F("Rock"));    break;
     case MP3_EQ_JAZZ:       Serial.println(F("Jazz"));    break;
     case MP3_EQ_CLASSIC:    Serial.println(F("Classic")); break; 
   }
   
   Serial.print(F("Loop Mode        : "));
   switch(mp3.getLoopMode())
   {
     case MP3_LOOP_ALL:      Serial.println(F("Play all tracks, then repeat."));  break;
     case MP3_LOOP_ALL_STOP:      Serial.println(F("Play all tracks, then stop."));  break;
     case MP3_LOOP_ALL_RANDOM: Serial.println(F("Play all tracks randomly."));    break;          
          
     case MP3_LOOP_ONE:      Serial.println(F("Play one track then repeat (loop track)."));    break;     
     case MP3_LOOP_ONE_STOP: Serial.println(F("Play one track then stop (no looping)."));    break;          
     
     case MP3_LOOP_FOLDER:   Serial.println(F("Play all tracks in folder, then repeat."));     break;     
     case MP3_LOOP_FOLDER_STOP:   Serial.println(F("Play all tracks in folder, then stop."));     break;
     case MP3_LOOP_FOLDER_RANDOM:   Serial.println(F("Play all tracks in folder randomly."));     break;     
     
   }
   Serial.println();

   if(!mp3.busy())
   {
     uint8_t currentSource = mp3.getSource();
   
     mp3.setSource(MP3_SRC_BUILTIN);   
     Serial.print(F("# of On Board Memory Files    : "));
     Serial.println(mp3.countFiles());
     
     if(mp3.sourceAvailable(MP3_SRC_SDCARD))
     {
       mp3.setSource(MP3_SRC_SDCARD);
       Serial.print(F("# of SD Card Files    : "));
       Serial.println(mp3.countFiles());
     }
     else
     {
       Serial.println(F("SD Card Not Available"));
     }
     
     if(currentSource != mp3.getSource())
     {
       mp3.setSource(currentSource);
     }
   }
   Serial.print(F("\"Current\" File Index: "));
   Serial.println(mp3.currentFileIndexNumber());
   Serial.println();
   
   Serial.print(F("\"Current\" File Name : "));   
   char buff[120];
   mp3.currentFileName(buff, sizeof(buff));
   Serial.println(buff);
      
   Serial.println();
   Serial.println(F("Controls (type in serial monitor and hit send): "));
   Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
   Serial.println(F("? Display this menu.\n"));
   
   Serial.println(F("p Play\t\t> Next\t\t< Prev\n[space] Pause\tr Restart from start of file\n] Next folder\t[ Prev folder\n"));
   
   Serial.println(F("f[1-65534]      Play file by (FAT table) index number\nF[01-99]/[001-999].mp3 Play [001-999].mp3 in folder [01-99]\n"));
   Serial.println(F("i[1-65534]      Interject file by (FAT table) index number\n"));
   
   Serial.println(F("+ Vol up\t- Vol down\tm Mute\nv[0-30] Set volume\n\ne[N/P/R/J/C/B] Equalizer (N)ormal, (P)op, (R)ock, (J)azz, (C)lassic\nl[A/F/O/R/N]   Loop (A)ll, (R)andom, (F)older, (r)andom In Folder, (O)ne,(N)o Loop\ns[S/B]         Switch to (S)D Card/(B)uilt In Memory\n\n"));
}
