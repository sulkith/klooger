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
 * This file contains the declaration of the ds1307 class
 */
#ifndef __DS1307__
#define __DS1307__

#include "rtc.h"

#define DS1307ADDRESS 0x68

class ds1307 : public rtc
{
public:
  virtual void settime(const uint8_t d, const uint8_t m, const uint8_t y, const uint8_t h, const uint8_t min, const uint8_t s);
  virtual void gettime(uint8_t& d, uint8_t& m, uint8_t& y, uint8_t& h, uint8_t& min, uint8_t& s);
  virtual void initialize();
private:
  uint8_t bcdtobin(uint8_t bcd);
  uint8_t bintobcd(uint8_t bin);
};

#endif /*__DS1307__*/
