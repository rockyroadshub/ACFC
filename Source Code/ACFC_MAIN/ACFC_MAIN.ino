/*
=====================================================================================================================================
          PROJECT :   AC Fluctuation TImer and Counter
          VERSION :   1.0.0
          FILE    :   ACFC_MAIN.ino
          AUTHOR  :   Arnell Christoper D. Dalid
          COMPANY :   PLDT Makati - South Core Net--Field Force 3 (PEMS) [PHILS]
          PURPOSE :   [Internship Project] - Counts and save the time duration of a fluctuation occured
          HISTORY :   [Created - 1.0.0]    - November 25, 2014
                      [Finalized]          - March  3, 2015 (Source Code)
                      [Edited]             - March 24, 2015 Added RST_func() for Resetting data
=====================================================================================================================================
*/

/* Libraries */
// ==================================================================================================================================
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>
#include <MemoryFree.h>
#include <StopWatch.h>
#include <inttypes.h>
#include <EEPROM.h>
#include <Time.h>
#include <Misc.h>
// ==================================================================================================================================

/* [A] Liquid Crystal (16x2) PINS  */
// ==================================================================================================================================
/*
 ------------------------------------------------------------------------
 | gizDuino ATmega328P PINS |  LCD 16x2 PINS (left to right)            |
 |          GND             |          01 - VSS                         |
 |          +5V             |          02 - VDD                         |
 |    w/3k Ohms Resistor    |          03 - VO  (Contrast)              |
 |          D07             |          04 - RS  (Select signal)         |
 |          GND             |          05 - R/W (Read/Write)            |
 |          D06             |          06 - E   (Enable)                |
 |          D05             |          11 - DB4 (Data bus [4-bit Mode]) |
 |          D04             |          12 - DB5      "       "          |
 |          D03             |          13 - DB6      "       "          |
 |          D02             |          14 - DB7      "       "          |
 |          D12             |          15 - A   (Backlight Power [+5v]) |
 |          GND             |          16 - K                           |
 ------------------------------------------------------------------------ 
*/

LiquidCrystal LCD(7,6,5,4,3,2);             // Liquid Crystal ( RS, E, DB4, DB5, DB6, DB7) --> 4-bit Mode
// ==================================================================================================================================

/* [0] Main System Variables */
// ==================================================================================================================================
/* [a] */
#define ACFC_VERSION "1.0.0"                // Device's current version

/* [b] */
#define RESET 0                             // For Reseting variables to 0
#define START 1                             // For Reseting variables to 1
#define PERIOD 50                           // Global device Sensitivity
#define LOCK_LOGO 0                         // Variable for calling lock-logo
#define ULCK_LOGO 1                         //     "           "    unlock-logo
#define BAUD_RATE 9600                      // Baud Rate
const uint8_t LMT  = 31;                    // Maximum Fluctuation data that can be saved
const uint8_t SEC  = 1000/PERIOD;           // Total counts to achieve 1 second (for Sleep and Edit Timer)
byte lock[8]       = {0,14,17,17,31,31,31}; // Logo for locking device
byte ulck[8]       = {0,14,16,16,31,31,31}; // "  " for unlocking device

/* [c] */
const uint8_t BFF = 4;                      // Buffer limit
char cmdBUFFER[BFF];                        // Command buffer

/* [d] */
uint8_t iFC;                                // Total Fluctuations occurred
uint8_t iBR;                                // Index for browsing functions
time_t FC[LMT];                             // Fluctuation Time Duration Storage
time_t OC[LMT];                             // Fluctuation Time/Date Storage
uint16_t SLPTimer;                          // Timer for sleep
uint16_t EDTTimer1;                         // Timer for editing System Date
uint16_t EDTTimer2;                         //    "         "    System Setting
StopWatch time_FC(StopWatch::SECONDS);      // Stopwatch Timer for a fluctuation's time duration
StopWatch time_DS(StopWatch::SECONDS);      // Stopwatch Timer for displaying data
// ==================================================================================================================================

