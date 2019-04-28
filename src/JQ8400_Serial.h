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

#ifndef JQ8400Serial_h
#define JQ8400Serial_h

#include <SoftwareSerial.h>

#define MP3_EQ_NORMAL     0
#define MP3_EQ_POP        1
#define MP3_EQ_ROCK       2
#define MP3_EQ_JAZZ       3
#define MP3_EQ_CLASSIC    4

#define MP3_SRC_USB       0
#define MP3_SRC_SDCARD    1
#define MP3_SRC_FLASH     2
#define MP3_SRC_BUILTIN   MP3_SRC_FLASH

// Loop Options
//   ALL = all tracks loop, ALL_STOP all tracks then stop, ALL_RANDOM all tracks randomly
//   ONE = one track loop, ONE_STOP (default) one track then stop
//   FOLDER = tracks within folder loop, FOLDER_STOP tracks within folder then stop, FOLDER_RANDOM random in folder

#define MP3_LOOP_ALL             0
#define MP3_LOOP_ALL_STOP        7
#define MP3_LOOP_ALL_RANDOM      3

#define MP3_LOOP_ONE             1
#define MP3_LOOP_ONE_STOP        2

#define MP3_LOOP_FOLDER          4
#define MP3_LOOP_FOLDER_RANDOM   5
#define MP3_LOOP_FOLDER_STOP     6

#define MP3_LOOP_NONE            2 

#define MP3_STATUS_STOPPED 0
#define MP3_STATUS_PLAYING 1
#define MP3_STATUS_PAUSED  2

// The response from a status query could be unreliable
//  we can increase this to check multiple times.
#define MP3_STATUS_CHECKS_IN_AGREEMENT 1

#define MP3_DEBUG 1

#define HEX_PRINT(a) if(a < 16) Serial.print(0); Serial.print(a, HEX);

class JQ8400_Serial : public SoftwareSerial
{
  
  public: 

    /** Create JQ8400 object.
     * 
     * Example, create global instance:
     * 
     *     JQ8400_Serial mp3(8,9);
     * 
     * For a 5v Arduino:
     * -----------------
     *  * TX on JQ8400 connects to D8 on the Arduino
     *  * RX on JQ8400 connects to one end of a 1k resistor,
     *      other end of resistor connects to D9 on the Arduino
     * 
     * For a 3v3 Arduino:
     * -----------------
     *  * TX on JQ8400 connects to D8 on the Arduino
     *  * RX on JQ8400 connects to D9 on the Arduino
     * 
     * Of course, power and ground are also required, VCC on JQ8400 is 5v tolerant (but RX isn't totally, hence the resistor above).
     * 
     * And then you can use in your setup():
     * 
     *     mp3.begin(9600)
     *     mp3.reset();
     *
     * and all the other commands :-)
     */
    
    JQ8400_Serial(short rxPin, short txPin) : SoftwareSerial(rxPin,txPin) { };
    
    /** Start playing the current file, if paused the playing is resumed.
     * 
     *  If stopped or playing the playing is started from beginning.
     * 
     */
    
    void play();
    
    /** Restart the current track from the beginning.
     * 
     */
    
    void restart();
    
    /** Pause the current file.  To unpause, use play(),
     *  to unpause and go back to beginning of track use restart()
     */
    
    void pause();
    
    /** Stop the current playing (if any).
     */
    
    void stop();
    
    /** Fast Forward by a number of seconds (default 5).
     */
    
    void fastForward(uint16_t seconds = 5);
    
    
    /** Rewind  by a number of seconds (default 5).
     */
    
    void rewind(uint16_t seconds = 5);
    
    /** Play the next file.
     */
    
    void next();
    
    /** Play the previous file.
     */
    
    void prev();
    
    /** Play the next folder. 
     */
    
    void nextFolder();
    
    /** Play the previous folder. 
     */
    
    void prevFolder();
    
