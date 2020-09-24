#include "animation_drive.h"


void center(ImgType * Img)
{
	int screen_w = 128;
	int screen_h = 64;
	int  img_x_bit = Img->w;
	int img_y_bit = Img->h*8;
	Img->x_pos = screen_w/2 + img_x_bit/2;
	Img->bit_y_pos = screen_h/2 + img_y_bit/2;
}


int IMU_motion(ImgType * Img,short new_pos[],int * right_left)
{
	int motion = 0;
	*right_left = 0;
	int screen_w = 128;
	int screen_h = 64;
	//int x_min =  Img->w;
	int x_max = screen_w;
	int x_area = x_max;

	
	
	//int y_min = Img->h;
	int y_max = screen_h;
	int y_area = y_max;
	
	int  img_x_bit = Img->w;
	int img_y_bit = Img->h*8;
	 

	int center_x = screen_w/2 + img_x_bit/2;
	int center_y = screen_h/2 + img_y_bit/2;
	
	int roll = (new_pos[1])	;
	int pitch = (new_pos[2]);
	
	int roll_step = (roll * y_area)/180;
	int pitch_step = (pitch* x_area)/180;
	
	int new_x_pos = center_x  - pitch_step; // (x_max - pitch_step);
	int new_y_pos = center_y +  roll_step;//(y_min+ roll_step);
	
	if(new_x_pos != Img->x_pos)
	{
		if(Img->x_pos<new_x_pos)
		{
			*right_left = 1;
		}
		Img->x_pos = new_x_pos;
		motion = 1;
	}
	if(new_y_pos != Img->bit_y_pos)
	{
		Img->bit_y_pos = new_y_pos;
		motion = 1;
	}
	
	return motion;
}
