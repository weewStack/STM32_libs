#include "gp_drive.h"
#include "systick_time.h"

/*
Pin setup

PA0  -> Digital output
PA10 -> Digital Input
PA9  -> Digital Input

*/

unsigned int i = 0; 

int main(void)
{
	
	systick_init();// initialize the delay function (Must initialize)
		
	Digital_Input(PA,10);  // Set PORTA PIN-10 as Digital Inpout
	Digital_Input(PA,9);  // Set PORTA PIN-10 as Digital Inpout
	Digital_Output(PA,0); // Set PORTA PIN-0 as Digital Output

		
	while(1)
	{		
			if(R_GP(PA,10))   // --> R_GP(PORT_NAME, PIN_NUMBER) => Return HIGH (1) or LOW(0)
				{
					DelayMs(50);
					i++;
				}
			else if(R_GP(PA,9))  // --> R_GP(PORT_NAME, PIN_NUMBER) => Return HIGH (1) or LOW(0)
				{
					DelayMs(50);
					if(i > 0)
					{
						i--;
					}
				}
				
			DelayMs(100 + i*100);
			toggle_GP(PA,0);	// --> toggle_GP(PORT_NAME, PIN_NUMBER) => No return but toggle the PIN
	}
	
}

