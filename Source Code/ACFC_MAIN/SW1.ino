void SW1_func() {
  LCD.clear(); LCD.noBlink();
  modeSYSD = RESET; modeSTNG = RESET;
  sysdEDIT = RESET; stngEDIT = RESET; 
  loadSTATE_BCK(); timerBLINK_ready(); timerSLP_restart();
  modePROG = incLoop(modePROG, PROG_TIMER_FDATE_DISP, PROG_SYSTEM_STNG_DISP);
  switch (modePROG) {
    case PROG_TIMER_FDATE_DISP: PROG_FDATE(iFC, iFC); break;
    case PROG_TOTAL_COUNT_DISP: PROG_COUNT(); break;
    case PROG_SYSTEM_DATE_DISP: PROG_DATE(); break;
    case PROG_SYSTEM_STNG_DISP: PROG_STNG(); break;
  }
}