    /** Play a specific file based on it's (FAT table) index number.
     * 
     *  Note that the index number has nothing to do with the file name (except if you 
     *  uploaded/copied them to the media in order of file name).
     * 
     *  To sort your SD Card FAT table, search for a FAT sorting utility for your operating system 
     *  of choice.  Specifically on Linux fatsort is the tool you want but be sure to get a recent
     *  version which supports FAT-12 that the JQ8400 use for flash memory.
     * 
     *  https://sourceforge.net/projects/fatsort/
     * 
     *  @param fileNumber FAT Table index of the file to play next.
     */
    
    void playFileByIndexNumber(unsigned int fileNumber);        
    
    /** Interject the currently playing file (if any) with the given index number file.
     * 
     *  Like a special announcement comes over, it interrupts the 
     *   music playing, when the announcement finishes, the music continues.
     * 
     *  Note that the index number has nothing to do with the file name (except if you 
     *  uploaded/copied them to the media in order of file name).
     * 
     *  To sort your SD Card FAT table, search for a FAT sorting utility for your operating system 
     *  of choice.  Specifically on Linux fatsort is the tool you want but be sure to get a recent
     *  version which supports FAT-12 that the JQ8400 use for flash memory.
     * 
     *  https://sourceforge.net/projects/fatsort/
     * 
     *  @param fileNumber FAT Table index of the file to play next.
     */
    
    void interjectFileByIndexNumber(unsigned int fileNumber);        
    
    
    /** Play a specific file in a specific folder based on the name of those folder and file.
     *
     * To use this function, folders must be named from 00 to 99, and the files in those folders
     * must be named from 000.mp3 to 999.mp3
     * 
     * So to play the file "/03/006.mp3" use mp3.playFileNumberInFolderNumber(3, 6);
     * 
     * Note that zero padding of your folder and file names is required - "01/002.mp3" good, "1/2.mp3" bad.
     * 
     * @param folderNumber 0 to 99
     * @param fileNumber  0 to 999
     */
    
    void playFileNumberInFolderNumber(unsigned int folderNumber, unsigned int fileNumber);
    
    /** Play the first (?) file in a specific folder 00 to 99.
     *
     * To use this function, folders must be named from 00 to 99.
     * 
     * So to play the folder "/03" use mp3.playInFolderNumber(3);
     * 
     * Note that zero padding of your folder and file required - "01/002.mp3" good, "1/2.mp3" bad.
     * 
     * @param folderNumber 0 to 99
     * 
     */
    
    void playInFolderNumber(unsigned int folderNumber);
    
    
    /** Seek to a specific file based on it's (FAT table) index number.  
     * 
     *  The file will not start playing until you issue `play()`
     * 
     *  Note that any currently playing file will stop immediately.
     * 
     *  It seems (undocumented) that seeking to a track will wake up the device's output
     *   and keep it awake until you play or stop (or it stops after playing), you may wish
     *   to issue a sleep() after seeking.
     * 
     *  Note that the index number has nothing to do with the file name (except if you 
     *  uploaded/copied them to the media in order of file name).
     * 
     *  To sort your SD Card FAT table, search for a FAT sorting utility for your operating system 
     *  of choice.  Specifically on Linux fatsort is the tool you want but be sure to get a recent
     *  version which supports FAT-12 that the JQ8400 use for flash memory.
     * 
     *  https://sourceforge.net/projects/fatsort/
     * 
     *  @param fileNumber FAT Table index of the file to play next.
     */
    
    void seekFileByIndexNumber(unsigned int fileNumber);
    
    /** A-B Loop for the file currently playing.
     * 
     *  For the **track that is already playing (and not paused)** loop play between two marks defined by a start and end second.
     * 
     *  If you want to start a file in an ab loop, start it playing and immediately issue the abLoopPlay.
     *  
     *     mp3.playFileByIndexNumber(1);
     *     mp3.abLoopPlay(25,50);
     * 
     *  will work, similarly if you want to resume from a pause into an ab loop
     *  
     *    mp3.play();
     *    mp3.abLoopPlay(25,50);
     * 
     *  if you pause in an abLoop when you play again the abLoop is terminated.
     * 
     * There seems to be some sort of odd granularity with this function of the device, 
     *   the first time through the ab-loop the start point is sometimes different
     *   to subsequent loops through, perhaps a second "out", that is if you specify
     *   start at 25 seconds, the first loop might start at 24, and subsequent ones at 
     *   25 (or vice-versa).  In short, experimentation might be needed here.
     * 
     * @param secondsStart Second to set as start marker.
     * @param secondsEmd Second to set as end marker.
     * 
     */
    
