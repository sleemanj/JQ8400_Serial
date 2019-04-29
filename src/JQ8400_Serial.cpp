/** 
 * Arduino Library for JQ8400 MP3 Module
 * 
 * Copyright (C) 2019 James Sleeman, <http://sparks.gogo.co.nz/jq6500/index.html>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 * 
 * @author James Sleeman, http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */

#include <Arduino.h>
#include "JQ8400_Serial.h"

void  JQ8400_Serial::play()
{
  this->sendCommand(MP3_CMD_PLAY);
}

void  JQ8400_Serial::restart()
{
  this->sendCommand(MP3_CMD_STOP); // Make sure really will restart
  this->sendCommand(MP3_CMD_PLAY);
}

void  JQ8400_Serial::pause()
{
  this->sendCommand(MP3_CMD_PAUSE);
}

void  JQ8400_Serial::stop()
{
  this->sendCommand(MP3_CMD_STOP);
}

void  JQ8400_Serial::next()
{
  this->sendCommand(MP3_CMD_NEXT);
}

void  JQ8400_Serial::prev()
{
  this->sendCommand(MP3_CMD_PREV);
}

void  JQ8400_Serial::playFileByIndexNumber(uint16_t fileNumber)
{  
  // this->sendCommand(MP3_CMD_PLAY_IDX, (fileNumber>>8) & 0xFF, fileNumber & (byte)0xFF);
  this->sendCommand(MP3_CMD_PLAY_IDX, fileNumber);
}

void  JQ8400_Serial::interjectFileByIndexNumber(uint16_t fileNumber)
{  
  uint8_t buf[3] = { getSource(), (uint8_t)((fileNumber>>8)&0xFF), (uint8_t)(fileNumber & (byte)0xFF) };
  this->sendCommandData(MP3_CMD_INSERT_IDX, buf, 3, 0, 0);
}

void  JQ8400_Serial::seekFileByIndexNumber(uint16_t fileNumber)
{  
  // this->sendCommand(MP3_CMD_SEEK_IDX, (fileNumber>>8) & 0xFF, fileNumber & (byte)0xFF);
  this->sendCommand(MP3_CMD_SEEK_IDX, fileNumber);
}

void JQ8400_Serial::abLoopPlay(uint16_t secondsStart, uint16_t secondsEnd)
{
  uint8_t buf[4] = { (uint8_t)(secondsStart / 60), (uint8_t)(secondsStart % 60), (uint8_t)(secondsEnd / 60), (uint8_t)(secondsEnd % 60) };
  this->sendCommandData(MP3_CMD_AB_PLAY, buf, sizeof(buf), 0, 0);
}

void JQ8400_Serial::abLoopClear()
{
  this->sendCommand(MP3_CMD_AB_PLAY_STOP);
}

void JQ8400_Serial::fastForward(uint16_t seconds)
{
  //this->sendCommand(MP3_CMD_FFWD, (seconds>>8)&0xFF, seconds&0xFF);
  this->sendCommand(MP3_CMD_FFWD, seconds);
}

void JQ8400_Serial::rewind(uint16_t seconds)
{
  //this->sendCommand(MP3_CMD_RWND, (seconds>>8)&0xFF, seconds&0xFF);
  this->sendCommand(MP3_CMD_RWND, seconds);
}

void  JQ8400_Serial::nextFolder()
{
  this->sendCommand(MP3_CMD_NEXT_FOLDER);
}

void  JQ8400_Serial::prevFolder()
{
  this->sendCommand(MP3_CMD_PREV_FOLDER);
}

void  JQ8400_Serial::playFileNumberInFolderNumber(uint16_t folderNumber, uint16_t fileNumber)
{
  // This is kinda weird, the wildcard is *REQUIRED*, without it, it WILL NOT find the file you want.
  //
  // Really Weird.  Anyway this is the format for the data
  //  the first byte is the source (as the byte value not ascii number)
  //  then slash separated path components, with a trailing wildcard fvor each one REQUIRED
  //  the basename of the file must have the wildcard also and the extention must be the 
  //  3 question mark character wildcards, you can't even match on ".mp3", damn this is weird
  
  char buf[] = " /42*/032*???";
  
  buf[0] = this->getSource();
  
  uint8_t i = 2; // 1st digit folder component
  if(folderNumber<10)
  {
    buf[i] = '0';
    i++;
  }
  itoa(folderNumber,&buf[i], 10);
  
  i = 4;
  buf[i] = '*'; // itoa clobbered this with it's null
  
  i = 6; // 1st digit filename component
  if(fileNumber < 100)
  {
    buf[i] = '0';
    i++;
  }
  
  if(fileNumber < 10)
  {
      buf[i] = '0';
      i++;
  }
  itoa(fileNumber, &buf[i], 10);
  
  buf[9] = '*'; // itoa clobbered this with it's null
  
  this->sendCommandData(MP3_CMD_PLAY_FILE_FOLDER, (uint8_t*)buf, sizeof(buf)-1, 0, 0);
}