/* [1] Pins for Keys (Input/Output) */
// ==================================================================================================================================
#define PIN_PROG 8                          // Select Display
#define PIN_MODE 9                          // Select Mode
#define PIN_INCR 10                         // Increment
#define PIN_DECR 11                         // Decrement
#define PIN_RLAY 12                         // Relay Power     (OUTPUT PIN)
#define PIN_BCKL 13                         // Backlight Power (OUTPUT PIN) 
// ==================================================================================================================================

/* [2] Address for System Settings */
// ==================================================================================================================================
#define ADDRESS_EPDEF 0                     // EEPROM Reset    (65)
#define ADDRESS_ISDEF 122                   // Default Set     (0|1)
#define ADDRESS_STBCK 123                   // Backlight State (0|1)
#define ADDRESS_STDIS 124                   // Display TImer   (1)
#define ADDRESS_STSLP 125                   // Sleep Timer     (0|255)
#define ADDRESS_STEDT 126                   // Edit Timer      (0|255)
#define ADDRESS_ISDBG 127                   // Debug Mode      (0|1)
// ==================================================================================================================================

/* [3] Default Values for Settings  */
// ==================================================================================================================================
#define DEFAULT_BCKLG LOW                   // Backlight default state
#define DEFAULT_DISPL 1                     // Display Timer (1 second refresh rate)
#define DEFAULT_EPDEF 65                    // EEPROM Reset Lock
#define DEFAULT_EDITS 30                    // Edit Time Duration (in Seconds)
#define DEFAULT_SLEEP_MIN 60                // Sleep Timer - Min     "    "
#define DEFAULT_SLEEP_MAX 120               // Sleep Timer - Max     "    "
#define DEFAULT_DEBUG false                 // Debug state
// ==================================================================================================================================

/* [4] Variable Settings */
// ==================================================================================================================================
uint8_t BCK;                                // Variable for loading Backlight state (ON/OFF)
uint8_t DIS;                                //       "         "    Display
uint8_t SLP;                                //       "         "    Sleep Timer
uint8_t EDT;                                //       "         "    Edit Time Duration (System Date and Setting)
boolean isDBG;                              // Trigger for Debug Mode
boolean isRLY;                              // Relay Switch debounce
// ==================================================================================================================================

/* [5] Main Program Variables and Constants */
// ==================================================================================================================================
#define PROG_TIMER_FDATE_DISP 1             // Displays TIME,DATE & DURATION of a fluctuation occured 
#define PROG_TOTAL_COUNT_DISP 2             //   " "    COUNT of fluctuations
#define PROG_SYSTEM_DATE_DISP 3             //   " "    SYSTEM DATE
#define PROG_SYSTEM_STNG_DISP 4             //   " "    SYSTEM SETTINGS (LCD Contrast, Sleep Mode, Backlight)
#define PROG_SYSTEM_DEBUG_MOD 5             // Puts the device on debug mode
#define PROG_SYSTEM_INTR_LOCK 6             // Mode of program for LOCKING the device, disabling it from controlling unless, unlocked
uint8_t modePROG;                           // Mode counter for determining what program to be used and display
// ==================================================================================================================================

/* [6] System Date Variables and Constants */
// ==================================================================================================================================
#define SYSTEM_DATE_MT 1                    // Month Setting Mode
#define SYSTEM_DATE_DY 2                    // Day      " "
#define SYSTEM_DATE_YR 3                    // Year     " "
#define SYSTEM_DATE_HR 4                    // Hour     " "
#define SYSTEM_DATE_MN 5                    // Minute   " "
#define SYSTEM_DATE_SC 6                    // Second   " "
#define SYSTEM_DATE_ED 7                    // System Date Setting OFF
uint8_t modeSYSD;                           // Mode counter for the editing time & date in SYSTEM DATE
uint8_t sysdEDIT;                           // Mode counter for editing each items in SYSTEM DATE
// ==================================================================================================================================

/* [7] System Setting Variables and Constants */
// ==================================================================================================================================
#define SETTING_CONFIG_BCK 1                // Backlight Setting Mode
#define SETTING_CONFIG_SLP 2                // Sleep Setting Mode
#define SETTING_CONFIG_EDT 3                // Setting Mode OFF
uint8_t modeSTNG;                           // Mode counter for the editing time SETTINGS
uint8_t stngEDIT;                           // Mode counter for editing each items in SETTINGS
// ==================================================================================================================================

