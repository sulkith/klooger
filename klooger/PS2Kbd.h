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
 * This file contains the declaration of the PS2Kbd class
 */
#ifndef __PS2KBD_CLASS__
#define __PS2KBD_CLASS__

#include "input.h"

class PS2Kbd: public Input
{
  public:
    void setLedStatus(int ledsUSB);
    bool available();
    char read();
    int getModKeys();
    void initialize();
};

#endif
