void RST_func() {
  LCD.clear();
  for(int i = 0; i < LMT; i++) {
    FC[i] = RESET;
    OC[i] = RESET;
    LCD.setCursor(0,0);
    LCD.print("Clearing data ");
    LCD.print(i);
  }
  iFC = RESET;
  LCD.clear();
  LCD.print("All data cleared");
}