/* [8] System Keys Variables */
// ==================================================================================================================================
Button SW1 (PIN_PROG, RUN_ONCE);            // Select Display (Program)
Button SW2 (PIN_MODE, RUN_ONCE);            // Select Mode - for editing date and settings)
Button SW3 (PIN_INCR, RUN_ONCE);            // Increment       " "
Button SW4 (PIN_DECR, RUN_ONCE);            // Decrement       " "
Button LCK (PIN_PROG, PIN_MODE, RUN_ONCE);  // System Lock
Button CLR (PIN_PROG, PIN_INCR, RUN_ONCE);  // Clear/Reset Display
Button RST (PIN_PROG, PIN_DECR, RUN_ONCE);  // Resets data
// ==================================================================================================================================

/* [9] Month Variables */
// ==================================================================================================================================
#define IS_MONTH_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')  // January
#define IS_MONTH_FEB (__DATE__[0] == 'F' && __DATE__[1] == 'e' && __DATE__[2] == 'b')  // February
#define IS_MONTH_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')  // March
#define IS_MONTH_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p' && __DATE__[2] == 'r')  // April
#define IS_MONTH_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')  // May
#define IS_MONTH_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')  // June
#define IS_MONTH_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')  // July
#define IS_MONTH_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u' && __DATE__[2] == 'g')  // August
#define IS_MONTH_SEP (__DATE__[0] == 'S' && __DATE__[1] == 'e' && __DATE__[2] == 'p')  // September
#define IS_MONTH_OCT (__DATE__[0] == 'O' && __DATE__[1] == 'c' && __DATE__[2] == 't')  // October
#define IS_MONTH_NOV (__DATE__[0] == 'N' && __DATE__[1] == 'o' && __DATE__[2] == 'v')  // November
#define IS_MONTH_DEC (__DATE__[0] == 'D' && __DATE__[1] == 'e' && __DATE__[2] == 'c')  // December
// ==================================================================================================================================

/* [10] Serial Commands Variables */
// ==================================================================================================================================
#define IS_RST (cmdBUFFER[0] == '-' && cmdBUFFER[1] == 'R' && cmdBUFFER[2] == 'S' && cmdBUFFER[3] == 'T')  // -RST --> Reset EEPROM
#define IS_DBG (cmdBUFFER[0] == '-' && cmdBUFFER[1] == 'D' && cmdBUFFER[2] == 'B' && cmdBUFFER[3] == 'G')  // -DBG --> Set to Debug
#define IS_MEM (cmdBUFFER[0] == '-' && cmdBUFFER[1] == 'M' && cmdBUFFER[2] == 'E' && cmdBUFFER[3] == 'M')  // -MEM --> Check SRAM
#define ST_BCK (cmdBUFFER[0] == '-' && cmdBUFFER[1] == 'B' && cmdBUFFER[2] == 'L')                         // -BLX --> Set Backlight
#define ST_BL0 (cmdBUFFER[3] == '0')                                                                       //   0  --> Set BCK OFF
#define ST_BL1 (cmdBUFFER[3] == '1')                                                                       //   1  --> Set BCK ON
#define CK_INF (cmdBUFFER[0] == '-' && cmdBUFFER[1] == 'I' && cmdBUFFER[2] == 'N' && cmdBUFFER[3] == 'F')  // -INF --> Check Info
#define ST_DEF (cmdBUFFER[0] == '-' && cmdBUFFER[1] == 'D' && cmdBUFFER[2] == 'E' && cmdBUFFER[3] == 'F')  // -DEF --> Set to default
#define ST_SLP (cmdBUFFER[0] == '-' && cmdBUFFER[1] == 'S' && cmdBUFFER[2] == 'L' && cmdBUFFER[3] == 'P')  /* -SLP --> Sets Sleep 
                                                                                                               3-DIGIT Format
                                                                                                               e.g. -SLP 050 
                                                                                                               sets to 50 seconds
                                                                                                           */
