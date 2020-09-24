#include "systick_time.h"
#include "bno055_drive.h"
#include "animation_drive.h"

/*
I2C2
PB10 -> SCL
PB11 -> SDA

I2C1
PB6 -> SCL
PB7 -> SDA
*/

short angles[3]; // Array to get the euler angles from the BNO055
unsigned char oled_buffer[8][128]; // Array to be used for the final display into the OLED display
	


int main(void)
{
	
	bno55_init(2); // Initializing the BNO055 to be in the Fusion mode using I2C2 Peripheral
	oled_init_64(2); // Initialize the Oled 64
	oled_blank(2); // Make the whole screen blank for future new display
 
	
	
	// Walkmen Set up
	ImgType walkmen = {.h = walkmen_4_rows, .w = walkmen_4_cols, .x_pos = 0, .y_pos =0, .bit_y_pos = 0};
	walkmen.image[0] = walkmen_0;
	walkmen.image[1] = walkmen_1;
	walkmen.image[2] = walkmen_2;
	walkmen.image[3] = walkmen_3;
	walkmen.image[4] = walkmen_4;
	walkmen.image[5] = walkmen_5;
	walkmen.image[6] = walkmen_6;
	walkmen.image[7] = walkmen_7;
	walkmen.image[8] = walkmen_8;
	clear_buffer(oled_buffer);
	center(&walkmen);
	update_buffer_bit(walkmen,4,oled_buffer);
	print_buffer(2,oled_buffer);
	
	DelayMs(2000);
	
	
	
	
	int j,right_left;
	
	
	while(1)
	{
		
		// Getting the Euler angular position
		bno055_eul(2,angles); // bno055_eul(**I2C peripheral number**,**Array to get the data from BNO055**)
		
		if(IMU_motion(&walkmen,angles,&right_left)) // 
		{
			if(right_left)
					{
					for(j=0;j<4;j++)
						{
							clear_buffer(oled_buffer);
							update_buffer_bit(walkmen, j, oled_buffer);
							print_buffer(2,oled_buffer);
							DelayMs(25);
						}
					}
					else
					{
						for(j=5;j<9;j++)
						{
							clear_buffer(oled_buffer);
							update_buffer_bit(walkmen, j, oled_buffer);
							print_buffer(2,oled_buffer);
							DelayMs(25);
						}
					}
		}
			else
			{
				clear_buffer(oled_buffer);
				update_buffer_bit(walkmen, 4, oled_buffer);
				update_str_buffer(7, 15,"** Just WeeW **",oled_buffer);
				print_buffer(2,oled_buffer);
				DelayMs(500);
			}
		}
	
}

