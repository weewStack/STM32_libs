#include "systick_time.h"
#include "bno055_drive.h"
#include "oled_drive.h"


/*
I2C2
PB10 -> SCL
PB11 -> SDA

I2C1
PB6 -> SCL
PB7 -> SDA
*/

short angles[3];


int main(void)
{
	bno55_init(2); // Initializing the BNO055 to be in the Fusion mode using I2C2 Peripheral
	oled_init_64(2); // Initialize the Oled 64
	oled_blank(2); // Make the whole screen blank for future new display
	
	oled_Title_logger(2,0,"Yaw: "); // oled_Title_logger(**I2C peripheral number**,**Row number**,**Measurement title **);
	oled_Title_logger(2,2,"Roll: ");// Add a data logger title
	oled_Title_logger(2,4,"Pitch: ");
	
	while(1)
	{
		
		bno055_eul(2,angles); // bno055_eul(**I2C peripheral number**,**Array to get the data from BNO055**)
		
		oled_Data_logger(2,0,angles[Yaw]); // oled_Data_logger(**I2C peripheral number**,**Row number**,**Measurement (must be int)**)
		oled_Data_logger(2,2,angles[Roll]);// Add a new data log
		oled_Data_logger(2,4,angles[Pitch]);
		
	}
	
}

