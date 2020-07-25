#include "gp_drive.h" // GPIO library for STM32F1
#include "systick_time.h" // Library to get the delay function
#include "lcd_1602_drive.h" // 1602 LCD library
#include "help_func.h" // Mainly function for string transformation
/*
Pin setup : 4 bits data communication

PA8 -> RS
PA9 -> RW
PA10-> E

PA4 -> DB4
PA5 -> DB5
PA6 -> DB6
PA7 -> DB7
*/


char string[3]; // For Int conversation
unsigned int i = 0;  // as coounter

int main(void)
{
		systick_init();// initialize the delay function (Must initialize)
		Digital_Input(PA,11); // Initialize push botton
		
		lcd_init(); // Initialize the LCD screen to work with 4 bits data interface
	
	
	
	
		lcd_msg(1, 1,"Welcome to the");// lcd_msg(Line_number 1 or 2, Position within the line, String to be shown)
		lcd_msg(2, 6,"WeeW");
	
	
	DelayMs(3000);
	lcd_msg(2,0, "INTEGER:       ");
	lcd_msg(1,0, "ASCII:         ");
	
	while(1)
	{
			if(R_GP(PA,11))
				{
					
					int2char(i,string);// Convert the integer to string
					lcd_cmd(0x8A); // function that will send intruction to LCD screen
					lcd_data(i);
					
					lcd_msg(2,10, "    "); // Load and 
					lcd_msg(2,10, string);
					str_empty(string);
					i++;
					if(i>127)
					{
						i=0;
					}
					DelayMs(20);
					
				}

	}
	
}

