void setBrowser (uint8_t i) {
  iBR = i;
}

void PROG_FDATE (uint8_t i, uint8_t j) {
  setBrowser(i);
  if (iFC > 0) {
    PROG_FDATE_label1();
    PROG_FDATE_data(i,j);
  }
  else {
    PROG_FDATE_label2();
  }
}

void PROG_FDATE_label1() {
  LCD.setCursor(0,0);
  LCD.print("F   TD:   :  :");
  LCD.setCursor(0,1);
  LCD.print("D         :  :");
}

void PROG_FDATE_label2() {
  LCD.setCursor(0,0);
  LCD.print("No voltage");
  LCD.setCursor(0,1);
  LCD.print("fluctuations");
}

void PROG_FDATE_data_1 (time_t t, uint8_t fc, uint8_t c, uint8_t r) {
  digitDisp(fc,c,r,2);
  digitDisp(convHour(t),c+6,r,3);
  digitDisp(convMinute(t),c+10,r,2);
  digitDisp(convSecond(t),c+13,r,2);
}

void PROG_FDATE_data (uint8_t i, uint8_t j) {
  PROG_FDATE_data_1(FC[i],j,1,0);
  dispDate(OC[i],1,1);
}
