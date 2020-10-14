#include "data_logger.h"



/*
I2C2
PB10 -> SCL
PB11 -> SDA

I2C1
PB6 -> SCL
PB7 -> SDA
*/


char reset(void);
char prev(void);
char next(void);
char record(void);



int  records = 0;
char info_msg[50] = "Total records: ";
int main(void)
{
	
	systick_init();
	
	Digital_Input(PB,12); // Used as record button
	Digital_Input(PB,13); // Used as next button
	Digital_Input(PB,14); // Used as prev button

	
	
	// You need to need to initialize this function if you are not going to oled_init_64(2); 
	//i2c_init(2,i2c_FM); // 
	
	// initialize the oled_screen
	oled_init_64(2); // oled_init_64(I2C);
	
	// Clear the the screen
	oled_blank(2); // oled_blank(I2C);

	
	// In case of the EEPROM logger master is not initialized:
	logger_init(2); // logger_init(I2C); 
	
	
	
	// Update into the Oled the number of the record into the EERPOM
	logger_oled_tot_rec(2,info_msg); // logger_oled_tot_rec(i2c,array: "message to display);
	
	// Show the number of record list
	logger_oled_rec_progress(2,0); // logger_oled_rec_progress(i2c,0: as default and if there is a record);
	
	if(get_total_records(2)) // Getting the number of records
	{
		// Display the first time of the record
		logger_oled_display_time(2,1); // logger_oled_display_time(i2c,The record number);
		//Display the first Date of the record
		logger_oled_display_date(2,1); // logger_oled_display_date(i2c,The record number);
	}
	
	oled_msg(2,3,0,"-----------------------");// Just for separation matter
	oled_msg(2,6,0,"-----------------------");// Just for separation matter
	
	while(1)
	{
		
		// The reset will reset the EEPROM Master
		if(reset())
		{
			oled_blank(2);
			oled_msg(2,3,0,"-----------------------");
			oled_msg(2,6,0,"-----------------------");
			logger_reset(2);
			logger_oled_tot_rec(2,info_msg);
			logger_oled_rec_progress(2,0);
			DelayMs(500);
			records = 0;
		}
		else 
		
		// The record function will add a new record into the EEPROM memory
		if(record())
		{		
			logger_time_rec(2);
			logger_oled_tot_rec(2,info_msg);
			
			if(get_total_records(2)==1)
			{
				logger_oled_rec_progress(2,0);
				logger_oled_display_time(2,1);
				logger_oled_display_date(2,1);
			}
			else
			{
				logger_oled_rec_progress(2,records);
			}
			DelayMs(200);
		}
		
		
		// The prev function will display the  previous record (if it exist and if possible) into the Oled screen
		else if(prev())
		{
			if(get_total_records(2))
			{
				if(records==0)
				{
					records = 0;
				}
				else
				{
					records--;
				}
				logger_oled_rec_progress(2,records);
				logger_oled_display_time(2,records);
				logger_oled_display_date(2,records);
				DelayMs(200);
			}
		}
		// The next function will display the next record (if it exist and if possible) into the Oled screen
		else if(next())
		{
			if(get_total_records(2))
			{
				if(records== (get_total_records(2)-1))
				{
					records = records;
				}
				else
				{
					records++;
				}
				logger_oled_rec_progress(2,records);
				logger_oled_display_time(2,records);
				logger_oled_display_date(2,records);
				DelayMs(200);
			}
		}
	}
	
}
// Return 1 if both PB14 & PB13 are pushed
char reset(void)
{
	char reset = 0;
	if(R_GP(PB,13) & R_GP(PB,14))
	{
		reset = 1;
	}
		return reset;
}
// Return 1 if PB14 is pushed
char prev(void)
{	
	return R_GP(PB,14);
}
// Return 1 if PB13 is pushed
char next(void)
{	
	return R_GP(PB,13);
}
// Return 1 if PB12 is pushed
char record(void)
{	
	return R_GP(PB,12);
}

