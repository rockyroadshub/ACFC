void SW2_func() {
  timerSLP_restart();
  switch (modePROG) {
    case PROG_SYSTEM_DATE_DISP: MODE_SYSD(); break;
    case PROG_SYSTEM_STNG_DISP: MODE_STNG(); break;
  }
}