void  JQ8400_Serial::playInFolderNumber(uint16_t folderNumber)
{
  char buf[] = " /42*/*???";
  
  buf[0] = this->getSource();
  
  uint8_t i = 2; // 1st digit folder component
  if(folderNumber<10)
  {
    buf[i] = '0';
    i++;
  }
  itoa(folderNumber,&buf[i], 10);
  
  i = 4;
  buf[i] = '*'; // itoa clobbered this with it's null
  
  this->sendCommandData(MP3_CMD_PLAY_FILE_FOLDER, (uint8_t*)buf, sizeof(buf)-1, 0, 0);
}

void JQ8400_Serial::playSequenceByFileNumber(uint8_t playList[], uint8_t listLength)
{
  char buf[listLength*2+1]; // itoa will need an extra null
  
  uint8_t i = 0;
  for(uint8_t x = 0; x < listLength; x++)
  {
    if(playList[x]<10)
    {
      buf[i++] = '0';
    }
    itoa(playList[x], &buf[i++], 10);
  }
  
  this->sendCommandData(MP3_CMD_PLAYLIST, (uint8_t *)buf, sizeof(buf)-1, 0, 0);
}

void JQ8400_Serial::playSequenceByFileName(const char * playList[], uint8_t listLength)
{
  char buf[listLength*2];
  
  uint8_t i = 0;
  for(uint8_t x = 0; x < listLength; x++)
  {
    buf[i++] = playList[x][0];
    buf[i++] = playList[x][1];
  }
  
  this->sendCommandData(MP3_CMD_PLAYLIST, (uint8_t *)buf, sizeof(buf), 0, 0);
}

void  JQ8400_Serial::volumeUp()
{
  if(currentVolume < 30) currentVolume++;
  this->sendCommand(MP3_CMD_VOL_UP); // We still send the command just in case we got out of sync somehow
}

void  JQ8400_Serial::volumeDn()
{
  if(currentVolume > 0 ) currentVolume--;
  this->sendCommand(MP3_CMD_VOL_DN); // We still send the command just in case we got out of sync somehow
}

void  JQ8400_Serial::setVolume(byte volumeFrom0To30)
{
  currentVolume = volumeFrom0To30;
  this->sendCommand(MP3_CMD_VOL_SET, volumeFrom0To30);
}

void  JQ8400_Serial::setEqualizer(byte equalizerMode)
{
  currentEq = equalizerMode;
  this->sendCommand(MP3_CMD_EQ_SET, equalizerMode);
}

void  JQ8400_Serial::setLoopMode(byte loopMode)
{
  currentLoop = loopMode;
  this->sendCommand(MP3_CMD_LOOP_SET, loopMode);
}


uint8_t JQ8400_Serial::getAvailableSources() 
{
  return this->sendCommandWithByteResponse(MP3_CMD_GET_SOURCES);
}

void  JQ8400_Serial::setSource(byte source)
{
  this->sendCommand(MP3_CMD_SOURCE_SET, source);
}

uint8_t JQ8400_Serial::getSource() 
{
  return this->sendCommandWithByteResponse(MP3_CMD_GET_SOURCE);
}


void  JQ8400_Serial::sleep()
{
  // The datasheet defined two stop commands but has no sleep command
  //
  // I believe that the device automatically sleeps on stopping.
  //
  //  I have elected to make what looks more like "universal stop" 0x10
  //  to be stop, and have defined for sake of convenience the other stop
  //  command as "RESET", we will issue both to be sure
    
  this->sendCommand(MP3_CMD_SLEEP);
  this->sendCommand(MP3_CMD_STOP);
}

