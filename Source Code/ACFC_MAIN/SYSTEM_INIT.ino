void systemInit() {
  init_SRL();
  init_CLK();
  init_LCD();
  init_KEY();
  init_EEP();
  init_DEF();
  init_STN();
  init_WTX();
  init_FNC();
}

void init_SRL() {
  Serial.begin(BAUD_RATE);
}

void init_CLK() {
  setTime(getHour(), getMinute(), getSecond(), getDay(), getMonth(), getYear());
}

void init_LCD() {
  LCD.begin(16,2);
  LCD.clear();
  LCD.createChar(LOCK_LOGO, lock);
  LCD.createChar(ULCK_LOGO, ulck);
}

void init_KEY() {
  pinMode(PIN_BCKL, OUTPUT);
  pinMode(PIN_RLAY, OUTPUT);
}

void init_EEP() {
  if (EEPROM.read(ADDRESS_EPDEF) != DEFAULT_EPDEF) {
    LCD.clear();
    LCD.print("Clears add: ");
    getMSG(22); Serial.println(ADDRESS_EPDEF);
    EEPROM.write(ADDRESS_EPDEF, DEFAULT_EPDEF);
    digitDisp(ADDRESS_EPDEF, 12,0,4);
    for (int j = 1; j < 1024; j++) { EEPROM.write(j, RESET); digitDisp(j, 12,0,4); getMSG(22); Serial.println(j); }
    LCD.clear();
    getMSG(23); Serial.println("");
    LCD.print("Clearing done.");
  }
}

void init_DEF() {
  if (!EEPROM.read(ADDRESS_ISDEF)) {
    init_DEF_func();
  }
}

void init_DEF_func() {
  LCD.clear();
  getMSG(24); Serial.println("");
  LCD.print("Setting defaults...");
  EEPROM.write(ADDRESS_ISDEF, true);
  EEPROM.write(ADDRESS_STBCK, DEFAULT_BCKLG);
  EEPROM.write(ADDRESS_STDIS, DEFAULT_DISPL);
  EEPROM.write(ADDRESS_STSLP, DEFAULT_SLEEP_MAX);
  EEPROM.write(ADDRESS_STEDT, DEFAULT_EDITS);
  EEPROM.write(ADDRESS_ISDBG, DEFAULT_DEBUG);
  LCD.clear();
  getMSG(21); Serial.println("");
  LCD.print("Default set.");
}

void init_STN() {
  LCD.clear();
  getMSG(25); Serial.println("");
  LCD.print("Loading settings");
  BCK   = EEPROM.read(ADDRESS_STBCK);
  DIS   = EEPROM.read(ADDRESS_STDIS);
  SLP   = EEPROM.read(ADDRESS_STSLP);
  EDT   = EEPROM.read(ADDRESS_STEDT);
  isDBG = EEPROM.read(ADDRESS_ISDBG);
  LCD.clear();
  getMSG(26); Serial.println("");
  LCD.print("Settings loaded.");
}

void init_WTX() {
  LCD.clear();
  getMSG(27); Serial.println("");
  LCD.setCursor(0,0); LCD.print("Device Ready.");
  LCD.setCursor(0,1); LCD.print("Version: "); LCD.print(ACFC_VERSION);
}

void init_FNC() {
  time_DS.start();
  timerGLOBAL_ready();
  timerSLP_restart();
  bckSTATE();
}