#define ST_EDT (cmdBUFFER[0] == '-' && cmdBUFFER[1] == 'E' && cmdBUFFER[2] == 'D' && cmdBUFFER[3] == 'T') /* -EDT --> Sets Edit
                                                                                                             3-DIGIT Format
                                                                                                             e.g.  -EDT 050
                                                                                                             sets to 50 seconds
                                                                                                          */
boolean isST_SLP, isST_EDT;   // boolean for editing date/settings and sleep timer                                                                                                        
// ==================================================================================================================================

/* [11] Serial Message Variables */
// ==================================================================================================================================
const uint8_t mBFF = 35;
char msgBUFFER[mBFF];
const char M00[] PROGMEM = "Version         : ";                         // 0
const char M01[] PROGMEM = "Date Compiled   : ";                         // 1
const char M02[] PROGMEM = "Timer/Delays (sec)";                         // 2
const char M03[] PROGMEM = "Period (milli)  : ";                         // 3
const char M04[] PROGMEM = "Backlight State : ";                         // 4
const char M05[] PROGMEM = "Sleep           : ";                         // 5
const char M06[] PROGMEM = "Edit Settings   : ";                         // 6
const char M07[] PROGMEM = "Display Refresh : ";                         // 7
const char M08[] PROGMEM = "Booleans (0|1)    ";                         // 8
const char M09[] PROGMEM = "Debug State     : ";                         // 9
const char M10[] PROGMEM = "Is Default set  : ";                         // 10
const char M11[] PROGMEM = "Available SRAM  : ";                         // 11
const char M12[] PROGMEM = "EEPROM reset. Please reset device";          // 12
const char M13[] PROGMEM = "Backlight set to: ";                         // 13
const char M14[] PROGMEM = "Debug ";                                     // 14
const char M15[] PROGMEM = "enabled";                                    // 15
const char M16[] PROGMEM = "disabled";                                   // 16
const char M17[] PROGMEM = "ON";                                         // 17
const char M18[] PROGMEM = "OFF";                                        // 18
const char M19[] PROGMEM = "Sleep set to: ";                             // 19
const char M20[] PROGMEM = "Edit set to: ";                              // 20
const char M21[] PROGMEM = "Defaults set.";                              // 21
const char M22[] PROGMEM = "Clearing address: ";                         // 22
const char M23[] PROGMEM = "Clearing done.";                             // 23
const char M24[] PROGMEM = "Setting defaults..";                         // 24
const char M25[] PROGMEM = "Loading settings..";                         // 25
const char M26[] PROGMEM = "Settings loaded.";                           // 26
const char M27[] PROGMEM = "Device ready.";                              // 27

/* [a] Message Table */
const char* const MSG_TABLE[] PROGMEM = { M00, M01, M02, M03, M04, M05, M06, M07, M08, M09, M10,
                                          M11, M12, M13, M14, M15, M16, M17, M18, M19, M20, M21, 
                                          M22, M23, M24, M25, M26, M27 };
// ==================================================================================================================================

// ==================================================================================================================================
// NOTE: Only Variable Sets A, 1 and 3 can be altered. Editing other variables might make the system unstable.
// ==================================================================================================================================

void setup () {
  systemInit();
}

void loop () {
  delay(PERIOD);
  systemFunc();
  
  if (SW1.pressed() && modePROG != PROG_SYSTEM_INTR_LOCK) { SW1_func(); }
  else if (SW2.pressed() && modePROG != RESET && modePROG != PROG_SYSTEM_INTR_LOCK) { SW2_func(); }
  else if (SW3.pressed() && modePROG != RESET && modePROG != PROG_SYSTEM_INTR_LOCK) { SW3_func(); }
  else if (SW4.pressed() && modePROG != RESET && modePROG != PROG_SYSTEM_INTR_LOCK) { SW4_func(); }
  else if (LCK.pressed2()) { LCK_func(); } 
  else if (CLR.pressed2()) { CLR_func(); }
  else if (RST.pressed2()) { RST_func(); }
}
