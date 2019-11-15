#include <Streaming.h>


const long BAUD = 115200;

const int lettera = 'A';
const int month = 4, day = 17, year = 2009;
const int hour = 8, minute = 20, second = 3;

const long LOOP_DELAY = 1000;

void setup()
{
  Serial.begin(BAUD);
}

void loop()
{
  Serial << "This is an example of the new streaming" << endl;
  Serial << "library.  This allows you to print variables" << endl;
  Serial << "and strings without having to type line after" << endl;
  Serial << "line of Serial.print() calls.  Examples: " << endl;

  Serial << "A is " << lettera << "." << endl;
  Serial << "The current date is " << day << "-" << month << "-" << year << "." << endl;

  Serial << "You can use modifiers too, for example:" << endl;
  Serial << _BYTE(lettera) << " is " << _HEX(lettera) << " in hex. " << endl;
  Serial << endl;

  Serial << "In library v6, you also get" << endl;
  Serial << "Padding [" << _PAD(10, '*') << "]" << endl;
  Serial << "Width specification [" << _WIDTH(10, 5) << "]" << endl;
  Serial << "Leading zeros [" << _WIDTHZ(month,2) << "]" << endl;
  Serial << _FMT("Format strings for stuff like dates and times %/%/% %:%:%", 
    _WIDTHZ(day, 2), _WIDTHZ(month, 2), year,
    _WIDTHZ(hour,2), _WIDTHZ(minute, 2), _WIDTHZ(second,2)) << endl;
  Serial << _FMT(F("To reduce your % size, these % can be in %"), F("sketch"), F("constants"), F("PROGMEM")) << endl;
  Serial << endl;

  delay(LOOP_DELAY);
}
