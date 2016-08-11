void serialEvent() {
  uint8_t j = 0;
  timerSLP_restart();
  while(Serial.available() && j != BFF) { cmdBUFFER[j] = (char)Serial.read(); j++; }
    
  if (isST_SLP) { ST_SLP_func(); }
  if (isST_EDT) { ST_EDT_func(); }
  
  if (IS_DBG) { IS_DBG_func(); }
  else if (CK_INF) { CK_INF_func(); }
  else if (ST_DEF) { ST_DEF_func(); }
  else if (IS_RST && isDBG) { IS_RST_func(); }
  else if (IS_MEM && isDBG) { IS_MEM_func(); }
  else if (ST_BCK && isDBG && (ST_BL0 || ST_BL1)) { ST_BCK_func();  }
  else if (ST_SLP && isDBG) { isST_SLP = true; }
  else if (ST_EDT && isDBG) { isST_EDT = true; }
  for (uint8_t i = 0; i < BFF; i++) { cmdBUFFER[i] = NULL; }
}

void IS_DBG_func() {
  if(!EEPROM.read(ADDRESS_ISDBG)) { 
    EEPROM.write(ADDRESS_ISDBG, true); 
    isDBG = true; 
    getMSG(14);
    getMSG(15);
    Serial.println(""); }
  else { 
    EEPROM.write(ADDRESS_ISDBG, false); 
    isDBG = false; 
    getMSG(14); 
    getMSG(16);
    Serial.println(""); 
  }
}

void CK_INF_func() {
  getMSG(0); Serial.println(ACFC_VERSION); 
  getMSG(1); Serial.println(__DATE__);
  getMSG(11); Serial.println(freeMemory());
  Serial.println(""); 
  getMSG(2); Serial.println("");
  getMSG(3); Serial.println(PERIOD);
  getMSG(5); Serial.println(SLP);
  getMSG(6); Serial.println(EDT);
  getMSG(7); Serial.println(DIS); 
  Serial.println(""); 
  getMSG(8); Serial.println("");
  getMSG(4); Serial.println(BCK); 
  getMSG(9); Serial.println(isDBG); 
  getMSG(10); Serial.println(EEPROM.read(ADDRESS_ISDEF)); 
}

void ST_DEF_func() {
  init_DEF_func();
  init_STN();
  digitalWrite(PIN_BCKL, BCK);
  getMSG(21); Serial.println("");
}

void IS_RST_func() { EEPROM.write(ADDRESS_EPDEF, RESET); getMSG(12); Serial.println(""); }

void IS_MEM_func() { getMSG(11); Serial.println(freeMemory()); }

void ST_BCK_func() {
  EEPROM.write(ADDRESS_STBCK, (cmdBUFFER[3] - '0')); BCK = cmdBUFFER[3] - '0'; 
  if (SLPTimer < SEC*SLP) { digitalWrite(PIN_BCKL, BCK); }
  if (modePROG == PROG_SYSTEM_STNG_DISP) { dispBCKState(); }
  getMSG(13);
  if(ST_BL0) { getMSG(18); Serial.println(""); }
  else { getMSG(17); Serial.println(""); }
}

void ST_SLP_func() {
  SLP = ((cmdBUFFER[1] - '0')*100) + ((cmdBUFFER[2] - '0')*10) + (cmdBUFFER[3] - '0');
  EEPROM.write(ADDRESS_STSLP, SLP);
  if (modePROG == PROG_SYSTEM_STNG_DISP) { digitDisp(SLP,12,1,3); }
  isST_SLP = false;
  getMSG(19); Serial.println(SLP);
}

void ST_EDT_func() {
  EDT = ((cmdBUFFER[1] - '0')*100) + ((cmdBUFFER[2] - '0')*10) + (cmdBUFFER[3] - '0');
  EEPROM.write(ADDRESS_STEDT, EDT);
  isST_EDT = false;
  getMSG(20); Serial.println(EDT);
}


