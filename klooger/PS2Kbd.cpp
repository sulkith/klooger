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
 * This file contains the definition of the PS2Kbd class
 */
#include <PS2NCS.h>
#include "PS2Kbd.h"
#include "Debug.h"
#include "Board.h"
#include "keymap.h"

static PS2Keyboard hwkbd;

void PS2Kbd::setLedStatus(int ledsUSB)
{
  static int ledsUSBold=0;
  if(ledsUSBold!=ledsUSB)
  {

    int ledsPS2 =  ((ledsUSB & 4) >> 2) + ((ledsUSB & 3) << 1);
    hwkbd.setLED(ledsPS2);
    ledsUSBold = ledsUSB;
  }
}
bool PS2Kbd::available()
{
  return hwkbd.available();
}
char PS2Kbd::read()
{
  return hwkbd.read();
}
int PS2Kbd::getModKeys()
{
  return hwkbd.modkeys();
}
void PS2Kbd::initialize()
{
  hwkbd.begin(ps2DataPin,ps2ClkPin,PS2Keymap_German_Code);
}
