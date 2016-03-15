/****************************************************************
***
*** LCDMinion.h
***    see LCDMinion.cpp for details
****************************************************************/

#ifndef _LCDMinion_h
#define _LCDMinion_h

#define MAX_CHAN 20     // Maximum Virtual LCD Screens

// public structure of an LCD screen
typedef struct
{
	string line0;   // Line 0 of a virtual LCD screen
	string line1;   // Line 1 of a virtual LCD screen
} LCDScreen;

// private structure for LCD Minion data
typedef struct
{
	bool alertEnable;
  bool isAlert;
} LCDMinionData;

string bannerLine0 = "  Badger Bots   ";
string bannerLine1 = "  Team #6058    ";

#define LCDMINION_WAIT  200 // LCD Screen update wait time.
#define FLASHLIGHT_WAIT 750 // Time backlight is on or off

// private data storage and variables
LCDScreen screens [MAX_CHAN];  // Array storage of virual screens
LCDMinionData panelLCD [MAX_CHAN];  // Array storage of LCD data
int numChan;  // Total number of used channels
int curChan;  // Current channel being displayed

#include "LCDMinion.cpp"

#endif
