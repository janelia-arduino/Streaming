#include <Streaming.h>

const long BAUD = 115200;
const long LOOP_DELAY = 1000;

const int year = 2026;
const int month = 3;
const int day = 26;
const int hour = 14;
const int minute = 5;
const int second = 9;
const float temperature_c = 23.875;
const float battery_v = 3.706;

void setup() {
  Serial.begin(BAUD);
}

void loop() {
  Serial << _FMT("Date %-%-% Time %:%:%",
                 year,
                 _WIDTHZ(month, 2),
                 _WIDTHZ(day, 2),
                 _WIDTHZ(hour, 2),
                 _WIDTHZ(minute, 2),
                 _WIDTHZ(second, 2))
         << endl;

  Serial << _FMT("Temperature [% C]", _FLOATW(temperature_c, 2, 6)) << endl;
  Serial << _FMT("Battery [% V]", _FLOATW(battery_v, 3, 6)) << endl;
  Serial << _FMT("Hex dump [%] [%] [%]",
                 _WIDTHZ(_HEX(0x0A), 2),
                 _WIDTHZ(_HEX(0x7F), 2),
                 _WIDTHZ(_HEX(0xC3), 2))
         << endl;
  Serial << endl;

  delay(LOOP_DELAY);
}
