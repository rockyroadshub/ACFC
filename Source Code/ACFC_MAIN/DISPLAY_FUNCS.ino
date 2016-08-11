void digitDisp (uint16_t i, uint8_t c, uint8_t r, uint8_t d) {
  boolean isZero=false;
  uint8_t count = 0;
  uint16_t num = i;
  while (!isZero) {
    i /= 10;
    count++;
    if (i==0) { isZero=true; }
  }
  uint8_t total = d-count;
  LCD.setCursor(c,r);
  for (uint8_t j=0; j < total; j++) { LCD.print("0"); }
  LCD.print(num);
}

void dispDate (time_t t, uint8_t c, uint8_t r) {
  uint8_t _year = 0;
  if (year(t) >= 2000) { _year = year(t) - 2000; }
  digitDisp(month(t),c,r,2);
  digitDisp(day(t),c+2,r,2);
  digitDisp(_year,c+4,r,2);
  digitDisp(hour(t),c+7,r,2);
  digitDisp(minute(t),c+10,r,2);
  digitDisp(second(t),c+13,r,2);
}

void dispBCKState () {
  if (BCK) { LCD.setCursor(4,1); LCD.print("ON "); }
  else { LCD.setCursor(4,1); LCD.print("OFF"); }
}

void dispData () {
  switch (modePROG) {
    case PROG_TIMER_FDATE_DISP: if (iFC > 0 && iBR == iFC) { PROG_FDATE_data(iFC,iFC); } break;
    case PROG_SYSTEM_DATE_DISP: if (!modeSYSD) { PROG_DATE_data(); } break;
  }
}
