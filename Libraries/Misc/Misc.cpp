#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif

#include "Misc.h"
#include "inttypes.h"
#include "String.h"

uint16_t incLoop (uint16_t var, uint16_t start, uint16_t max) {
	if (var < max) {
		var++;
		return var;
	}
    else if (var == max) {
       	var = start;
       	return var;
   	}
    return 0;
}

uint16_t incLoop (uint16_t var, uint16_t start, uint16_t inc, uint16_t max) {
	if (var < max) {
		var += inc;
		return var;
	}
    else if (var == max) {
       	var = start;
       	return var;
   	}
    return 0;
}

uint16_t decLoop (uint16_t var, uint16_t start, uint16_t max) {
	if (var == start || var == 0) {
    	var = max;
    	return var;
    }
    else if (var <= max) {
        var--;
        return var;
    }
	return 0;
}

uint16_t decLoop (uint16_t var, uint16_t start, uint16_t dec, uint16_t max) {
	if (var == start || var == 0) {
    	var = max;
    	return var;
    }
    else if (var <= max) {
        var -= dec;
        return var;
    }
	return 0;
}

uint16_t convHour(uint32_t t) {
	uint16_t j = t/3600;
	return j;
}

uint8_t convMinute(uint32_t t) {
	uint8_t j = (t/60)%60;
	return j;
}

uint8_t convSecond(uint32_t t) {
	uint8_t j = t%60;
	return j;
}

Button::Button(uint8_t sw, switch_type st) {
	pinMode(sw,INPUT);
	switch_debounce = false;
	_st = st;
	_sw = sw;
}

Button::Button(uint8_t sw1, uint8_t sw2, switch_type st) {
	pinMode(sw1,INPUT);
	pinMode(sw2,INPUT);
	switch_debounce = false;
	_st = st;
	_sw1 = sw1;
	_sw2 = sw2;
}

A2DButton::A2DButton(uint8_t sw, switch_type st) {
	pinMode(sw,INPUT);
	switch_debounce = false;
	_st = st;
	_sw = sw;
}

A2DButton::A2DButton(uint8_t sw1, uint8_t sw2, switch_type st) {
	pinMode(sw1,INPUT);
	pinMode(sw2,INPUT);
	switch_debounce = false;
	_st = st;
	_sw1 = sw1;
	_sw2 = sw2;
}

boolean A2DButton::pressed() {
	if (_st == RUN_ONCE) {
		if (analogRead(_sw) < 614 && switch_debounce) {
			switch_debounce = false;
			return false;
		}
		else if (analogRead(_sw) >= 614 && !switch_debounce) {
			switch_debounce = true;
			return true;
		}
		return false;
	}
}

boolean A2DButton::pressed2() {
	if (_st == RUN_ONCE) {
		if (analogRead(_sw1) < 614 && analogRead(_sw2) < 614 && switch_debounce) {
			switch_debounce = false;
			return false;
		}
		else if (analogRead(_sw1) >= 614 && analogRead(_sw2) >= 614 && !switch_debounce) {
			switch_debounce = true;
			return true;
		}
		return false;
	}
}


boolean Button::pressed() {
	if (_st == RUN_ONCE) {
		if (!digitalRead(_sw) && switch_debounce) {
			switch_debounce = false;
			return false;
		}
		else if (digitalRead(_sw) && !switch_debounce) {
			switch_debounce = true;
			return true;
		}
		return false;
	}
}

boolean Button::pressed2() {
	if (_st == RUN_ONCE) {
		if (!digitalRead(_sw1) && !digitalRead(_sw2) && switch_debounce) {
			switch_debounce = false;
			return false;
		}
		else if (digitalRead(_sw1) && digitalRead(_sw2) && !switch_debounce) {
			switch_debounce = true;
			return true;
		}
		return false;
	}
	else if (_st == TOGGLE) {
		if (!digitalRead(_sw1) && !digitalRead(_sw2) && switch_debounce) {
			switch_debounce = false;
			return false;
		}
		else if (digitalRead(_sw1) && digitalRead(_sw2) && !switch_debounce) {
			switch_debounce = true;
			switchToggle = incLoop(switchToggle,ON,OFF);
			return true;
		}
		return false;
	}
	
}

uint8_t Button::toggled() {
	if (_st == TOGGLE) {
		return switchToggle;
	}
	return 0;
}

uint8_t pcMonth() {
	String pc_month = "";
	String month_list[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	uint8_t m = 0;
	for (int i = 0; i < 3; i++) {
    	pc_month += __DATE__[i];
  	}
	for (int j = 0; j < 12; j++) {
		if(month_list[j] == pc_month) {
			m = j+1;
			return m;
		}
	}
  return m;
}
uint8_t pcDay() {
	uint8_t d = 0;
	for (int i = 4; i < 6; i++) {
    	int j = round(pow(10,5-i));
    	int k = __DATE__[i]-'0';
    	d += j*k;
  	}
  	return d;
}
uint16_t pcYear() {
	int y = 0;
	for (int k = 7; k < 11; k++) {
    	int i = round(pow(10,10-k));
    	int j = __DATE__[k]-'0';
    	y += i*j;
  	}
  	return y;
}
uint8_t pcHour() {
	uint8_t hr = (__TIME__[0]-'0')*10 + (__TIME__[1]-'0');
	return hr;
}
uint8_t pcMinute() {
	uint8_t mn = (__TIME__[3]-'0')*10 + (__TIME__[4]-'0');;
	return mn;
}
uint8_t pcSecond() {
	uint8_t sc = (__TIME__[6]-'0')*10 + (__TIME__[7]-'0');;
	return sc;
}