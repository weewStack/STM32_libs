#include "stm32f10x.h" 
#include "systick_time.h"
#include "gp_drive.h" 
#include "n5110_drive.h"

/*
Pin setup

SPI - 1
--> 
PA0 -->RST
PA1 --> DC
PA2 -->Screen Light
PA4 --> SS
PA5 --> SCLK
PA7 --> MOSI

3.3V --> Vcc
G --> Grd
*/

unsigned char n5110_buffer[6][84];


int main(void)
{
	int i= 0;
	int j;
systick_init();
n5110_init(1); // Initialize the Nokia Driver using SPI 1

// Imag type is up to 30 image	
//Setup the Subscribe Image type
ImgType weew_pic = {.h = weew_stack_rows, .w = weew_stack_cols, .x_pos = 0, .y_pos =0}; /// Setup the initial starting picture
weew_pic.image[0] = weew_stack;

//Setup the Subscribe Image type
ImgType subs = {.h = subscribe_rows, .w = subscribe_cols, .x_pos = 0, .y_pos =0}; /// Setup the initial starting picture
subs.image[0] = subscribe;

// Like
ImgType like_img = {.h = like_rows, .w = like_cols, .x_pos = 10, .y_pos =0}; /// Setup the initial starting picture
like_img.image[0] = like;

//Setup the Walking men
ImgType walkmen = {.h = walkmen_4_rows, .w = walkmen_4_cols, .x_pos = 0, .y_pos =0};
walkmen.image[0] = walkmen_0;
walkmen.image[1] = walkmen_1;
walkmen.image[2] = walkmen_2;
walkmen.image[3] = walkmen_3;
walkmen.image[4] = walkmen_4;


// Initiate the inputs
Digital_Input(PA,10);
Digital_Input(PA,9);


//Setup the startup screen Image and message
n5110_clear(); // Clear the Nokia device
clear_buffer(n5110_buffer); // Clear the Buffer
update_buffer(weew_pic, 0, n5110_buffer); // Updating the the buffer with an image --> update_buffer(ImgType, Image Number, Nokia Buffer );
update_str_buffer(5, 15," Hello WeeW ",n5110_buffer); // Updating buffer with string --> update_str_buffer(Yposition, Xposition,**Sting**, Nokia Buffer);
print_buffer(n5110_buffer); // Printing the buffer into the Nokia
DelayMs(3000);
n5110_ligh(HIGH); // Set the Nokia screen light on

clear_buffer(n5110_buffer);
update_buffer(subs, 0, n5110_buffer);
print_buffer(n5110_buffer);
DelayMs(3000);

clear_buffer(n5110_buffer);
update_buffer(like_img, 0, n5110_buffer);
print_buffer(n5110_buffer);
DelayMs(3000);

	while(1)
	{
		
		if(R_GP(PA,10))
		{
			if(i > 0)
			{
				i--;
			}
			walkmen.x_pos = i*3;
			for(j=0;j<4;j++)
			{
				clear_buffer(n5110_buffer);
				update_buffer(walkmen, j, n5110_buffer);
				print_buffer(n5110_buffer);
				DelayMs(100);
			}
		}
		else if(R_GP(PA,9))
		{
			if(i < 22)
			{
				i++;
			}
			walkmen.x_pos = i*3;
			for(j=0;j<4;j++)
			{
				clear_buffer(n5110_buffer);
				update_buffer(walkmen, j, n5110_buffer);
				print_buffer(n5110_buffer);
				DelayMs(100);
			}
		} 
		else
			{
				clear_buffer(n5110_buffer);
				update_buffer(walkmen, 4, n5110_buffer);
				update_str_buffer(5, 5,"** Just WeeW **",n5110_buffer);
				print_buffer(n5110_buffer);
				DelayMs(1000);
			}
	}
}
