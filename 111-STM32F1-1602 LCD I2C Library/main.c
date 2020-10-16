#include "systick_time.h"
#include "lcd_1602_drive.h"

/*
I2C2
PB10 -> SCL
PB11 -> SDA

I2C1
PB6 -> SCL
PB7 -> SDA
*/



int main(void)
{
		systick_init();// initialize the delay function (Must initialize)
		// initialize communication and setup the LCD display

		lcd_i2c_init(2);// lcd_i2c_init(I2C_peripheral);
		
		//Display String on the LCD 
	
		lcd_i2c_msg(2,1, 0,"Welcome to the"); // lcd_i2c_msg(I2C_peripheral,Row_1_2, Xposition,"String to display");
	
		lcd_i2c_msg(2,2, 6,"WeeW");
	
	while(1)
	{
		
		// Clear both lines of the LCD
		lcd_blank_12(2); // lcd_blank_12(I2C_peripheral); 
		lcd_i2c_msg(2,2, 0,"Welcome to the");
		lcd_i2c_msg(2,1, 6,"WeeW");
		DelayMs(750);
		lcd_blank_12(2);
		lcd_i2c_msg(2,1, 0,"Welcome to the");
		lcd_i2c_msg(2,2, 6,"WeeW");
		DelayMs(750);
		
	}
	
}

