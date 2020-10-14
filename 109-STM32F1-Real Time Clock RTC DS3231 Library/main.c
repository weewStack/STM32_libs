#include "systick_time.h"
#include "DS3231_Drive.h"
#include "oled_drive.h"

/*
I2C2
PB10 -> SCL
PB11 -> SDA

I2C1
PB6 -> SCL
PB7 -> SDA


*/




char yr = 20;
char month = 10;
char day = 3;
char hrs = 22;
char min = 11;
char sec = 15;
char time_table[7];
char time_str[12],date_str[12];


int main(void)
{
	systick_init();
	
	
	oled_init_64(2); // 
	oled_blank(2);
	
	
	//i2c_init(2,i2c_FM); // If using only the the DS3231 Chip

	//DS3231_update_date_time(2,yr,month,day,hrs,min,sec); // Update the time & Date
	
	
	DelayMs(500);
	

	
	while(1)
	{
		
		DS3231_get_time(2,time_table);
		
		DS3231_Time_Date(time_table,time_str,Time);// --> Create a time string format hh/mm/ss
		
		DS3231_Time_Date(time_table,date_str,Date);// --> Create a date string format yy/mm/dd
		
		oled_msg(2,2,35,time_str); // Display the string into the oled
		
		oled_msg(2,4,35,date_str); // Display the string into the oled
		
		
		
		str_empty(date_str);
		str_empty(time_str);
		
		DelayMs(1000);
	}
	
}


