#include "systick_time.h"
#include "i2c_drive.h"
#include "oled_drive.h"

unsigned char oled_buffer[8][128];
int up(void);
int down(void);
int left(void);
int right(void);
int step(void);

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
	int i,j,right_left;
	int textstep = 0;
	int textpos =127;
	int x_dir = 20;
	int y_dir = 50;
	
	
	systick_init();
	for(i=12;i<16;i++)
	{
		Digital_Input(PB,i); // Set the pins as inputs
	}
	
	
	oled_init_64(2);
	oled_blank(2); // Clear the the screen
	// oled_msg(I2C,Y_Byte position,X_position,Message to display);
	oled_msg(2,3,30,"Hello WeeW");// function to display text in the OLED 
	DelayMs(2000);
	
	ImgType Saitama = {.h = saitama_rows, .w=saitama_cols, .x_pos =100,.y_pos =0, .bit_y_pos =64};
	Saitama.image[0] = saitama;
	clear_buffer(oled_buffer);
	update_buffer_bit(Saitama,0,oled_buffer);
	print_buffer(2,oled_buffer);
	
	DelayMs(2000);

	//Setup the Subscribe Image type
	ImgType subs = {.h = subscribe_rows, .w = subscribe_cols, .x_pos = 100, .y_pos =0, .bit_y_pos = 0}; /// Setup the initial starting picture
	subs.image[0] = subscribe;
	

	// Like
	ImgType like_img = {.h = like_rows, .w = like_cols, .x_pos = 90, .y_pos =0, .bit_y_pos = 50}; /// Setup the initial starting picture
	like_img.image[0] = like;
	
		for(subs.bit_y_pos=0;subs.bit_y_pos<60;subs.bit_y_pos++)
	{
		clear_buffer(oled_buffer);
		update_buffer_bit(subs, 0, oled_buffer);
		print_buffer(2,oled_buffer);
	}
	DelayMs(2000);
	
	
	clear_buffer(oled_buffer);
	update_buffer_bit(like_img, 0, oled_buffer);
	print_buffer(2,oled_buffer);
	DelayMs(2000);
	
	

	
	// Walkmen Set up
	ImgType walkmen = {.h = walkmen_4_rows, .w = walkmen_4_cols, .x_pos = 20, .y_pos =0, .bit_y_pos = 50};
	walkmen.image[0] = walkmen_0;
	walkmen.image[1] = walkmen_1;
	walkmen.image[2] = walkmen_2;
	walkmen.image[3] = walkmen_3;
	walkmen.image[4] = walkmen_4;
	walkmen.image[5] = walkmen_5;
	walkmen.image[6] = walkmen_6;
	walkmen.image[7] = walkmen_7;
	walkmen.image[8] = walkmen_8;
	
	

	
	


	oled_blank(2);
	oled_msg(2,3,30,"Text ready");

	while(1)
	{
		
		while((up() & down())==0)
		{
			if(step())
			{
			if(up())
				{
					textstep--;
				}
			if(down())
				{
					textstep++;
				}
			if(right())
			{
				textpos ++;
			}
			if(left())
			{
				textpos--;
			}
			clear_buffer(oled_buffer);
			update_str_buffer_bit(textstep,textpos,70,"Hello WeeW is a youTube channel \
				that aims to share Engineering knowledge, the idea is to create code \
				from scratch and try make it better again and again!!!", oled_buffer);
			print_buffer(2,oled_buffer);
			}
		}
		
		
		oled_blank(2);
		oled_msg(2,3,0,"Going to to the WeeW Men");
		DelayMs(3000);
		while((right() & left()) == 0)	
		{
			if(step())
			{
				if(up())
				{
					y_dir -=2;
				}
				if(down())
				{
					y_dir+=2;
				}
				if(left())
				{
					x_dir-=2;
					right_left = 0;
				}
				if(right())
				{
					x_dir+=2;
					right_left = 1;
				}
					walkmen.x_pos = x_dir;
					walkmen.bit_y_pos = y_dir;
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
			}
		}
		
		//Break before movin to text section
		oled_blank(2);
		oled_msg(2,3,0,"Text scrolling will start");
		DelayMs(3000);
		
	}
	
	
}

int down(void)
{
	return R_GP(PB,12);
}
int up(void)
{
	return R_GP(PB,15);
}
int left(void)
{
	return R_GP(PB,13);
}
int right(void)
{
	return R_GP(PB,14);
}

int step(void)
{
	return right() | left() | up() | down();
}

