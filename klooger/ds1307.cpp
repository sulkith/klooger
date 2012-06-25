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
 * This file contains the definition of the ds1307 class
 */
#include "ds1307.h"
#include <Wire.h>

/* Registers of ds1307
/--------------------------------------------------------\
| BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT 0 |
----------------------------------------------------------
|!RUN  |    10 SECONDS      |      SECONDS               |
----------------------------------------------------------
|  0   |    10 MINUTES      |      MINUTES               |
----------------------------------------------------------
|  0   |12/24 |10/A/P| 10H  |      HOURS                 |
----------------------------------------------------------
|  0   |  0   |  0   |  0   |  0   |  DAY OF WEEK        |
----------------------------------------------------------
|  0   |  0   |  10 DATE    |      DATE                  |
----------------------------------------------------------
|  0   |  0   |  0   | 10M  |      MONTH                 |
----------------------------------------------------------
|      10 YEAR              |      YEAR                  |
----------------------------------------------------------
| OUT  |  0   |  0   | SQWE |  0   |  0   |  RS1 |  RS2  |
\--------------------------------------------------------/
*/

TwoWire RTC;

uint8_t ds1307::bcdtobin(const uint8_t bcd)
{
  uint8_t bin = bcd & 0x0F;
  bin += 10 * ((bcd & 0xF0)>>4);
  return bin;
}
uint8_t ds1307::bintobcd(const uint8_t bin)
{
  uint8_t bcd = (bin % 10);
  bcd |= ((bin / 10) % 10) << 4;
  return bcd;
}

void ds1307::initialize()
{
  RTC.begin();
}
void ds1307::settime(const uint8_t d, const uint8_t m, const uint8_t y, const uint8_t h, const uint8_t min, const uint8_t s)
{
  uint8_t seconds = bintobcd(s)&0x7F;
  uint8_t minutes = bintobcd(min)&0x7F;
  uint8_t hours   = bintobcd(h)&0x3F;
  uint8_t date    = bintobcd(d)&0x3F;
  uint8_t month   = bintobcd(m)&0x1F;
  uint8_t year    = bintobcd(y)&0xFF;
  
  RTC.beginTransmission(DS1307ADDRESS);
  RTC.send(0);
  RTC.send(seconds);
  RTC.send(minutes);
  RTC.send(hours);
  
  //the day of week isnt needed
  RTC.send(0);
  RTC.send(date);
  RTC.send(month);
  RTC.send(year);
  RTC.endTransmission();
}
void ds1307::gettime(uint8_t& d, uint8_t& m, uint8_t& y, uint8_t& h, uint8_t& min, uint8_t& s)
{
  RTC.beginTransmission(DS1307ADDRESS);
  RTC.send(0);
  RTC.endTransmission();
  
  RTC.requestFrom(DS1307ADDRESS,7);  
  
  uint8_t byte = 0;
  while(RTC.available())
  {
    char c = RTC.read();
    switch(byte)
    {
      case 0: //Seconds
        s = bcdtobin(c&0x7F);
        break;
      case 1: //minutes
        min = bcdtobin(c&0x7F);
        break;
      case 2: //hours
          h=0;
          if(c&0b00100000)
            h += ((c&0b01000000))?12:20; //12 or 24 hour mode
          h += ((c&0b00010000)>>4)*10;
          h += bcdtobin(c&0x0F);
          break;
      case 3: break; //day of wek is irrelevant
      case 4:
        d = bcdtobin(c&0x3F);
      case 5:
        m = bcdtobin(c&0x1F);
      case 6:
        y = bcdtobin(c&0xFF);
      default:
          break;
    }
    byte++;
    //wait a short moment to recive the next byte
    //delay(10);
  }
}
