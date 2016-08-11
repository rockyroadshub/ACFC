void PROG_COUNT() {
  PROG_COUNT_label();
  PROG_COUNT_data();
}

void PROG_COUNT_label() {
  LCD.setCursor(0,0);
  LCD.print("Fluctuations: ");
}

void PROG_COUNT_data() {
  digitDisp(iFC,14,0,2);
}
