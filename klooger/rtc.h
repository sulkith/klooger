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
 * This file contains the declaration of the rtc class - abstract
 * And the class NoRtc which is an empty implementation of the abstract rtc class
 */
#ifndef __RTC__
#define __RTC__
#include <inttypes.h>
#include "Debug.h"

class rtc
{
public:
  virtual void settime(const uint8_t d, const uint8_t m, const uint8_t y, const uint8_t h, const uint8_t min, const uint8_t s)=0;
  virtual void gettime(uint8_t& d, uint8_t& m, uint8_t& y, uint8_t& h, uint8_t& min, uint8_t& s)=0;
};

class NoRtc : public rtc
{
  virtual void settime(const uint8_t d, const uint8_t m, const uint8_t y, const uint8_t h, const uint8_t min, const uint8_t s)
  {
    LOGLN("using NoRtc");
  };
  virtual void gettime(uint8_t& d, uint8_t& m, uint8_t& y, uint8_t& h, uint8_t& min, uint8_t& s)
  {
    LOGLN("using NoRtc");
    d = 99;
    m = 99;
    y = 99;
    h = 99;
    min = 99;
    s = 99;
  };
};

#endif/*__RTC__*/
