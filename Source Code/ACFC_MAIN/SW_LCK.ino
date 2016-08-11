void LCK_func() {
  timerSLP_restart(); 
  LCD.clear();  
  LCD.noBlink();
  if (modePROG != PROG_SYSTEM_INTR_LOCK) { 
    modePROG = PROG_SYSTEM_INTR_LOCK; 
    lockDisp(); 
  }
  else if (modePROG == PROG_SYSTEM_INTR_LOCK) { 
    modePROG = RESET; 
    ulckDisp(); 
  }
}
