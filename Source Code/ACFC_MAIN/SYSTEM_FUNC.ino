void systemFunc() {
  timer_FNC();
  relaySTATE();
  lockDispACFC();
}

void bckSTATE() {
  if (EEPROM.read(ADDRESS_STBCK)) { digitalWrite(PIN_BCKL, HIGH); }
}

void loadSTATE_BCK() {
  if (!digitalRead(PIN_BCKL)) { bckSTATE(); }
}

void relaySTATE() {
  if (digitalRead(PIN_RLAY)) {
    if (!isRLY) {
      isRLY = true;
      iFC = incLoop(iFC,START,LMT-1);
      iBR = iFC;
      OC[iFC] = now();
      time_FC.start();
      if (modePROG == PROG_TIMER_FDATE_DISP) { PROG_FDATE(iFC,iFC); }
      else if (modePROG == PROG_TOTAL_COUNT_DISP) { digitDisp(iFC,14,0,2); }
    }
    FC[iFC] = time_FC.elapsed();
  }
  else if (!digitalRead(PIN_RLAY) && isRLY) {
    isRLY = false;
    FC[iFC] = time_FC.elapsed();
    time_FC.stop();
    time_FC.reset();
  }
  
}


void lockDispACFC () { 
  if (modePROG == PROG_SYSTEM_INTR_LOCK && (
      SW1.pressed() ||
      SW2.pressed() ||
      SW3.pressed() ||
      SW4.pressed())) {
      timerSLP_restart(); lockDisp(); }
}

void lockDisp() {
  LCD.setCursor(0,0);
  LCD.print("ACFC "); LCD.print(ACFC_VERSION);
  LCD.setCursor(0,1);
  LCD.print("LOCKED   ");
  LCD.write(byte(LOCK_LOGO));
  loadSTATE_BCK();
}

void ulckDisp() {
  LCD.setCursor(0,0);
  LCD.print("ACFC "); LCD.print(ACFC_VERSION);
  LCD.setCursor(0,1);
  LCD.print("UNLOCKED ");
  LCD.write(byte(ULCK_LOGO));
  loadSTATE_BCK();
}

void getMSG(uint8_t j) {
  strcpy_P(msgBUFFER, (char*)pgm_read_word(&(MSG_TABLE[j])));
  Serial.print(msgBUFFER);
}
