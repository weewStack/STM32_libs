#include "stm32f10x.h" 
#include "gp_drive.h"

/*
Pin setup

PA0 -> Digital output
PA10 -> Digital Input

*/


int main(void)
{

	Digital_Input(PA,10);  // Set PORTA PIN-10 as Digital Inpout
	Digital_Output(PA,0); // Set PORTA PIN-0 as Digital Output

	
	
	while(1)
	{
			// --> R_GP(PORT_NAME, PIN_NUMBER) => Return HIGH or LOW
		  // --> W_GP(PORT_NAME, PIN_NUMBER, HIGH_LOW) => No return 
		
			W_GP(PA,0,R_GP(PA,10));	
	}
	
}