    void abLoopPlay(uint16_t secondsStart, uint16_t secondsEnd);
    
    /** Use this to break a currently running A-B loop.
     * 
     * Pause, stop, play will also break the A-B loop, this will do it 
     *  without requiring any break the play will just continue through 
     *  the previously set end marker without looping.
     * 
     */
    
    void abLoopClear();
    
    /** Increase the volume by 1 (volume ranges 0 to 30). */
    
    void volumeUp();
    
    /** Decrease the volume by 1 (volume ranges 0 to 30). */
    
    void volumeDn();
    
    /** Set the volume to a specific level (0 to 30). 
     * 
     * @param volumeFrom0To30 Level of volume to set from 0 to 30
     */
    
    void setVolume(byte volumeFrom0To30);
    
    /** Set the equalizer to one of 6 preset modes.
     * 
     * @param equalizerMode One of the following, 
     * 
     *  *  MP3_EQ_NORMAL
     *  *  MP3_EQ_POP        
     *  *  MP3_EQ_ROCK       
     *  *  MP3_EQ_JAZZ       
     *  *  MP3_EQ_CLASSIC    
     * 
     */
    
    void setEqualizer(byte equalizerMode); // EQ_NORMAL to EQ_BASS
    
    /** Set the looping mode.
     * 
     * @param loopMode One of the following,
     * 
     *  *  MP3_LOOP_NONE          - No loop, just play one file and then stop. (aka MP3_LOOP_ONE_STOP)
     *  *  MP3_LOOP_ONE           - Loop one file over and over and over again.
     * 
     *  *  MP3_LOOP_ALL           - Play through all files and repeat.
     *  *  MP3_LOOP_ALL_STOP      - Play through all files and stop.
     *  *  MP3_LOOP_ALL_RANDOM    - Play all files randomly (continuously)
     * 
     *  *  MP3_LOOP_FOLDER        - Play through all files in the same folder and repeat.
     *  *  MP3_LOOP_FOLDER_STOP   - Play through all files in the same folder and stop
     *  *  MP3_LOOP_FOLDER_RANDOM - Play through all files in the same folder randomly (continuously)
     * 
     *  Note that the random options are perhaps not that optimal especially with low
     *   file counts, a track playing twice back to back is not unusual, and the randomness
     *   does not anecdotally seem all that random.  (NB: These random options are implemented
     *   inside the JQ8400 device itself, not in code, you could make your own randomiser
     *   and just use play by index etc...)
     * 
     */
    
    void setLoopMode(byte loopMode);
    
    /** Set the source to read mp3 data from.  Note that the datasheet calls this "drive".
     * 
     *  @param source One of the following...
     * 
     *   * MP3_SRC_BUILTIN    - Files read from the on-board flash memory
     *   * MP3_SRC_SDCARD     - Files read from the SD Card
     *   * MP3_SRC_USB        - Files from a connected USB device?  I have not seen modules capable of this, but possible?
     */
    
    void setSource(byte source);
    
    /** Return the currently selected source.
     * 
     *  @return One of the following...
     * 
     *   * MP3_SRC_BUILTIN    - Files read from the on-board flash memory
     *   * MP3_SRC_SDCARD     - Files read from the SD Card
     *   * MP3_SRC_USB        - Files from a connected USB device?  I have not seen modules capable of this, but possible?
     */
    
    uint8_t getSource();
        
    /** Return boolean indicating if the given source is available (can be selected using `setSource()`)
     * 
     * @param  source One of the following     
     *   * MP3_SRC_BUILTIN    - Files read from the on-board flash memory
     *   * MP3_SRC_SDCARD     - Files read from the SD Card
     *   * MP3_SRC_USB        - Files from a connected USB device?  I have not seen modules capable of this, but possible?
     * 
     * @return bool
     */
    
    uint8_t sourceAvailable(uint8_t source)
    {
      return getAvailableSources() & 1<<source;
    }
    
