#include <Streaming.h>

const long BAUD = 115200;

const int lettera = 'A';
const int month = 4, day = 17, year = 2009;
const int hour = 8, minute = 20, second = 3;
const float pi = 3.14159;

char arrowStart_full[] = " full: >>>";
char arrowStart_part[] = " partial: >>>";
char arrowEnd[] = "<<<";
char textABC_sizeAuto[] = "ABC";
char textABC_size3[3]   = {'A','B','C'};
char textABC_size5[5]   = "ABC";
char textAZ_size32[32] = "abcdefghijklmnopqrstuvwxyz";
char textAZ16_size32[32] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','1','2','3','4','5','6'};

const long LOOP_DELAY = 1000;

void setup() { Serial.begin(BAUD); }

void loop() {
  Serial << "This is an example of the new streaming" << endl;
  Serial << "library.  This allows you to print variables" << endl;
  Serial << "and strings without having to type line after" << endl;
  Serial << "line of Serial.print() calls.  Examples: " << endl;

  Serial << "A is " << lettera << "." << endl;
  Serial << "The current date is " << day << "-" << month << "-" << year << "."
         << endl;

  Serial << "You can use modifiers too, for example:" << endl;
  Serial << _BYTE(lettera) << " is " << _HEX(lettera) << " in hex. " << endl;
  Serial << endl;

  Serial << "In library v6, you also get" << endl;
  Serial << "Padding [" << _PAD(10, '*') << "]" << endl;
  Serial << "Width specification [" << _WIDTH(10, 5) << "]" << endl;
  Serial << "Leading zeros [" << _WIDTHZ(month, 2) << "]" << endl;
  Serial << _FMT("Format strings for stuff like dates and times %/%/% %:%:%",
                 _WIDTHZ(day, 2), _WIDTHZ(month, 2), year, _WIDTHZ(hour, 2),
                 _WIDTHZ(minute, 2), _WIDTHZ(second, 2))
         << endl;
  Serial << _FMT(F("To reduce your % size, these % can be in %"), F("sketch"),
                 F("constants"), F("PROGMEM"))
         << endl;
  Serial << endl;

  Serial << "pi to 3 digits = " << _FLOAT(pi, 3) << endl;

  // comma separator
  int f = 11;
  Serial << "Data: " << 1, 3, 5, 7, 9, f;
  Serial << endl;
  Serial << "comma separator works!" << endl;
  Serial << endl;

  Serial << "Print char arrays:" << endl;
  Serial << F("textABC_sizeAuto: ArrayLen=") << sizeof (textABC_sizeAuto) << arrowStart_full << _BUFPART (textABC_sizeAuto, sizeof (textABC_sizeAuto)) << arrowEnd << arrowStart_part << _BUFPART (textABC_sizeAuto, strnlen (textABC_sizeAuto, sizeof (textABC_sizeAuto))) <<  arrowEnd << endl;
  Serial << F("textABC_size3:    ArrayLen=") << sizeof (textABC_size3   ) << arrowStart_full << _BUFPART (textABC_size3   , sizeof (textABC_size3   )) << arrowEnd << arrowStart_part << _BUFPART (textABC_size3   , strnlen (textABC_size3   , sizeof (textABC_size3   ))) <<  arrowEnd << endl;
  Serial << F("textABC_size5:    ArrayLen=") << sizeof (textABC_size5   ) << arrowStart_full << _BUFPART (textABC_size5   , sizeof (textABC_size5   )) << arrowEnd << arrowStart_part << _BUFPART (textABC_size5   , strnlen (textABC_size5   , sizeof (textABC_size5   ))) <<  arrowEnd << endl;
  Serial << F("textAZ_size32:    ArrayLen=") << sizeof (textAZ_size32   ) << arrowStart_full << _BUFPART (textAZ_size32   , sizeof (textAZ_size32   )) << arrowEnd << arrowStart_part << _BUFPART (textAZ_size32   , strnlen (textAZ_size32   , sizeof (textAZ_size32   ))) <<  arrowEnd << endl;
  Serial << F("textAZ16_size32:  ArrayLen=") << sizeof (textAZ16_size32 ) << arrowStart_full << _BUFPART (textAZ16_size32 , sizeof (textAZ16_size32 )) << arrowEnd << arrowStart_part << _BUFPART (textAZ16_size32 , strnlen (textAZ16_size32 , sizeof (textAZ16_size32 ))) <<  arrowEnd << endl;

  Serial << F("textAZ_size32,   offset = 5 : ") << arrowStart_full << _BUFPART (textAZ_size32   + 5, sizeof (textAZ_size32  ) - 5) << arrowEnd << arrowStart_part << _BUFPART (textAZ_size32   + 5, strnlen (textAZ_size32,   sizeof (textAZ_size32  )) - 5) <<  arrowEnd << endl;
  Serial << F("textAZ16_size32, offset = 5 : ") << arrowStart_full << _BUFPART (textAZ16_size32 + 5, sizeof (textAZ16_size32) - 5) << arrowEnd << arrowStart_part << _BUFPART (textAZ16_size32 + 5, strnlen (textAZ16_size32, sizeof (textAZ16_size32)) - 5) <<  arrowEnd << endl;
  Serial << endl;

  delay(LOOP_DELAY);
}
