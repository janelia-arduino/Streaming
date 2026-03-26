.PHONY: clean
clean:
	rm -rf .pio

.PHONY: native-test
native-test:
	g++ -std=c++17 -Wall -Wextra -pedantic -DARDUINO=10800 -Itest/native/include -Isrc test/native/test_streaming.cpp -o /tmp/streaming_native_test
	/tmp/streaming_native_test

.PHONY: teensy41-firmware
teensy41-firmware: clean
	pio run -e teensy41

.PHONY: teensy41-upload
teensy41-upload: clean
	pio run -e teensy41 --target upload --upload-port /dev/ttyACM0

.PHONY: mega-firmware
mega-firmware: clean
	pio run -e mega

.PHONY: mega-upload
mega-upload: clean
	pio run -e mega --target upload --upload-port /dev/ttyACM0

.PHONY: uno-firmware
uno-firmware: clean
	pio run -e uno

.PHONY: uno-upload
uno-upload: clean
	pio run -e uno --target upload --upload-port /dev/ttyACM0

.PHONY: monitor
monitor:
	pio device monitor --echo --eol=LF
