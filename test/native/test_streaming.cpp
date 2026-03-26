#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>

#include <Streaming.h>

namespace
{
class AssertPrint : public Print
{
public:
  size_t write(uint8_t b) override
  {
    buffer_.push_back(static_cast<char>(b));
    return 1;
  }

  void reset()
  {
    buffer_.clear();
  }

  const std::string &buffer() const
  {
    return buffer_;
  }

  void expect(const char *expected)
  {
    if (buffer_ != expected)
    {
      fprintf(stderr, "Expected [%s] but got [%s]\n", expected, buffer_.c_str());
    }
    assert(buffer_ == expected);
    reset();
  }

  void expect(const __FlashStringHelper *expected)
  {
    expect(reinterpret_cast<const char *>(expected));
  }

private:
  std::string buffer_;
};

class PrintableTest : public Printable
{
public:
  size_t printTo(Print &p) const override
  {
    return p.print("printed");
  }
};

class NoCopyConstructorType : public Printable
{
public:
  NoCopyConstructorType() = default;

  size_t printTo(Print &p) const override
  {
    return p.print("NoCopyCtr");
  }

private:
  NoCopyConstructorType(NoCopyConstructorType &) = delete;
};
}

int main()
{
  AssertPrint out;
  const char abc[] = "abc";
  double v = 1.23456789012345;

  out << 1;
  out.expect("1");
  out << 10;
  out.expect("10");
  out << -5;
  out.expect("-5");
  out << 'a';
  out.expect("a");
  out << 1.0;
  out.expect("1.00");
  out << 1.005;
  out.expect("1.00");
  out << -10.23;
  out.expect("-10.23");
  out << "abc";
  out.expect("abc");
  out << abc;
  out.expect("abc");
  out << F("abc");
  out.expect("abc");
  out << String("string");
  out.expect("string");
  out << PrintableTest();
  out.expect("printed");

  out << 1 << '2' << 0x3 << "4" << F("5");
  out.expect("12345");

  out << _DEC(1);
  out.expect("1");
  out << _DEC(-1);
  out.expect("-1");
  out << _DEC((int8_t)-1);
  out.expect("-1");
  out << _DEC((uint8_t)-1);
  out.expect("255");
  out << _DEC((int16_t)-1);
  out.expect("-1");
  out << _DEC((uint16_t)-1);
  out.expect("65535");
  out << _DEC((int32_t)-1);
  out.expect("-1");
  out << _DEC((uint32_t)-1);
  out.expect("4294967295");
  out << _HEX(15);
  out.expect("F");
  out << _OCT(15);
  out.expect("17");
  out << _BIN(15);
  out.expect("1111");
  out << _BYTE(64);
  out.expect("@");

  out << _FLOAT(v, 1);
  out.expect("1.2");
  out << _FLOAT(v, 2);
  out.expect("1.23");
  out << _FLOAT(v, 3);
  out.expect("1.235");
  out << _FLOAT(v, 4);
  out.expect("1.2346");
  out << _FLOAT(v, 5);
  out.expect("1.23457");
  out << _FLOAT(v, 6);
  out.expect("1.234568");
  out << _FLOAT(1.7649E22, 5);
  out.expect("ovf");

  out << 1 << endl;
  out.expect("1\r\n");

  out << _PAD(10, ' ');
  out.expect("          ");
  out << _PAD(4, '0');
  out.expect("0000");
  out << _PAD(20, '=');
  out.expect("====================");

  out << _WIDTH(1, 5);
  out.expect("    1");
  out << _WIDTH(-1, 5);
  out.expect("   -1");
  out << _WIDTHZ(1, 5);
  out.expect("00001");
  out << _WIDTHZ(128, 5);
  out.expect("00128");
  out << _WIDTHZ(_HEX(128), 5);
  out.expect("00080");
  out << _WIDTH(abc, 5);
  out.expect("  abc");
  out << _WIDTH(F("one"), 5);
  out.expect("  one");
  out << _WIDTH((int8_t)-20, 11);
  out.expect("        -20");
  out << _WIDTH((int8_t)-128, 11);
  out.expect("       -128");
  out << _WIDTH((uint32_t)-1, 11);
  out.expect(" 4294967295");
  out << _WIDTH((int16_t)-32768, 11);
  out.expect("     -32768");
  out << _WIDTH((int32_t)-2147483647L - 1L, 11);
  out.expect("-2147483648");

  out << _WIDTH(_FLOAT(-9.99, 1), 11);
  out.expect("      -10.0");
  out << _WIDTH(-3.14159, 11);
  out.expect("      -3.14");
  out << _WIDTH((float)12.566, 11);
  out.expect("      12.57");

  out << _FLOATW(-1, 2, 11);
  out.expect("      -1.00");
  out << _FLOATW(1.23456e4, 1, 11);
  out.expect("    12345.6");
  out << _FLOATW(-3.1415926, 6, 11);
  out.expect("  -3.141593");
  out << _FLOATW(-9.999999, 4, 11);
  out.expect("   -10.0000");
  out << _FLOATW(-0.0, 1, 11);
  out.expect("        0.0");
  out << _FLOATW(-0.01, 1, 11);
  out.expect("       -0.0");
  out << _FLOATW(-0.01, 0, 11);
  out.expect("         -0");
  out << _FLOATW(1.4, 0, 11);
  out.expect("          1");
  out << _FLOATW(0. / 0., 2, 11);
  out.expect("        nan");
  out << _FLOATW(-1. / 0., 2, 11);
  out.expect("        inf");
  out << _FLOATW(1.e10, 2, 11);
  out.expect("        ovf");

  out << _FMT("Hello % the time is %:%:%", "gazoodle", _WIDTHZ(1, 2), _WIDTHZ(4, 2), _WIDTHZ(8, 2));
  out.expect("Hello gazoodle the time is 01:04:08");
  out << _FMT("Too many % % % for the parms", 1);
  out.expect("Too many 1 % % for the parms");
  out << _FMT("Too few % for the parms", 1, 2);
  out.expect("Too few 1 for the parms");
  out << _FMT("Output a \\% sign");
  out.expect("Output a % sign");
  out << _FMT("Handle trailing \\");
  out.expect("Handle trailing ");
  out << _FMT(F("Replace % with %"), 1, 2);
  out.expect("Replace 1 with 2");
  out << _FMT("Your score is %\\%", 65.3);
  out.expect("Your score is 65.30%");
  out << _FMT("Once % a %%%", F("upon"), 't', 1, "me");
  out.expect("Once upon a t1me");

  out
    << (int8_t)1
    << _BYTE(50)
    << _HEX(3)
    << _FLOAT(4, 0)
    << _PAD(1, '5')
    << _WIDTH(6, 1)
    << _FMT("%", 7)
    << endl;
  out.expect("1234567\r\n");

  out << NoCopyConstructorType();
  out.expect("NoCopyCtr");

  out << "Value:" , NoCopyConstructorType();
  out.expect("Value: NoCopyCtr");

  out << "Data:" << 1, 3, 5, 7, 9;
  out.expect("Data:1 3 5 7 9");

  return 0;
}
