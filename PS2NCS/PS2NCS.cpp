/*
  PS2Keyboard.cpp - PS2Keyboard library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Christian Weichel <info@32leaves.net>

  ** Mostly rewritten Paul Stoffregen <paul@pjrc.com> 2010, 2011
  ** Modified for use beginning with Arduino 13 by L. Abraham Smith, <n3bah@microcompdesign.com> * 
  ** Modified for easy interrup pin assignement on method begin(datapin,irq_pin). Cuningan <cuninganreset@gmail.com> **
  ** Modified for extra keys by Irongeek (at) Irongeek.com
  ** Modified for modifier keys and status LEDs by Michael Drost <sulkith@gmail.com>

  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html
 
  Version klooger Mod (May 2012)
  - merged IG Mod and Ps2KeyboardExt2
  - added support for some special keys to emulate a fully functional keyboard.

  Version IG Mod (Oct 2011)
  - Added some code to tell mod keys for my keylogger, added more keys, and other odds and end.

  Version 2.3 (October 2011)
  - Minor bugs fixed

  Version 2.2 (August 2011)
  - Support non-US keyboards - thanks to Rainer Bruch for a German keyboard :)

  Version 2.1 (May 2011)
  - timeout to recover from misaligned input
  - compatibility with Arduino "new-extension" branch
  - TODO: send function, proposed by Scott Penrose, scooterda at me dot com

  Version 2.0 (June 2010)
  - Buffering added, many scan codes can be captured without data loss
    if your sketch is busy doing other work
  - Shift keys supported, completely rewritten scan code to ascii
  - Slow linear search replaced with fast indexed table lookups
  - Support for Teensy, Arduino Mega, and Sanguino added

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "PS2NCS.h"

//buffer for the key input
#define BUFFER_SIZE 45
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;
static uint16_t state=0;

//Needed to send commands to the keyboard
static volatile uint8_t cmd_value;
static volatile uint8_t cmd_count;
static volatile bool cmd_parity;
static volatile bool cmd_ack_value;
static volatile bool cmd_in_progress;

//These Variables are not gonna change during the program
static uint8_t DataPin;
static uint8_t ClockPin;
static uint8_t CharBuffer=0;
static uint8_t UTF8next=0;
static const PS2Keymap_t *keymap=NULL;

// The ISR for the external interrupt
void ps2interrupt(void)
{
	//Count the sent bits
        static uint8_t cmd_count=0;

	static uint8_t bitcount=0;
	static uint8_t incoming=0;
	static uint32_t prev_ms=0;
	uint32_t now_ms;
	uint8_t n, val;

        if (cmd_in_progress) {
    	cmd_count++;          // cmd_count keeps track of the shifting
    	switch (cmd_count) {
    	case 1: // start bit
      		digitalWrite(DataPin,LOW);
      		break;
    	case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
      		// data bits to shift
      		digitalWrite(DataPin,cmd_value&1);
      		cmd_value = cmd_value>>1;
      		break;
    	case 10:  // parity bit
      		digitalWrite(DataPin,cmd_parity);
      		break;
    	case 11:  // stop bit
      		// release the data pin, so stop bit actually relies on pull-up
      		// but this ensures the data pin is ready to be driven by the kbd for
      		// for the next bit.
      		pinMode(DataPin, INPUT);
      		digitalWrite(DataPin, HIGH);
      		break;
    	case 12: // ack bit - driven by the kbd, so we read its value
      		cmd_ack_value = digitalRead(DataPin);
      		cmd_in_progress = false;  // done shifting out
		//reset sent bits
		cmd_count=0;
    	}
    	return; // don't fall through to the receive section of the ISR
  	}


	//Read the Value on the DataPin
	val = digitalRead(DataPin);

	//Check for timeout
	now_ms = millis();
	if (now_ms - prev_ms > 250) {
		bitcount = 0;
		incoming = 0;
	}
	prev_ms = now_ms;

	//build the recived Data
	//ignore all irrelevant data
	n = bitcount - 1;
	if (n <= 7) {
		incoming |= (val << n);
	}
	bitcount++;

	//Write successfull bit to buffer
	if (bitcount == 11) {
		uint8_t i = head + 1;
		if (i >= BUFFER_SIZE) i = 0;
		if (i != tail) {
			buffer[i] = incoming;
			head = i;
		}
		bitcount = 0;
		incoming = 0;
	}
}

static inline uint8_t get_scan_code(void)
{
	uint8_t c, i;

	i = tail;
	if (i == head) return 0;
	i++;
	if (i >= BUFFER_SIZE) i = 0;
	c = buffer[i];
	tail = i;
	return c;
}

// http://www.quadibloc.com/comp/scan.htm
// http://www.computer-engineering.org/ps2keyboard/scancodes2.html

// These arrays provide a simple key map, to turn scan codes into ISO8859
// output.  If a non-US keyboard is used, these may need to be modified
// for the desired output.
//

const PROGMEM PS2Keymap_t PS2Keymap_US = {
  // without shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
	0, 0, 'z', 's', 'a', 'w', '2', 0,
	0, 'c', 'x', 'd', 'e', '4', '3', 0,
	0, ' ', 'v', 'f', 't', 'r', '5', 0,
	0, 'n', 'b', 'h', 'g', 'y', '6', 0,
	0, 0, 'm', 'j', 'u', '7', '8', 0,
	0, ',', 'k', 'i', 'o', '0', '9', 0,
	0, '.', '/', 'l', ';', 'p', '-', 0,
	0, 0, '\'', 0, '[', '=', 0, 0,
	PS2_CAPSLOCK /*CapsLock*/, PS2_RSHIFT /*Rshift*/, PS2_ENTER /*Enter*/, ']', 0, '\\', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, PS2_NUMLOCK /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
  // with shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '~', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
	0, 0, 'Z', 'S', 'A', 'W', '@', 0,
	0, 'C', 'X', 'D', 'E', '$', '#', 0,
	0, ' ', 'V', 'F', 'T', 'R', '%', 0,
	0, 'N', 'B', 'H', 'G', 'Y', '^', 0,
	0, 0, 'M', 'J', 'U', '&', '*', 0,
	0, '<', 'K', 'I', 'O', ')', '(', 0,
	0, '>', '?', 'L', ':', 'P', '_', 0,
	0, 0, '"', 0, '{', '+', 0, 0,
	PS2_CAPSLOCK /*CapsLock*/, PS2_RSHIFT /*Rshift*/, PS2_ENTER /*Enter*/, '}', 0, '|', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, PS2_NUMLOCK /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
	0
};


