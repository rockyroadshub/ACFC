void timer_FNC() {
  timer_FNC_DIS();
  timer_FNC_SLP();
  timer_FNC_EDT1();
  timer_FNC_EDT2();
}

void timer_FNC_DIS() {
  if (time_DS.elapsed() == DEFAULT_DISPL) {
    timerDIS_restart();
    dispData();
  }
}

void timer_FNC_SLP() {
  if (SLPTimer < SEC*SLP) { SLPTimer++; }
  else if (SLPTimer == SEC*SLP) { SLPTimer++;
    if (modePROG != PROG_SYSTEM_INTR_LOCK) { modePROG = RESET; }
    modeSYSD = RESET; modeSTNG = RESET;
    sysdEDIT = RESET; stngEDIT = RESET;
    timerBLINK_ready();
    LCD.noBlink();
    LCD.clear();
    if (digitalRead(PIN_BCKL)) { digitalWrite(PIN_BCKL, LOW); }
  }
}

void timer_FNC_EDT1() {
  if (EDTTimer1 < SEC*EDT) { EDTTimer1++; }
  else if (EDTTimer1 == SEC*EDT) { EDTTimer1++;
    modeSYSD = RESET;
    sysdEDIT = RESET;
    LCD.noBlink();
  }
}

void timer_FNC_EDT2() {
  if (EDTTimer2 < SEC*EDT) { EDTTimer2++; }
  else if (EDTTimer2 == SEC*EDT) { EDTTimer2++;
    modeSTNG = RESET;
    stngEDIT = RESET;
    LCD.noBlink();
  }
}

void timerGLOBAL_ready() {
  timerSLP_ready();
  timerEDT1_ready();
  timerEDT2_ready();
}

void timerBLINK_ready() {
  timerEDT1_ready();
  timerEDT2_ready();
}

/* Edit */
//[1]
void timerEDT1_stop() {
  EDTTimer1 = SEC*EDT;
}

void timerEDT1_ready() {
  EDTTimer1 = 1+(SEC*EDT);
}

void timerEDT1_restart() {
  EDTTimer1 = RESET;
}

//[2]
void timerEDT2_stop() {
  EDTTimer2 = SEC*EDT;
}

void timerEDT2_ready() {
  EDTTimer2 = 1+(SEC*EDT);
}

void timerEDT2_restart() {
  EDTTimer2 = RESET;
}

/* Sleep */
void timerSLP_stop() {
  SLPTimer = SEC*SLP;
}

void timerSLP_ready() {
  SLPTimer = 1+(SEC*SLP);
}

void timerSLP_restart() {
  SLPTimer = RESET;
}

/* Display */
void timerDIS_ready() {
  time_DS.stop();
  time_DS.reset();
}

void timerDIS_restart() {
  timerDIS_ready();
  time_DS.start();
}