    /** Put the device to sleep.
     *
     *  This will stop all playing.  When you play() again it will be 
     *  from the beginning of the current track.
     * 
     *  Note that the JQ8400 seems to automatically sleep when it 
     *  stops playing **except** when you seek to a track (or pause).
     * 
     */
    
    void sleep();
    
    /** Reset the device (softly).
     *       
     * It may be necessary in practice to actually power-cycle the device
     * in case it gets confused or something.
     * 
     * So if designing a PCB/circuit including JQ8400 modules it might be 
     * worth while to include such ability (ie, power the device through 
     * a MOSFET which you can turn on/off at will).
     * 
     */
    
    void reset();
    
    /** Get the status from the device.
     * 
     * @return One of MP3_STATUS_PAUSED, MP3_STATUS_PLAYING and MP3_STATUS_STOPPED
     */
    
    byte getStatus();
    
    /** Return if the device is busy (playing) or not.
     * 
     *  Just a convenience test, equivalent to `getStatus() == MP3_STATUS_PLAYING`
     * 
     * @return bool
     */
    
    uint8_t busy() { return getStatus() == MP3_STATUS_PLAYING; }
    
    /** Get the current volume level.
     * 
     * @return Value between 0 and 30
     */
    
    byte getVolume();
    
    /** Get the equalizer mode.
     * 
     * @return One of the following, 
     * 
     *  *  MP3_EQ_NORMAL
     *  *  MP3_EQ_POP        
     *  *  MP3_EQ_ROCK       
     *  *  MP3_EQ_JAZZ       
     *  *  MP3_EQ_CLASSIC    
     *  *  MP3_EQ_BASS      
     */
    
    byte getEqualizer();
    
    /** Get loop mode.
     * 
     * @return One of the loop modes as follows...
     * 
     *  *  MP3_LOOP_NONE          - No loop, just play one file and then stop. (aka MP3_LOOP_ONE_STOP)
     *  *  MP3_LOOP_ONE           - Loop one file over and over and over again.
     * 
     *  *  MP3_LOOP_ALL           - Play through all files and repeat.
     *  *  MP3_LOOP_ALL_STOP      - Play through all files and stop.
     *  *  MP3_LOOP_ALL_RANDOM    - Play all files randomly (continuously)
     * 
     *  *  MP3_LOOP_FOLDER        - Play through all files in the same folder and repeat.
     *  *  MP3_LOOP_FOLDER_STOP   - Play through all files in the same folder and stop
     *  *  MP3_LOOP_FOLDER_RANDOM - Play through all files in the same folder randomly (continuously)
     */
    
    byte getLoopMode();
    
    
    /** Count the number of files on the current media.
     * 
     * @return Number of files present on that media.
     * 
     */
    
    unsigned int   countFiles();    
    
    /** For the currently playing (or paused, or file that would be played 
     *  next if stopped) file, return the file's (FAT table) index number.
     * 
     *  This number can be used with playFileByIndexNumber();
     * 
     *  @return Number of file.
     */
    
    unsigned int   currentFileIndexNumber();
    
    /** For the currently playing or paused file, return the 
     *  current position in seconds.
     * 
     * @return Number of seconds into the file currently played.
     * 
     */
    
    unsigned int   currentFilePositionInSeconds();
    
    /** For the currently playing or paused file, return the 
     *  total length of the file in seconds.
     * 
     * @return Length of audio file in seconds.
     */
    
    unsigned int   currentFileLengthInSeconds();
    
    /** Get the name of the "current" file.
     *
     * The name returned is shortened significantly to 8+3 format without
     * a dot, all caps... in other words, this isn't very useful probably.
     * 
     * The current file is the one that is playing, paused, or if stopped then
     * could be next to play or last played, uncertain.
     *
     */
    
    void           currentFileName(char *buffer, unsigned int bufferLength);    
        