const PROGMEM PS2Keymap_t PS2Keymap_German = {
  // without shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '^', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
	0, 0, 'y', 's', 'a', 'w', '2', 0,
	0, 'c', 'x', 'd', 'e', '4', '3', 0,
	0, ' ', 'v', 'f', 't', 'r', '5', 0,
	0, 'n', 'b', 'h', 'g', 'z', '6', 0,
	0, 0, 'm', 'j', 'u', '7', '8', 0,
	0, ',', 'k', 'i', 'o', '0', '9', 0,
	0, '.', '-', 'l', PS2_o_DIAERESIS, 'p', PS2_SHARP_S, 0,
	0, 0, PS2_a_DIAERESIS, 0, PS2_u_DIAERESIS, '\'', 0, 0,
	PS2_CAPSLOCK /*CapsLock*/, PS2_RSHIFT /*Rshift*/, PS2_ENTER /*Enter*/, '+', 0, '#', 0, 0,
	0, '<', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, PS2_NUMLOCK /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
  // with shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, PS2_DEGREE_SIGN, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
	0, 0, 'Y', 'S', 'A', 'W', '"', 0,
	0, 'C', 'X', 'D', 'E', '$', PS2_SECTION_SIGN, 0,
	0, ' ', 'V', 'F', 'T', 'R', '%', 0,
	0, 'N', 'B', 'H', 'G', 'Z', '&', 0,
	0, 0, 'M', 'J', 'U', '/', '(', 0,
	0, ';', 'K', 'I', 'O', '=', ')', 0,
	0, ':', '_', 'L', PS2_O_DIAERESIS, 'P', '?', 0,
	0, 0, PS2_A_DIAERESIS, 0, PS2_U_DIAERESIS, '`', 0, 0,
	PS2_CAPSLOCK /*CapsLock*/, PS2_RSHIFT /*Rshift*/, PS2_ENTER /*Enter*/, '*', 0, '\'', 0, 0,
	0, '>', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, PS2_NUMLOCK /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
	1,
  // with altgr
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 0, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, '@', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_SUPERSCRIPT_TWO, 0,
	0, 0, 0, 0, PS2_CURRENCY_SIGN, 0, PS2_SUPERSCRIPT_THREE, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, PS2_MICRO_SIGN, 0, 0, '{', '[', 0,
	0, 0, 0, 0, 0, '}', ']', 0,
	0, 0, 0, 0, 0, 0, '\\', 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	PS2_CAPSLOCK /*CapsLock*/, PS2_RSHIFT /*Rshift*/, PS2_ENTER /*Enter*/, '~', 0, '’', 0, 0,
	0, '|', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, PS2_NUMLOCK /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 }
};



