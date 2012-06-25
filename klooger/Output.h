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
 * This file contains the following classes:
 * [Output] generic output Interface. abstract class.
 * [LoggingOutput] adds some Funtions to the OutputClass to read the logged Text.
 * [NoOutput] empty Output class does nothing. NoOutput can also be used as LoggingOutput
 */
#ifndef __Output__
#define __Output__
#include "Debug.h"

#define MAXKEYLENGTH 20

#define CASE_KEY(c) case KEY_##c: return #c;
#define CASE_SKEY(c, str) case KEY_##c: return str;
#define CASE_KEYPAD(c) case KEYPAD_##c: return dblquote(Keypad_##c);
#define dblquote(s) #s

class Output
{
  public:
  virtual int getLedStatus(){}
  virtual void print(const char c) = 0;
  virtual void prints(const char* const c)
  {
    int i = 0;
    while(c[i] != '\0')
    {
      this->print(c[i]);
    }
  }
  virtual void printKey(const int modifier, const char key)
  {
    char cout[MAXKEYLENGTH];
    char* c = convertKeyCode(key);
    int pos = 0;
    //MODIFIERKEY_CTRL	Control Key
    //MODIFIERKEY_SHIFT	Shift Key
    //MODIFIERKEY_ALT	Alt Key
    //MODIFIERKEY_GUI	Windows (PC) or Clover (Mac)
    if(modifier & MODIFIERKEY_CTRL)
      cout[pos++] = 'C';
    if(modifier & MODIFIERKEY_ALT)
      cout[pos++] = 'A';
    if(modifier & MODIFIERKEY_GUI)
      cout[pos++] = 'W';
    if(modifier & MODIFIERKEY_SHIFT)
      cout[pos++] = 'S';
    
    if(pos != 0)
      cout[pos++] = '^';
    
    //Copy key code to 
    int posc = 0;
    while(c[posc] !='\0')
    {
      cout[pos++] = c[posc++];
    }
    
    cout[pos++] = ' ';
    cout[pos] = '\0';
    prints(cout);
  }
  char* convertKeyCode(const char key)
  {
    switch(key)
    {
      CASE_KEY(A)
      CASE_KEY(B)
      CASE_KEY(C)
      CASE_KEY(D)
      CASE_KEY(E)
      CASE_KEY(F)
      CASE_KEY(G)
      CASE_KEY(H)
      CASE_KEY(I)
      CASE_KEY(J)
      CASE_KEY(K)
      CASE_KEY(L)
      CASE_KEY(M)
      CASE_KEY(N)
      CASE_KEY(O)
      CASE_KEY(P)
      CASE_KEY(Q)
      CASE_KEY(R)
      CASE_KEY(S)
      CASE_KEY(T)
      CASE_KEY(U)
      CASE_KEY(V)
      CASE_KEY(W)
      CASE_KEY(X)
      CASE_KEY(Y)
      CASE_KEY(Z)
      CASE_KEY(0)
      CASE_KEY(1)
      CASE_KEY(2)
      CASE_KEY(3)
      CASE_KEY(4)
      CASE_KEY(5)
      CASE_KEY(6)
      CASE_KEY(7)
      CASE_KEY(8)
      CASE_KEY(9)
      CASE_KEY(F1)
      CASE_KEY(F2)
      CASE_KEY(F3)
      CASE_KEY(F4)
      CASE_KEY(F5)
      CASE_KEY(F6)
      CASE_KEY(F7)
      CASE_KEY(F8)
      CASE_KEY(F9)
      CASE_KEY(F10)
      CASE_KEY(F11)
      CASE_KEY(F12)
      CASE_KEY(TAB)
      CASE_KEY(SPACE)
      CASE_KEY(ENTER)
      CASE_KEY(BACKSPACE)
      CASE_KEY(ESC)
      CASE_KEY(CAPS_LOCK)
      CASE_KEY(NUM_LOCK)
      CASE_KEY(SCROLL_LOCK)
      CASE_KEY(INSERT)
      CASE_KEY(HOME)
      CASE_KEY(PAGE_UP)
      CASE_KEY(PAGE_DOWN)
      CASE_KEY(DELETE)
      CASE_KEY(UP)
      CASE_KEY(DOWN)
      CASE_KEY(RIGHT)
      CASE_KEY(LEFT)
      CASE_KEY(END)
      CASE_KEY(COMMA)
      CASE_KEY(PERIOD)
      
      //Language specific keys
      CASE_SKEY(TILDE,"^")
      CASE_SKEY(SLASH,"-")
      CASE_SKEY(SEMICOLON,"Ö")
      CASE_SKEY(MINUS,"ß")
      CASE_SKEY(QUOTE, "Ä")
      CASE_SKEY(LEFT_BRACE, "Ü")
      CASE_SKEY(EQUAL,"´")
      CASE_SKEY(RIGHT_BRACE, "+")
      
      
      CASE_KEYPAD(1)
      CASE_KEYPAD(2)
      CASE_KEYPAD(3)
      CASE_KEYPAD(4)
      CASE_KEYPAD(5)
      CASE_KEYPAD(6)
      CASE_KEYPAD(7)
      CASE_KEYPAD(8)
      CASE_KEYPAD(9)
      CASE_KEYPAD(0)
      CASE_KEYPAD(PLUS)
      CASE_KEYPAD(MINUS)
      CASE_KEYPAD(ASTERIX)
      CASE_KEYPAD(PERIOD)
    }
    return "";
  }
};

class LoggingOutput: public Output{
public:
  virtual void print(char c)=0;
  virtual void prints(const char* c)=0;
  virtual void printToOutput(Output* out)=0;
  virtual void clear()=0;
};

class NoOutput: public LoggingOutput{
public:
  virtual void print(char c){};
};

#endif //__Output__