    /** Play a sequence of files, which must all exist in a folder called "ZH" and be named 00.mp3 through 99.mp3
     * 
     *  Don't ask me why the folder must be called "ZH", that's what the JQ8400 wants.
     * 
     * @param playList An array of the numbers of files in the "ZH" folder.
     *   For example, if your ZH folder contains 01.mp3, 02.mp3 and 03.mp3 and you want 
     *   to play them in the order 03.mp3, 01.mp3, 02.mp3 then your array should be
     *   
     *     uint8_t playList[] = { 3, 1, 2 };
     * 
     * @param listLength          Number of filenames in the list.
     * 
     */
    
    void playSequenceByFileNumber(uint8_t playList[], uint8_t listLength);
    
    /** Play a sequence of files, which must all exist in a folder called "ZH" and have 2 character names.
     * 
     *  Don't ask me why the folder must be called "ZH", that's what the JQ8400 wants.
     * 
     *  Example files: ZH/A1.mp3 , ZH/1B.mp3, ZH/AZ.mp3
     * 
     * @param playList An array of the two character names.
     *   
     *    const char * fileNames[] = { "DD", "CC", "BB", "AA" };
     *    mp3.playSequenceByFileName(fileNames, sizeof(fileNames)/sizeof(char *));
     * 
     * @param listLength          Number of filenames in the list.
     * 
     */
    
    void playSequenceByFileName(const char *playList[], uint8_t listLength);
    
    
    
  protected:

    /** Send a command to the JQ8400 module, 
     * 
     * @param command        Byte value of to send as from the datasheet.
     * @param requestBuffer  Pointer to (or NULL) request data bytes.
     * @param requestLength  Number of bytes in the request buffer.
     * @param responseBuffer Buffer to store a single line of response, if NULL, no response is read.  Note that the response is NOT a null-terminated string, if you want that, do it yourself (and specify length-1).
     * @param buffLength     Length of response buffer.
     * 
     */
    
    void sendCommandData(uint8_t command, uint8_t *requestBuffer, uint8_t requestLength, uint8_t *responseBuffer, uint8_t bufferLength);
    
    /** Send a command with no arguments and no response. 
     * 
     * @param command       Byte value of to send as from the datasheet.
     */
    
    inline void sendCommand(uint8_t command, uint8_t *responseBuffer = 0, uint8_t bufferLength = 0) 
    { 
      sendCommandData(command, NULL,  0, responseBuffer, bufferLength);
    }
    
    /** Send a command with a single 8 bit argument and no response. 
     * 
     * @param command       Byte value of to send as from the datasheet.
     * @param arg           Single byte of data
     */
    
    inline void sendCommand(uint8_t command, uint8_t arg, uint8_t *responseBuffer = 0, uint8_t bufferLength = 0)
    { 
      sendCommandData(command, &arg, 1, responseBuffer, bufferLength); 
    }
    
    /** Send a command with a 16 bit integer argument.
     *      * 
     * @param command       Byte value of to send as from the datasheet.
     */
    
    inline void sendCommand(uint8_t command, uint16_t arg, uint8_t *responseBuffer = 0, uint8_t bufferLength = 0) 
    { 
      #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__    
        sendCommandData(command, ((uint8_t *)(&arg)), 2, responseBuffer, bufferLength);
      #else
        uint8_t buf[] = { *(((uint8_t *)(&arg))+1), *((uint8_t *)(&arg)) };
        sendCommandData(command, buf, 2, responseBuffer, bufferLength);
      #endif
    }
        
    /** Send a command to the JQ8400 module, and get a 16 bit integer response.
     * 
     * @param command        Byte value of to send as from the datasheet.
     * @return Response from module.
     */
    
    unsigned int sendCommandWithUnsignedIntResponse(byte command);

    /** Send a command to the JQ8400 module, and get an 8 bit integer response.
     * 
     * @param command        Byte value of to send as from the datasheet.
     * @return Response from module.
     */
    
    uint8_t sendCommandWithByteResponse(uint8_t command);

    
    /** Return a bitmask of the available sources.
     * 
     * @return Bitmask indicating which sources are connected to the module.
     * 
     *    bit 0 = USB
     *    bit 1 = SD
     *    bit 2 = FLASH
     */
    
