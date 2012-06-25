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
#include <usb_api.h> //include the file containing the Serial class

#define LOGGING_ON

#ifndef CORE_TEENSY_SERIAL
#undef LOGGING_ON
#endif

#ifdef LOGGING_ON
  #define LOG(msg) Serial.print(msg);
  #define LOGF(msg,fmt) Serial.print(msg,fmt);
  #define LOGLN(msg) Serial.println(msg);
  #define IFLOG(pgrm) pgrm
#else
  #define LOG(msg)
  #define LOGF(msg,fmt)
  #define LOGLN(msg)
  #define IFLOG(pgrm)
#endif
