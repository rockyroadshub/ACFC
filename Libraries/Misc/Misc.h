#ifndef Misc_h
#define Misc_h

#define MISC_LIB_VERSION "1.0.0"

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <inttypes.h>
#include <String.h>

enum switch_type { TOGGLE, RUN_ONCE };
enum switch_toggle { ON = 1, OFF = 2 };

uint8_t pcMonth();
uint8_t pcDay();
uint16_t pcYear();
uint8_t pcHour();
uint8_t pcMinute();
uint8_t pcSecond();
uint16_t incLoop (uint16_t var, uint16_t start, uint16_t max);
uint16_t incLoop (uint16_t var, uint16_t start, uint16_t inc, uint16_t max);
uint16_t decLoop (uint16_t var, uint16_t start, uint16_t max);
uint16_t decLoop (uint16_t var, uint16_t start, uint16_t dec, uint16_t max);
uint16_t convHour (uint32_t t);
uint8_t convMinute (uint32_t t);
uint8_t convSecond (uint32_t t);

class Button {
	private:
		switch_type _st;
		boolean switch_debounce;
		uint8_t switchToggle;
		uint8_t _sw;
		uint8_t _sw1;
		uint8_t _sw2;
		uint8_t _sw3;
		uint8_t _sw4;
	public:
		Button(uint8_t sw, switch_type st);
		Button(uint8_t sw1, uint8_t sw2, switch_type st);
		Button(uint8_t sw1, uint8_t sw2, uint8_t sw3, switch_type st);
		Button(uint8_t sw1, uint8_t sw2, uint8_t sw3, uint8_t sw4, switch_type st);
		boolean pressed();
		boolean pressed2();
		uint8_t toggled();
};

class A2DButton {
	private:
		switch_type _st;
		bool switch_debounce;
		uint8_t switchToggle;
		uint8_t _sw;
		uint8_t _sw1;
		uint8_t _sw2;
		uint8_t _sw3;
		uint8_t _sw4;
	public:
		A2DButton(uint8_t sw, switch_type st);
		A2DButton(uint8_t sw1, uint8_t sw2, switch_type st);
		boolean pressed();
		boolean pressed2();
};

#endif