    uint8_t getAvailableSources();
    
    
    /** Read bytes until and including a terminator character.
     * 
     * Note that no null termination is added, do it yourself.
     * 
     * @param terminator Character to indicate end of input.
     * @param buffer     Buffer to place received characters.
     * @param length     Length of buffer.
     * @param maxOneLineOnly boolean to indicate if can span lines.
     *
     * @return Number of bytes read.
     */
    
    size_t readBytesUntilAndIncluding(char terminator, char *buffer, size_t length, byte maxOneLineOnly = 0);
    
    /** Blocking wait with a timeout for serial input.
     * 
     * @param maxWaitTime Milliseconds
     * @return bool Available (True) / Timed Out (False)
     */
    
    int    waitUntilAvailable(unsigned long maxWaitTime = 1000);
    
    
    /** 
     * The JQ8400 does not have commands to query the volume, eq or playing mode
     *   so we have to keep track of that ourselves unfortunately.
     */
    
    uint8_t currentVolume = 20;
    uint8_t currentEq     = 0;
    uint8_t currentLoop   = 2;
    
    
    static const uint8_t MP3_CMD_BEGIN = 0xAA;
    
    static const uint8_t MP3_CMD_PLAY = 0x02;
    static const uint8_t MP3_CMD_PAUSE = 0x03;
    
    static const uint8_t MP3_CMD_FFWD  = 0x23;
    static const uint8_t MP3_CMD_RWND  = 0x22;
        
    static const uint8_t MP3_CMD_STOP = 0x10; // Not sure, maybe 0x04?
    
    static const uint8_t MP3_CMD_NEXT = 0x06;
    static const uint8_t MP3_CMD_PREV = 0x05;
    static const uint8_t MP3_CMD_PLAY_IDX = 0x07;
    static const uint8_t MP3_CMD_SEEK_IDX = 0x1F;
    static const uint8_t MP3_CMD_INSERT_IDX = 0x16;
    
    static const uint8_t MP3_CMD_AB_PLAY      = 0x20;
    static const uint8_t MP3_CMD_AB_PLAY_STOP = 0x21;
        
    static const uint8_t MP3_CMD_NEXT_FOLDER = 0x0F;
    static const uint8_t MP3_CMD_PREV_FOLDER = 0x0E;
    
      static const uint8_t MP3_CMD_PLAY_FILE_FOLDER = 0x08; // FIXME
    
    static const uint8_t MP3_CMD_VOL_UP = 0x14;
    static const uint8_t MP3_CMD_VOL_DN = 0x15;
    static const uint8_t MP3_CMD_VOL_SET = 0x13;
    
    static const uint8_t MP3_CMD_EQ_SET = 0x1A;
    static const uint8_t MP3_CMD_LOOP_SET = 0x18;    
    static const uint8_t MP3_CMD_SOURCE_SET = 0x0B;
    
    static const uint8_t MP3_CMD_SLEEP = 0x04;    // I am not sure about these, see implmentation of sleep() and reset()
    static const uint8_t MP3_CMD_RESET = 0x04;    //  what I have done seems to work maybe, maybe.

    static const uint8_t MP3_CMD_STATUS = 0x01;
    
    static const uint8_t MP3_CMD_GET_SOURCES = 0x09;
    static const uint8_t MP3_CMD_GET_SOURCE  = 0x0A;
        
    static const uint8_t MP3_CMD_COUNT_FILES     = 0x0C; 
    static const uint8_t MP3_CMD_COUNT_IN_FOLDER = 0x12; 
    
    static const uint8_t MP3_CMD_CURRENT_FILE_IDX         = 0x0D; 
    static const uint8_t MP3_CMD_FIRST_FILE_IN_FOLDER_IDX = 0x11; 
    
    static const uint8_t MP3_CMD_CURRENT_FILE_LEN = 0x24;
    static const uint8_t MP3_CMD_CURRENT_FILE_POS = 0x25; // This turns on continuous reporting of position
    static const uint8_t MP3_CMD_CURRENT_FILE_POS_STOP = 0x26; // This stops that
    static const uint8_t MP3_CMD_CURRENT_FILE_NAME = 0x1E;
    
    static const uint8_t MP3_CMD_PLAYLIST = 0x1B;
};

#endif