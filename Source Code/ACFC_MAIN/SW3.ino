void SW3_func() {
  timerSLP_restart();
  switch (modePROG) {
    case PROG_TIMER_FDATE_DISP: PROG_FDATE(incLoop(iBR, START, iFC), incLoop(iBR, START, iFC)); break;
    case PROG_SYSTEM_DATE_DISP: INCREMENT_SYSD(incLoop(month(),1,12), incLoop(year(),2000,2099), 1); break;
    case PROG_SYSTEM_STNG_DISP: INCREMENT_STNG(incLoop(BCK,LOW,HIGH), incLoop(SLP, DEFAULT_SLEEP_MIN, DEFAULT_SLEEP_MAX)); break;
  }
}