#define BREAK     0x01
#define MODIFIER  0x02
#define SHIFT_L   0x04
#define SHIFT_R   0x08
#define ALTGR     0x100

static char get_iso8859_code(void)
{
	uint8_t s;
	char c;

	while (1) {
		s = get_scan_code();
		if (!s) return 0;
		if (s == 0xF0) {
			state |= BREAK;
		} else if (s == 0xE0) {
			state |= MODIFIER;
		} else {
			if (state & BREAK) {
				if (s == 0x12) {
					state &= ~SHIFT_L;
				} else if (s == 0x59) {
					state &= ~SHIFT_R;
				} else if (s == 0x11 && (state & MODIFIER)) {
					state &= ~ALTGR;
				} else if (s == 0x14) {
					state &= ~CTL;
				} else if (s == 0x11) {
					state &= ~ALT;
				} else if (s == 0x1F) {
					state &= ~WIN;
				} else if (s == 0x27) {
					state &= ~WIN;
				}
				
				// CTRL, ALT & WIN keys could be added
				// but is that really worth the overhead?
				state &= ~(BREAK | MODIFIER);
				continue;
			}
			if (s == 0x12) {
				state |= SHIFT_L;
				continue;
			} else if (s == 0x59) {
				state |= SHIFT_R;
				continue;
			} else if (s == 0x11 && (state & MODIFIER)) {
				state |= ALTGR;
			} else if (s == 0x14) {
				state |= CTL;
			} else if (s == 0x11) {
				state |= ALT;
			} else if (s == 0x1F) {
				state |= WIN;
			} else if (s == 0x27) {
				state |= WIN;
			}
			c = 0;
			if (state & MODIFIER) {
				state &=~SHIFT_L;
				switch (s) {
				  case 0x70: c = PS2_INSERT;      break;
				  case 0x6C: c = PS2_HOME;        break;
				  case 0x7D: c = PS2_PAGEUP;      break;
				  case 0x71: c = PS2_DELETE;      break;
				  case 0x69: c = PS2_END;         break;
				  case 0x7A: c = PS2_PAGEDOWN;    break;
				  case 0x75: c = PS2_UPARROW;     break;
				  case 0x6B: c = PS2_LEFTARROW;   break;
				  case 0x72: c = PS2_DOWNARROW;   break;
				  case 0x74: c = PS2_RIGHTARROW;  break;
				  case 0x4A: c = '/';             break;
				  case 0x5A: c = PS2_ENTER;       break;
				  case 0x14:
				  if(state & BREAK)
					 state &= ~CTL;
				  else
					 state |= CTL;
				  break;
				  default: break;
				}
			} else if ((state & ALTGR) && keymap->uses_altgr) {
				if (s < PS2_KEYMAP_SIZE)
					c = pgm_read_byte(keymap->altgr + s);
			} else if (((state & (SHIFT_L | SHIFT_R))!=0) ^ ((state & CAPS)!=0)) {
				if (s < PS2_KEYMAP_SIZE)
					c = pgm_read_byte(keymap->shift + s);
			} else {
				if (s < PS2_KEYMAP_SIZE)
					c = pgm_read_byte(keymap->noshift + s);
			}
			state &= ~(BREAK | MODIFIER);
			if (c) return c;
		}
	}
}

static char get_scan_and_set_mod(void)
{
	uint8_t s;
	uint8_t c;

	s = get_scan_code();

	return s;

}
bool PS2Keyboard::available() {
	if (CharBuffer || UTF8next) return true;
	CharBuffer = get_iso8859_code();
	if (CharBuffer) return true;
	return false;
}

int PS2Keyboard::read() {
	uint8_t result;

	result = UTF8next;
	if (result) {
		UTF8next = 0;
	} else {
		result = CharBuffer;
		if (result) {
			CharBuffer = 0;
		} else {
			result = get_iso8859_code();
		}
		if (result >= 161/*128*/) {
			UTF8next = (result & 0x3F) | 0x80;
			result = ((result >> 6) & 0x1F) | 0xC0;
		}
	}
	if (!result) return -1;
	return result;
}

