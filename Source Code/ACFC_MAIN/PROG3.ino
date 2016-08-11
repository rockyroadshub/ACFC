void PROG_DATE() {
  PROG_DATE_label();
  PROG_DATE_data();
}

void PROG_DATE_label() {
  LCD.setCursor(0,0);
  LCD.print("MON:");
  LCD.setCursor(0,1);
  LCD.print("DAY:      :  :");
  LCD.setCursor(8,0);
  LCD.print("YR:");
}

void PROG_DATE_data() {
  digitDisp(month(),5,0,2);
  digitDisp(day(),5,1,2);
  digitDisp(year(),12,0,4);
  digitDisp(hour(),8,1,2);
  digitDisp(minute(),11,1,2);
  digitDisp(second(),14,1,2);
}


void MODE_SYSD() {
  if (!modeSYSD) {
    modeSYSD = true;
    sysdEDIT = incLoop(sysdEDIT, SYSTEM_DATE_MT, SYSTEM_DATE_ED);
    LCD.blink();
    LCD.setCursor(6,0);
    timerEDT1_restart();
  }
  else if (modeSYSD) {
    timerEDT1_restart();
    sysdEDIT = incLoop(sysdEDIT, SYSTEM_DATE_MT, SYSTEM_DATE_ED);
    if (sysdEDIT == SYSTEM_DATE_DY) { LCD.setCursor(6,1); }
    else if (sysdEDIT == SYSTEM_DATE_YR) { LCD.setCursor(15,0); }
    else if (sysdEDIT == SYSTEM_DATE_HR) { LCD.setCursor(9,1);  }
    else if (sysdEDIT == SYSTEM_DATE_MN) { LCD.setCursor(12,1); }
    else if (sysdEDIT == SYSTEM_DATE_SC) { LCD.setCursor(15,1); }
    else if (sysdEDIT == SYSTEM_DATE_ED) { timerEDT1_stop(); }
  }
}

void INCREMENT_SYSD (uint8_t mn, uint16_t yr, int i) {
  timerEDT1_restart();
  if (sysdEDIT == SYSTEM_DATE_MT) { setTime(hour(),minute(),second(),day(),mn,year());  PROG_DATE_data(); LCD.setCursor(6,0); }
  else if (sysdEDIT == SYSTEM_DATE_YR) { setTime(hour(),minute(),second(),day(),month(),yr); PROG_DATE_data(); LCD.setCursor(15,0); }
  else if (sysdEDIT == SYSTEM_DATE_DY) { adjustTime(i*86400); PROG_DATE_data(); LCD.setCursor(6,1); }
  else if (sysdEDIT == SYSTEM_DATE_HR) { adjustTime(i*3600); PROG_DATE_data(); LCD.setCursor(9,1); }
  else if (sysdEDIT == SYSTEM_DATE_MN) { adjustTime(i*60); PROG_DATE_data(); LCD.setCursor(12,1); }
  else if (sysdEDIT == SYSTEM_DATE_SC) { adjustTime(i*1); PROG_DATE_data(); LCD.setCursor(15,1); }
}

uint8_t getHour () {
  return ((__TIME__[0] - '0')*10) + (__TIME__[1] - '0');
}

uint8_t getMinute () {
  return ((__TIME__[3] - '0')*10) + (__TIME__[4] - '0');
}

uint8_t getSecond () {
  return ((__TIME__[6] - '0')*10) + (__TIME__[7] - '0');
}

uint8_t getDay() {
  if (__DATE__[4] < '0') { return __DATE__[5] - '0'; }
  else { return (( __DATE__[4] - '0')*10) + (__DATE__[5] - '0'); }
  return 0;
}

uint8_t getMonth() {
  if (IS_MONTH_JAN) { return 1; }
  else if (IS_MONTH_FEB) { return 2; }
  else if (IS_MONTH_MAR) { return 3; }
  else if (IS_MONTH_APR) { return 4; }
  else if (IS_MONTH_MAY) { return 5; }
  else if (IS_MONTH_JUN) { return 6; }
  else if (IS_MONTH_JUL) { return 7; }
  else if (IS_MONTH_AUG) { return 8; }
  else if (IS_MONTH_SEP) { return 9; }
  else if (IS_MONTH_OCT) { return 10; }
  else if (IS_MONTH_NOV) { return 11; }
  else if (IS_MONTH_DEC) { return 12; }
  return 1;
}

uint16_t getYear() {
  return ((__DATE__[7] - '0')*1000) + ((__DATE__[8] - '0')*100) + ((__DATE__[9] - '0')*10) + (__DATE__[10] - '0');
}

