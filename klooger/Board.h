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
#ifndef __BOARD__
#define __BOARD__

#ifdef __AVR_AT90USB646__
#define Teensypp
#elif __AVR_AT90USB1286__
#define Teensypp
#elif AVR_ATmega32U4__
#define Teensy
#endif

//PS2Keyboard with the selfmade adapter
// Clock - The Black Cable D2
// Data - The Red Cable D3
// +5V - The Brown Cable
// GND - The Orange Cable

#ifdef Teensypp
//#######################################
//# Wiring Section for the Teensy++ 1&2 #
//#######################################

/* Definitions with no need to define
  I2C_SCL --> PD0
  I2C_SDA --> PD1
  
  SPI MISO --> PB3
  SPI_MOSI --> PB2
  SPI_SCLK --> PB1
  SPI_SS   --> PB0
*/
#define ledPin 6   // Teensy has LED on 11, Teensy++ on 6
#define menuPin PIN_F0;



#define ps2ClkPin PIN_D2
#define ps2DataPin PIN_D3


#elif Teensy
//#####################################
//# Wiring Section for the Teensy 2.0 #
//#####################################

/* Definitions with no need to define
  I2C_SCL --> PD0
  I2C_SDA --> PD1
  
  SPI MISO --> PB3
  SPI_MOSI --> PB2
  SPI_SCLK --> PB1
  SPI_SS   --> PB0
*/
#define ledPin 11 // Teensy has LED on 11, Teensy++ on 6
#define menuPin PIN_F0;
#define ps2ClkPin PIN_D2
#define ps2DataPin PIN_D3

#endif



#endif //__BOARD__