int PS2Keyboard::readraw() {
	uint8_t result;
	result = get_scan_and_set_mod();
	return result;
}
int PS2Keyboard::modkeys() {
    int results=0;
    if((state & SHIFT_L) == int(SHIFT_L)) results +=MODIFIERKEY_SHIFT;
    if((state & SHIFT_R) == int(SHIFT_R)) results +=MODIFIERKEY_SHIFT;

    if((state & CTL) == int(CTL)) results +=MODIFIERKEY_CTRL;
    if((state & ALT) == int(ALT)) results +=MODIFIERKEY_ALT;
    if((state & WIN) == int(WIN)) results +=MODIFIERKEY_GUI;
    return results;
}
PS2Keyboard::PS2Keyboard() {
  // nothing to do here, begin() does it all
}

// sending command bytes to the keybaord needs proper parity (otherwise the keyboard
// just asks you to repeat the byte)
byte odd_parity(byte val) {
  bool parity = true;
  while(val > 0)
  {
	parity = (val & 1)?!parity:parity;
	val = val/2;
  }
  return parity;
}

static void send(char cin)
{
  // stop interrupt routine from receiving characters so that we can use it
  // to send a byte
  cmd_in_progress = true;
  cmd_count       = 0;

  // set up the byte to shift out and initialise the ack bit
  cmd_value      = cin;
  cmd_ack_value  = 1;    // the kbd will clear this bit on receiving the byte
  cmd_parity     = odd_parity(cin);

  // set the data pin as an output, ready for driving
  digitalWrite(DataPin, HIGH);
  pinMode(DataPin, OUTPUT);

  // drive clock pin low - this is going to generate the first
  // interrupt of the shifting out process
  pinMode(ClockPin, OUTPUT);
  digitalWrite(ClockPin, LOW);

  // wait at least one clock cycle (in case the kbd is mid transmission)
  delayMicroseconds(60);

  // set up the 0 start bit
  digitalWrite(DataPin, LOW);
  // let go of clock - the kbd takes over driving the clock from here
  digitalWrite(ClockPin, HIGH);
  pinMode(ClockPin, INPUT);

  // wait for interrupt routine to shift out byte, parity and receive ack bit
  while (cmd_in_progress) ;
  if(cmd_ack_value!=0)
  {
    //send(cin);
  }
 
}

void PS2Keyboard::setLED(char leds) {

  send(0xED);
  cmd_in_progress = false; 

  //Give the Keyboard some time to send ACK
  delay(10);

  send(leds);
  cmd_in_progress = false; 

  //Set my internal CAPS flag
  if(leds & 4)
    state |= CAPS;
  else
    state &= ~CAPS;

  //Give the Keyboard some time to send ACK
  delay(10);

}

void PS2Keyboard::begin(uint8_t data_pin, uint8_t irq_pin, const PS2Keymap_t &map) {
  uint8_t irq_num=0;

  DataPin = data_pin;
  ClockPin = irq_pin;
  keymap = &map;

  // initialize the pins
  pinMode(irq_pin, INPUT);
  digitalWrite(irq_pin, HIGH);
  pinMode(data_pin, INPUT);
  digitalWrite(data_pin, HIGH);
  
  switch(irq_pin) {
    #ifdef CORE_INT0_PIN
    case CORE_INT0_PIN:
      irq_num = 0;
      break;
    #endif
    #ifdef CORE_INT1_PIN
    case CORE_INT1_PIN:
      irq_num = 1;
      break;
    #endif
    #ifdef CORE_INT2_PIN
    case CORE_INT2_PIN:
      irq_num = 2;
      break;
    #endif
    #ifdef CORE_INT3_PIN
    case CORE_INT3_PIN:
      irq_num = 3;
      break;
    #endif
    #ifdef CORE_INT4_PIN
    case CORE_INT4_PIN:
      irq_num = 4;
      break;
    #endif
    #ifdef CORE_INT5_PIN
    case CORE_INT5_PIN:
      irq_num = 5;
      break;
    #endif
    #ifdef CORE_INT6_PIN
    case CORE_INT6_PIN:
      irq_num = 6;
      break;
    #endif
    #ifdef CORE_INT7_PIN
    case CORE_INT7_PIN:
      irq_num = 7;
      break;
    #endif
    default:
      irq_num = 0;
      break;
  }
  head = 0;
  tail = 0;
  attachInterrupt(irq_num, ps2interrupt, FALLING);
}


