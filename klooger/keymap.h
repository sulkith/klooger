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
 * This file contains the keymap for using the UsbKeyboardOutput with the PS2NCS Library
 */
#ifndef __KEYMAP__
#define __KEYMAP__
const PROGMEM PS2Keymap_t PS2Keymap_German_Code = {
  // without shift
        {0, KEY_F9, 0, KEY_F5, KEY_F3, KEY_F1, KEY_F2, KEY_F12,
        0, KEY_F10, KEY_F8, KEY_F6, KEY_F4, KEY_TAB, KEY_TILDE, 0,
        0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, KEY_Q, KEY_1, 0,
        0, 0, KEY_Z, KEY_S, KEY_A, KEY_W, KEY_2, 0,
        0, KEY_C, KEY_X, KEY_D, KEY_E, KEY_4, KEY_3, 0,
        0, KEY_SPACE, KEY_V, KEY_F, KEY_T, KEY_R, KEY_5, 0,
        0, KEY_N, KEY_B, KEY_H, KEY_G, KEY_Y, KEY_6, 0,
        0, 0, KEY_M, KEY_J, KEY_U, KEY_7, KEY_8, 0,
        0, KEY_COMMA, KEY_K, KEY_I, KEY_O, KEY_0, KEY_9, 0,
        0, KEY_PERIOD, KEY_SLASH, KEY_L, KEY_SEMICOLON, KEY_P, KEY_MINUS, 0,
        0, 0, KEY_QUOTE, 0, KEY_LEFT_BRACE, KEY_EQUAL, 0, 0,
        KEY_CAPS_LOCK /*CapsLock*/, PS2_RSHIFT /*Rshift*/, KEY_ENTER /*Enter*/, KEY_RIGHT_BRACE, 0, '\'', 0, 0,
        0, 0/*'<' this key doesn't exist on an american keyboard so it wont work*/, 0, 0, 0, 0, KEY_BACKSPACE, 0,
        0, KEYPAD_1, 0, KEYPAD_4, KEYPAD_7, 0, 0, 0,
        KEYPAD_0, KEYPAD_PERIOD, KEYPAD_2, KEYPAD_5, KEYPAD_6, KEYPAD_8, KEY_ESC, KEY_NUM_LOCK /*NumLock*/,
        KEY_F11, KEYPAD_PLUS, KEYPAD_3, KEYPAD_MINUS, KEYPAD_ASTERIX, KEYPAD_9, KEY_SCROLL_LOCK, 0,
        0, 0, 0, KEY_F7 },
  // with shift
        {0, KEY_F9, 0, KEY_F5, KEY_F3, KEY_F1, KEY_F2, KEY_F12,
        0, KEY_F10, KEY_F8, KEY_F6, KEY_F4, KEY_TAB, KEY_TILDE, 0,
        0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, KEY_Q, KEY_1, 0,
        0, 0, KEY_Z, KEY_S, KEY_A, KEY_W, KEY_2, 0,
        0, KEY_C, KEY_X, KEY_D, KEY_E, KEY_4, KEY_3, 0,
        0, KEY_SPACE, KEY_V, KEY_F, KEY_T, KEY_R, KEY_5, 0,
        0, KEY_N, KEY_B, KEY_H, KEY_G, KEY_Y, KEY_6, 0,
        0, 0, KEY_M, KEY_J, KEY_U, KEY_7, KEY_8, 0,
        0, KEY_COMMA, KEY_K, KEY_I, KEY_O, KEY_0, KEY_9, 0,
        0, KEY_PERIOD, KEY_SLASH, KEY_L, KEY_SEMICOLON, KEY_P, KEY_MINUS, 0,
        0, 0, KEY_QUOTE, 0, KEY_LEFT_BRACE, KEY_EQUAL, 0, 0,
        KEY_CAPS_LOCK /*CapsLock*/, PS2_RSHIFT /*Rshift*/, KEY_ENTER /*Enter*/, KEY_RIGHT_BRACE, 0, '\'', 0, 0,
        0, 0/*< this key doesn't exist on an american keyboard so it wont work*/, 0, 0, 0, 0, KEY_BACKSPACE, 0,
        0, KEYPAD_1, 0, KEYPAD_4, KEYPAD_7, 0, 0, 0,
        KEYPAD_0, KEYPAD_PERIOD, KEYPAD_2, KEYPAD_5, KEYPAD_6, KEYPAD_8, KEY_ESC, KEY_NUM_LOCK /*NumLock*/,
        KEY_F11, KEYPAD_PLUS, KEYPAD_3, KEYPAD_MINUS, KEYPAD_ASTERIX, KEYPAD_9, KEY_SCROLL_LOCK, 0,
        0, 0, 0, KEY_F7 },
        0,
};
#endif /*__KEYMAP__*/
