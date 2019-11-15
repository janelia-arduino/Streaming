/*
  Streaming.h - Arduino library for supporting the << streaming operator
  Copyright (c) 2010-2012 Mikal Hart.  All rights reserved.

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

/*
  Version 6 library changes
    Copyright (c) 2019 Gazoodle. All rights reserved.

  1.  _BASED moved to template to remove type conversion to long and
      sign changes which break int8_t and int16_t negative numbers. 
      The print implementation still upscales to long for it's internal
      print routine.

  2.  _PAD added to allow padding & filling of characters to the stream

  3.  _WIDTH & _WIDTHZ added to allow width printing with space padding
      and zero padding for numerics

*/

#ifndef ARDUINO_STREAMING
#define ARDUINO_STREAMING

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <type_traits>

#define STREAMING_LIBRARY_VERSION 6

#if !defined(typeof)
#define typeof(x) __typeof__(x)
#endif

// Generic template
template<class T>
inline Print &operator <<(Print &stream, T arg)
{ stream.print(arg); return stream; }

template<typename T>
struct _BASED
{
  T val;
  int base;
  _BASED(T v, int b): val(v), base(b)
  {}
};

#if ARDUINO >= 100

struct _BYTE_CODE
{
  byte val;
  _BYTE_CODE(byte v) : val(v)
  {}
};
#define _BYTE(a)    _BYTE_CODE(a)

inline Print &operator <<(Print &obj, const _BYTE_CODE &arg)
{ obj.write(arg.val); return obj; }

#else

#define _BYTE(a)    _BASED<typeof(a)>(a, BYTE)

#endif

#define _HEX(a)     _BASED<typeof(a)>(a, HEX)
#define _DEC(a)     _BASED<typeof(a)>(a, DEC)
#define _OCT(a)     _BASED<typeof(a)>(a, OCT)
#define _BIN(a)     _BASED<typeof(a)>(a, BIN)

// Specialization for class _BASED
// Thanks to Arduino forum user Ben Combee who suggested this
// clever technique to allow for expressions like
//   Serial << _HEX(a);

template<typename T>
inline Print &operator <<(Print &obj, const _BASED<T> &arg)
{ obj.print(arg.val, arg.base); return obj; }

#if ARDUINO >= 18
// Specialization for class _FLOAT
// Thanks to Michael Margolis for suggesting a way
// to accommodate Arduino 0018's floating point precision
// feature like this:
//   Serial << _FLOAT(gps_latitude, 6); // 6 digits of precision

struct _FLOAT
{
  float val;
  int digits;
  _FLOAT(double v, int d): val(v), digits(d)
  {}
};

inline Print &operator <<(Print &obj, const _FLOAT &arg)
{ obj.print(arg.val, arg.digits); return obj; }
#endif

// Specialization for enum _EndLineCode
// Thanks to Arduino forum user Paul V. who suggested this
// clever technique to allow for expressions like
//   Serial << "Hello!" << endl;

enum _EndLineCode { endl };

inline Print &operator <<(Print &obj, _EndLineCode arg)
{ obj.println(); return obj; }

// Specialization for padding & filling, mainly utilized
// by the width printers
//
//  Use like 
//    Serial << _PAD(10,' ');   // Will output 10 spaces
//    Serial << _PAD(4, '0');   // Will output 4 zeros
struct _PAD
{
    int8_t width;
    char   chr;
    _PAD(int8_t w, char c) : width(w), chr(c) {}
};

inline Print &operator <<(Print& stm, const _PAD &arg)
{
    for(int8_t i = 0; i < arg.width; i++)
        stm.print(arg.chr);
    return stm;
}

// Specialization for width printing
//
//    Use like                            Result
//    --------                            ------
//    Serial << _WIDTH(1,5)               "    1"
//    Serial << _WIDTH(10,5)              "   10"
//    Serial << _WIDTH(100,5)             "  100"
//    Serial << _WIDTHZ(1,5)              "00001"
//
//  Great for times & dates, or hex dumps
//
//    Serial << _WIDTHZ(hour,2) << ':' << _WIDTHZ(min,2) << ':' << _WIDTHZ(sec,2)
//
//    for(int index=0; index<byte_array_size; index++)
//      Serial << _WIDTHZ(_HEX(byte_array[index]))

template<typename T, int8_t WIDTH, char PAD = ' '>
struct __WIDTH
{
    const T& val;
    __WIDTH(const T& v) : val(v) {}
};

//  Count digits in an integer of specific base
template<typename T>
inline uint8_t digits(T v, int8_t base = 10)
{
    uint8_t digits = 0;
    if ( std::is_signed<T>::value )
    {
        if ( v < 0 )
          digits++;
    }
    do
    {
        v /= base;
        digits++;
    } while( v > 0 );
    return digits;
}

// Generic get the width of a value in base 10
template<typename T>
inline uint8_t get_value_width(T val)
{ return digits(val); }

inline uint8_t get_value_width(const char * val)
{ return strlen(val); }

inline uint8_t get_value_width(const __FlashStringHelper * val)
{ return strlen_P(reinterpret_cast<const char *>(val)); }

// _BASED<T> get the width of a value
template<typename T>
inline uint8_t get_value_width(_BASED<T> b)
{ return digits(b.val, b.base); }

// Macros to hide the template requirement specification
#define _WIDTH(a,w)       __WIDTH<typeof(a), w>(a)
#define _WIDTHZ(a,w)      __WIDTH<typeof(a), w, '0'>(a)

// Operator overload to handle width printing.
template<typename T, int8_t WIDTH, char PAD>
inline Print &operator <<(Print &stm, const __WIDTH<T, WIDTH, PAD> &arg)
{ stm << _PAD(WIDTH-get_value_width(arg.val), PAD); stm << arg.val; return stm; }

#endif
