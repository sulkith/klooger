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
 * This file contains the definition of the TestInput class
 * The TestInput class is for debugging without a keyboard only
 */
#ifndef __TESTINPUT__
#define __TESTINPUT__
#include "input.h"
class TestInput: public Input
{
  public:
    void setLedStatus(int ledsUSB){}
    bool available()
    {
      const int period = 10000;
      static int i = 0;
      i++;
      return (i%period)==0;
    }
    char read()
    {
      return KEY_B;
    }
    int getModKeys()
    {
      return MODIFIERKEY_SHIFT;
    }
};
#endif /*__TESTINPUT__*/
