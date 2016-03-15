/****************************************************************
***	 LCDMinion.cpp
***
***	 This object provides services to output data onto a vex
***	 cortex LCD display.	It utilizes upto MAX_SCREENS virtual
***	 screens to allow multiple users to display information.
***	 The virtual screen to be displayed is selected by using
***  the left and center push buttons on the VEX LCD device.
***  Left is up, center is down.
***  Alerts can be recieved from calling functions to
***  switch the current channel to the channel with the alert.
***  The received alert is also indicated by flashing the LCD
***  backlight.  The right push button is used to dismiss the
***  alert.
***
***	 Usage Example:
***		 startTask(LCDMinion);
***
***	 Public Functions:
***		 int reqLCDChan (void)	// Allocates a virtual channel
***		 void dispLCDChan(int chan, LCDScreen disp, bool alert)
***		 void clearLCDAlert (int chan) // Same as right button.
***
***	 Private Functions:
***
***	 Tasks:
***		 task FlashLight ()
***		 task LCDMinion ()
***
****************************************************************/


// Public functions *********************************************

/****************************************************************
***	 Request an LCD Channel to use.
****************************************************************/

int reqLCDChan (void)
{
	int newChan;

	// Allocate new channel
	newChan = numChan;
	++numChan;

	// Enable Alerts
	panelLCD[newChan].alertEnable=true;

	return (newChan);
}

// Task *********************************************************

/****************************************************************
***	 FlashLight flashes the LCD backlight
****************************************************************/

task FlashLight ()
{
	while(true)
	{
		bLCDBacklight=false;
		wait1Msec(FLASHLIGHT_WAIT);
		bLCDBacklight=true;
		wait1Msec(FLASHLIGHT_WAIT);
	}
}

/****************************************************************
***	 Display an LCD channel.
****************************************************************/

void dispLCDChan(int chan, LCDScreen disp, bool alert)
{
	// store incoming display in data structure
	screens[chan].line0 = disp.line0;
	screens[chan].line1 = disp.line1;

	// Process alert
	if ((alert) && (panelLCD[chan].alertEnable) && (!panelLCD[chan].isAlert))
	{
		// switch current channel to chan
		curChan = chan;

		// start backlight flash
		startTask(FlashLight);

		panelLCD[chan].isAlert = true;
	}

	if (!alert)
		panelLCD[chan].isAlert = false;
}

/****************************************************************
***	 Clear an LCD alert.
****************************************************************/

void clearLCDAlert (int chan)
{
	// Toggle alert enable for current channel
	panelLCD[chan].alertEnable = !panelLCD[chan].alertEnable;

	if (!panelLCD[chan].alertEnable)
	{
		// stop backlight flash
		stopTask(FlashLight);

		// Turn backlight on
		bLCDBacklight = true;
	}
	else
	{
		panelLCD[chan].isAlert = false;
	}
}


// Private functions ********************************************

// Tasks ********************************************************

/****************************************************************
***	 LCDMinion
****************************************************************/

task LCDMinion ()
{
	const int clickWait = 500;  // mSec to wait for a press and hold
	int i;                      // loop vars
	int btnLCDPressed;          // local button value variable


	numChan = 1;
	curChan = 0;

	// Initialize Channel 0 Banner
	screens[0].line0 = bannerLine0;
	screens[0].line1 = bannerLine1;

	// Turn backlight on
	bLCDBacklight = true;

	while(true)
	{
		// Check for button press
		btnLCDPressed = nLCDButtons;
		if (btnLCDPressed > 0)
		{
			// Check for click and hold
		  // Process button every clickWait time or when released
			for(i = 0; i < clickWait; ++i)
			{
				if (!nLCDButtons)
					break;

				wait1Msec(1);
			}

			switch (btnLCDPressed)
			{
			case 1:	 // Left "Up" Buttion
				// Increment channel
				++curChan;
				// if channel above top set channel to 0
				if(curChan==numChan)
					curChan=0;
				break;

			case 2:	 // Center "Down" Buttion
				// Decrement channel
				--curChan;
				// if channel below 0 set channel to top
				if(curChan<0)
					curChan=numChan-1;
				break;

			case 4:	 // Right "Alert" Buttion
				// Toggle alert enable for current channel
				panelLCD[curChan].alertEnable = !panelLCD[curChan].alertEnable;

				if (!panelLCD[curChan].alertEnable)
				{
					// stop backlight flash
					stopTask(FlashLight);

					// Turn backlight on
					bLCDBacklight = true;
				}
				else
				{
					panelLCD[curChan].isAlert = false;
				}
				break;
			}
		}
		// Display Current Channel
		displayLCDString (0,0, screens[curChan].line0);
		displayLCDString (1,0, screens[curChan].line1);

		wait1Msec(LCDMINION_WAIT);
	}
}
