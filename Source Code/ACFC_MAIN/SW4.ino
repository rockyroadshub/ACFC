void SW4_func() {
  timerSLP_restart();
  switch (modePROG) {
    case PROG_TIMER_FDATE_DISP: PROG_FDATE(decLoop(iBR, START, iFC), decLoop(iBR, START, iFC)); break;
    case PROG_SYSTEM_DATE_DISP: INCREMENT_SYSD(decLoop(month(),1,12), decLoop(year(),2000,2099), -1); break;
    case PROG_SYSTEM_STNG_DISP: INCREMENT_STNG(decLoop(BCK,LOW,HIGH), decLoop(SLP, DEFAULT_SLEEP_MIN, DEFAULT_SLEEP_MAX)); break;
  }
}