void  JQ8400_Serial::reset()
{
  uint8_t retry = 5; // Try really hard to make ourselves heard.
  do
  {
    // The datasheet defined two stop commands but has no reset command
    //  I have elected to make what looks more like "universal stop" 0x10
    //  to be stop, and have defined for sake of convenience the other stop
    //  command as "RESET", we will issue both to be sure and then 
    //  set things back to "defaults", in absense of an actual reset
    
    this->sendCommand(MP3_CMD_STOP);  delay(1); // There seems to be something
    this->sendCommand(MP3_CMD_RESET); delay(1); //  related to timing here
    
    
    // Reset to the startup defaults
    this->setVolume(20);
    this->setEqualizer(0);
    this->setLoopMode(2);
    this->seekFileByIndexNumber(1);
    this->sendCommand(MP3_CMD_STOP);
    
    uint8_t timeout = 9;
    while(timeout-- > 0 )
    {
      if(getAvailableSources())
      {
        retry = 0;
        break; 
      }
      delay(1);
    }
  }
  while(retry-- > 0);
}


    byte  JQ8400_Serial::getStatus()    
    {
      if(MP3_STATUS_CHECKS_IN_AGREEMENT <= 1)
      {
        return this->sendCommandWithByteResponse(MP3_CMD_STATUS); 
      }
      
      byte statTotal = 0;
      byte stat       = 0;
      do
      {
        statTotal = 0;
        for(byte x = 0; x < MP3_STATUS_CHECKS_IN_AGREEMENT; x++)
        {
          stat = this->sendCommandWithByteResponse(MP3_CMD_STATUS);      
          if(stat == 0) return 0; // STOP is fairly reliable
          statTotal += stat;
        }

      } while (statTotal != 1 * MP3_STATUS_CHECKS_IN_AGREEMENT && statTotal != 2 * MP3_STATUS_CHECKS_IN_AGREEMENT);
      
      return statTotal / MP3_STATUS_CHECKS_IN_AGREEMENT;      
    }
    
    byte  JQ8400_Serial::getVolume()    { return currentVolume; }
    byte  JQ8400_Serial::getEqualizer() { return currentEq;     }
    byte  JQ8400_Serial::getLoopMode()  { return currentLoop;   }
    
    
    uint16_t  JQ8400_Serial::countFiles()   
    {
      return this->sendCommandWithUnsignedIntResponse(MP3_CMD_COUNT_FILES); 
    }
    
    uint16_t  JQ8400_Serial::currentFileIndexNumber()
    {
      return this->sendCommandWithUnsignedIntResponse(MP3_CMD_CURRENT_FILE_IDX); 
    }
    
    uint16_t  JQ8400_Serial::currentFilePositionInSeconds() 
    {
      uint8_t buf[3];
      
      // This turns on continuous position reporting, every second
      this->sendCommandData(MP3_CMD_CURRENT_FILE_POS, 0, 0, buf, 3);
      
      // Stop it doing that
      this->sendCommand(MP3_CMD_CURRENT_FILE_POS_STOP);
      
      return (buf[0]*60*60) + (buf[1]*60) + buf[2];
    }
    
    uint16_t  JQ8400_Serial::currentFileLengthInSeconds()   
    {
      uint8_t buf[3];
      
      this->sendCommandData(MP3_CMD_CURRENT_FILE_LEN, 0, 0, buf, 3);
      
      return (buf[0]*60*60) + (buf[1]*60) + buf[2];
      
      return 0; /* FIXME this->sendCommandWithUnsignedIntResponse(MP3_CMD_CURRENT_FILE_LEN_SEC); */ 
    }
    
    void          JQ8400_Serial::currentFileName(char *buffer, uint16_t bufferLength) 
    {
      // this->sendCommand(MP3_CMD_CURRENT_FILE_NAME, 0, 0, buffer, bufferLength);
      this->sendCommand(MP3_CMD_CURRENT_FILE_NAME, (uint8_t *)buffer, bufferLength);
      buffer[bufferLength-1] = 0; // Ensure null termination since this is a string.
    }
    
    // Used for the status commands, they mostly return an 8 to 16 bit integer 
    // and take no arguments
    uint16_t JQ8400_Serial::sendCommandWithUnsignedIntResponse(byte command)
    {      
      uint8_t buffer[4];
      this->sendCommand(command, buffer, sizeof(buffer));
      return ((uint8_t)buffer[0]<<8) | ((uint8_t)buffer[1]);
    }
    
    uint8_t JQ8400_Serial::sendCommandWithByteResponse(uint8_t command)
    {
      uint8_t response = 0;
      this->sendCommand(command, &response, 1);
      return response;
    }
    
    void  JQ8400_Serial::sendCommandData(uint8_t command, uint8_t *requestBuffer, uint8_t requestLength, uint8_t *responseBuffer, uint8_t bufferLength)
    {
      // Calculate the checksum which forms the end byte
      uint8_t MP3_CHECKSUM = MP3_CMD_BEGIN + command + requestLength;
      
      for(uint8_t x = 0; x < requestLength; x++)
      {
        MP3_CHECKSUM += (uint8_t)requestBuffer[x];
      }
      
#if MP3_DEBUG
      Serial.println();
      
      HEX_PRINT(MP3_CMD_BEGIN);  Serial.print(" ");
      HEX_PRINT(command);        Serial.print(" ");
      HEX_PRINT(requestLength);  Serial.print(" ");
      
      for(uint8_t x = 0; x < requestLength; x++)
      {
          HEX_PRINT(requestBuffer[x]); 
          Serial.print(' ');
      }
      
      HEX_PRINT(MP3_CHECKSUM);  Serial.print(" ");
#endif
      
      // If there is any random garbage on the line, clear that out now.
      while(this->waitUntilAvailable(10)) this->_Serial->read();

      this->_Serial->write(MP3_CMD_BEGIN);
      this->_Serial->write(command);
      this->_Serial->write(requestLength);
      for(uint8_t x = 0; x < requestLength; x++)
      {
          this->_Serial->write(requestBuffer[x]);
      }
      this->_Serial->write(MP3_CHECKSUM);
            
      if(responseBuffer && bufferLength) 
      {
        memset(responseBuffer, 0, bufferLength);
      }
      
      // If we don't expect a response (or don't care) don't wait for ones
      else
      {
        return;
      }
      
      // Allow some time for the device to process what we did and 
      // respond, up to 1 second, but typically only a few ms.
      this->waitUntilAvailable(1000);

      
#if MP3_DEBUG
      Serial.print(" ==> [");
#endif
      
      // The response format is the same as the command format
      //  AA [CMD] [DATA_COUNT] [B1..N] [SUM]
      MP3_CHECKSUM = 0;
      
      uint8_t      i = 0;
      uint8_t      j = 0;
      uint8_t      dataCount = 0;
      while(this->waitUntilAvailable(150))
      {
        j = this->_Serial->read();
                
#if MP3_DEBUG
        HEX_PRINT(j); Serial.print(" ");
#endif
        if(i == 2)
        {
          // The number of data bytes to read
          dataCount = j;
        }
        
        // We only record the data bytes so bytes 0,1 and 2 are discarded
        //   except for calculating checksum
        if(i <= 2) 
        {
          MP3_CHECKSUM += j;
          i++;
          continue;
        }
        else
        {
          if(dataCount > 0)
          {
            // This is a databyte to read
            if((i-3) <= (bufferLength-1))
            {
              responseBuffer[i-3] = j;
            }
            i++;
            dataCount--;
            MP3_CHECKSUM += j;
          }
          else
          {
            // This is the checksum byte
            if((MP3_CHECKSUM & 0xFF) != j)
            {
              // Checksum Failed
              #if MP3_DEBUG
                Serial.print(" ** CHECKSUM FAILED " );
                HEX_PRINT((MP3_CHECKSUM & 0xFF)); 
                Serial.print(" != ");
                HEX_PRINT(j); 
              #endif
              memset(responseBuffer, 0, bufferLength);
            }
            else
            {
               #if MP3_DEBUG
                Serial.print(" ** CHECKSUM OK " );
                HEX_PRINT((MP3_CHECKSUM & 0xFF)); 
                Serial.print(" == ");
                HEX_PRINT(j); 
              #endif
            }
          }
        }
      }
      
#if MP3_DEBUG      
      Serial.print("] --> ");
      for(uint8_t x = 0; x < bufferLength; x++)
      {
        HEX_PRINT(responseBuffer[x]);
      }
      
      Serial.println();
#endif
      
    }
    

// Waits until data becomes available, or a timeout occurs
int JQ8400_Serial::waitUntilAvailable(uint16_t maxWaitTime)
{
  uint32_t startTime;
  int c = 0;
  startTime = millis();
  do {
    c = this->_Serial->available();
    if (c) break;
  } while(millis() - startTime < maxWaitTime);
  
  return c;
}
