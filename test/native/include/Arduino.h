// Minimal Arduino compatibility shim for native unit tests.
#ifndef STREAMING_TEST_NATIVE_ARDUINO_H
#define STREAMING_TEST_NATIVE_ARDUINO_H

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>

#ifndef DEC
#define DEC 10
#endif
#ifndef HEX
#define HEX 16
#endif
#ifndef OCT
#define OCT 8
#endif
#ifndef BIN
#define BIN 2
#endif

using byte = uint8_t;

class __FlashStringHelper;

#define F(str) reinterpret_cast<const __FlashStringHelper *>(PSTR(str))
#define PSTR(str) (str)

inline size_t strlen_P(const char *str)
{
  return strlen(str);
}

inline int memcmp_P(const void *lhs, const void *rhs, size_t size)
{
  return memcmp(lhs, rhs, size);
}

inline char pgm_read_byte(const __FlashStringHelper *ptr)
{
  return *reinterpret_cast<const char *>(ptr);
}

class String
{
public:
  String() = default;
  String(const char *value) : value_(value ? value : "") {}

  const char *c_str() const
  {
    return value_.c_str();
  }

private:
  std::string value_;
};

class Print;

class Printable
{
public:
  virtual ~Printable() = default;
  virtual size_t printTo(Print &p) const = 0;
};

class Print
{
public:
  virtual ~Print() = default;

  virtual size_t write(uint8_t value) = 0;

  virtual size_t write(const uint8_t *buffer, size_t size)
  {
    size_t count = 0;
    for (size_t i = 0; i < size; ++i)
    {
      count += write(buffer[i]);
    }
    return count;
  }

  size_t write(const char *str)
  {
    return write(reinterpret_cast<const uint8_t *>(str), strlen(str));
  }

  size_t print(const char *value)
  {
    return write(value ? value : "");
  }

  size_t print(char value)
  {
    return write(static_cast<uint8_t>(value));
  }

  size_t print(unsigned char value, int base = DEC)
  {
    return printNumber(value, base);
  }

  size_t print(int value, int base = DEC)
  {
    return printSigned(value, base);
  }

  size_t print(unsigned int value, int base = DEC)
  {
    return printNumber(value, base);
  }

  size_t print(long value, int base = DEC)
  {
    return printSigned(value, base);
  }

  size_t print(unsigned long value, int base = DEC)
  {
    return printNumber(value, base);
  }

  size_t print(long long value, int base = DEC)
  {
    return printSigned(value, base);
  }

  size_t print(unsigned long long value, int base = DEC)
  {
    return printNumber(value, base);
  }

  size_t print(double value, int digits = 2)
  {
    if (std::isnan(value))
    {
      return print("nan");
    }
    if (std::isinf(value))
    {
      return print("inf");
    }
    if (value > 4294967040.0 || value < -4294967040.0)
    {
      return print("ovf");
    }

    if (digits < 0)
    {
      digits = 0;
    }

    size_t count = 0;

    if (value < 0.0)
    {
      count += print('-');
      value = -value;
    }

    double rounding = 0.5;
    for (int i = 0; i < digits; ++i)
    {
      rounding /= 10.0;
    }
    value += rounding;

    unsigned long long int_part = static_cast<unsigned long long>(value);
    double remainder = value - static_cast<double>(int_part);

    count += printNumber(int_part, DEC);

    if (digits == 0)
    {
      return count;
    }

    count += print('.');

    while (digits-- > 0)
    {
      remainder *= 10.0;
      int to_print = static_cast<int>(remainder);
      count += print(static_cast<char>('0' + to_print));
      remainder -= to_print;
    }

    return count;
  }

  size_t print(const String &value)
  {
    return print(value.c_str());
  }

  size_t print(const Printable &value)
  {
    return value.printTo(*this);
  }

  size_t print(const __FlashStringHelper *value)
  {
    return print(reinterpret_cast<const char *>(value));
  }

  size_t println()
  {
    return print("\r\n");
  }

private:
  template<typename T>
  size_t printSigned(T value, int base)
  {
    if (base == 0)
    {
      return write(static_cast<uint8_t>(value));
    }
    if (base == DEC && value < 0)
    {
      size_t count = print('-');
      using Unsigned = std::make_unsigned_t<T>;
      Unsigned magnitude = static_cast<Unsigned>(-(value + 1)) + 1;
      return count + printNumber(magnitude, base);
    }
    return printNumber(static_cast<std::make_unsigned_t<T>>(value), base);
  }

  template<typename T>
  size_t printNumber(T value, int base)
  {
    if (base < 2)
    {
      base = 10;
    }

    char buffer[8 * sizeof(T) + 1];
    size_t pos = sizeof(buffer) - 1;
    buffer[pos] = '\0';

    do
    {
      T digit = value % static_cast<T>(base);
      buffer[--pos] = static_cast<char>(digit < 10 ? '0' + digit : 'A' + (digit - 10));
      value /= static_cast<T>(base);
    } while (value > 0);

    return write(buffer + pos);
  }
};

#endif
