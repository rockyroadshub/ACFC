void PROG_STNG() {
  PROG_STNG_label();
  PROG_STNG_data();
}

void PROG_STNG_label() {
  LCD.setCursor(0,0); LCD.print("Settings");
  LCD.setCursor(0,1); LCD.print("BL: ");
  LCD.setCursor(8,1); LCD.print("SL: ");
}

void PROG_STNG_data() {
  digitDisp(SLP,12,1,3);
  dispBCKState();
}

void MODE_STNG() {
  if (!modeSTNG) {
    modeSTNG = true;
    stngEDIT = incLoop(stngEDIT, SETTING_CONFIG_BCK, SETTING_CONFIG_EDT);
    LCD.blink();
    LCD.setCursor(6,1);
    timerEDT2_restart();
  }
  else if (modeSTNG) {
    timerEDT2_restart();
    stngEDIT = incLoop(stngEDIT, SETTING_CONFIG_BCK, SETTING_CONFIG_EDT);
    if (stngEDIT == SETTING_CONFIG_SLP) { LCD.setCursor(14,1); }
    else if (stngEDIT == SETTING_CONFIG_EDT) { timerEDT2_stop(); }
  }
}

void INCREMENT_STNG (uint8_t bck, uint8_t slp) {
  timerEDT2_restart();
  if (stngEDIT == SETTING_CONFIG_BCK) { BCK = bck; EEPROM.write(ADDRESS_STBCK, BCK); dispBCKState(); LCD.setCursor(6,1);
    digitalWrite(PIN_BCKL, BCK); }
  else if (stngEDIT == SETTING_CONFIG_SLP) { SLP = slp; EEPROM.write(ADDRESS_STSLP, SLP); digitDisp(SLP,12,1,3); LCD.setCursor(14,1); }
}
