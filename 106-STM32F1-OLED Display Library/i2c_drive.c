#include "oled_drive.h"


/*
I2C2
PB10 -> SCL
PB11 -> SDA

I2C1
PB6 -> SCL
PB7 -> SDA

*/


// i2c_init()
void i2c_init(char i2c,unsigned short speed_mode)
{
	RCC->APB2ENR |= 1; 
	
	if(i2c==1)
	{
		RCC->APB1ENR |= 0x200000;
		// Pin enable 
		init_GP(PB,6,OUT50,O_AF_OD);
		init_GP(PB,7,OUT50,O_AF_OD);
		I2C1->CR1 |= 0x8000;
		I2C1->CR1 &= ~0x8000;
		I2C1->CR2 =0x8;
		I2C1->CCR = speed_mode;
		I2C1->TRISE = 0x9;
		I2C1->CR1 |= 1;
	}
	else if(i2c==2)
	{
		RCC->APB1ENR |= 0x400000;
		// Pin enable 
		init_GP(PB,10,OUT50,O_AF_OD);
		init_GP(PB,11,OUT50,O_AF_OD);
		I2C2->CR1 |= 0x8000;
		I2C2->CR1 &= ~0x8000;
		I2C2->CR2 =0x8;
		I2C2->CCR = speed_mode;
		I2C2->TRISE = 0x9;
		I2C2->CR1 |= 1;
	}

}

// Start step
void i2c_start(char i2c)
{
	if(i2c==1)
	{
		I2C1->CR1 |= 0x100;
		while (!(I2C1->SR1 & 1)){};
	}
	else if(i2c==2)
	{
		I2C2->CR1 |= 0x100;
		while (!(I2C2->SR1 & 1)){};
	}
}
// Sending the address + R or Write	
void i2c_add(char i2c, char address,char RW)
{
	volatile int tmp;
	if(i2c==1)
	{
		I2C1->DR = (address|RW);
		while((I2C1->SR1 & 2)==0){};
		while((I2C1->SR1 & 2)){
			tmp = I2C1->SR1;
			tmp = I2C1->SR2;
			if((I2C1->SR1 & 2)==0)
			{
				break;
			}
		}
	}
	else if(i2c==2)
	{
		I2C2->DR = (address|RW);
		while((I2C2->SR1 & 2)==0){};
		while((I2C2->SR1 & 2)){
			tmp = I2C2->SR1;
			tmp = I2C2->SR2;
			if((I2C2->SR1 & 2)==0)
			{
				break;
			}
		}
	}

}
// Sending data step
void i2c_data(char i2c,char data)
{
	if(i2c==1)
	{
		while((I2C1->SR1 & 0x80) == 0){}
			I2C1->DR = data;
		while((I2C1->SR1 & 0x80) == 0){}
	}
	else if(i2c==2)
	{
		while((I2C2->SR1 & 0x80) == 0){}
			I2C2->DR = data;
		while((I2C2->SR1 & 0x80) == 0){}
	}
}
// Stop step
void i2c_stop(char i2c)
{
	volatile int tmp;
	if(i2c==1)
	{
		tmp = I2C1->SR1;
		tmp = I2C1->SR2;
		I2C1->CR1 |= 0x200;
	}
	else if(i2c==2)
	{
		tmp = I2C2->SR1;
		tmp = I2C2->SR2;
		I2C2->CR1 |= 0x200;
	}
}
// i2c_write()
void i2c_write(char i2c, char address,char data[])
{
	int i = 0;
	
	i2c_start(i2c);
	
	i2c_add(i2c, address,0);
	
	while(data[i])
		{
			i2c_data(i2c,data[i]);
			i++;
		}
	i2c_stop(i2c);
}

void update_str_buffer_bit(short Ypos, short Xpos,int text_space,char str[], unsigned char screen_buffer[][128])
{
	int i,j;
	int x_dir, y_dir,startx, endx,endy,starty,cnt, cnt_1,cnt_3,y_offset;
	int bit_y_pos_byte =0; // get the number of bytes
	int start_img_x, start_img_x_temp;
	int start_img_byte = 0; /// Variable to check from where getting the downlad
	int buffer_height = 8;
	int buffer_width = 127;
	//int text_space =72;
	int str_w,str_h,str_len;
	
	// Get str dimension
	i=0;
	str_len=0;
	while(str[i])
	{
		str_len++;
		i++;
	}

	str_len *= 5;
	if(text_space == -1)
	{
		text_space = str_len;
	}
	else if(text_space>buffer_width)
	{
		text_space = buffer_width;
	}
	if(str_len>text_space)
	{
		str_w = text_space;
	}
	else
	{
		str_w = str_len;
	}
	str_h = str_len / text_space;
	if(str_len % text_space)
	{
		str_h++;
	}
	
	// Find endx
	if((Xpos)>buffer_width)
	{
		endx = buffer_width;
	}
	else if((Xpos)<1)
	{
		endx = 0;
	}
	else{
		endx = Xpos;
	}
	//find startx
	startx = Xpos-str_w;
	if(startx > buffer_width)
	{
		startx = buffer_width;
	}
	else if(startx<0)
	{
		startx = 0;
	}
		// Starting x_location from the 
	
	start_img_x = str_w - Xpos;
	if(start_img_x<0)
	{
		start_img_x = 0;
	}
	

	y_offset = Ypos % 8;/*
	if(y_offset == 0)
	{
		bit_y_pos_byte = Ypos/8;
		if(bit_y_pos_byte>buffer_height)
		{
			bit_y_pos_byte = buffer_height;
		}
		
		if(bit_y_pos_byte>buffer_height)
		{
			endy = buffer_height;
		}
		else
		{
			endy = bit_y_pos_byte;
		}
		
		start_img_byte = str_h - bit_y_pos_byte;
		if(start_img_byte < 0)
		{
		start_img_byte = 0;
		}
		
		
		starty = Ypos / 8 - str_h;
		if(starty > buffer_height)
		{
			starty = buffer_height;
		}
		else if (starty < 0)
		{
			starty = 0;
		}else if(start_img_byte>=0)
		{
			start_img_byte = 0;
		}

		for(y_dir = starty; y_dir<endy; y_dir++)
		{
			cnt=0;
			start_img_x_temp =start_img_x;
			for (x_dir= startx;x_dir < endx; x_dir ++)
			{
				cnt_1 =(start_img_byte)*str_w +start_img_x_temp;
				if((cnt%5)==0)
				{
					
					for(j=0;j<5;j++)
					{
						screen_buffer[y_dir][x_dir] = (ASCII[str[cnt_1/5]-32][j]);
						start_img_x_temp++;
						x_dir++;
						cnt++;
						if(x_dir == endx)
						{
							break;
						}
					}
					x_dir--;
				}
				else
				{
					start_img_x_temp++;
					cnt++;
				}
				if(cnt_1 >= str_len-1)
				{
					break;
				}
				
			}
			start_img_byte++;
			
		}
	}
	else
	{*/
		bit_y_pos_byte = Ypos/8;
		if(Ypos%8)
		/*
		if(bit_y_pos_byte>buffer_height)
		{
			bit_y_pos_byte = buffer_height;
		}
		*/
		endy =  bit_y_pos_byte +1;
		if(endy> buffer_height)
		{
			endy = buffer_height;
		}
		
		start_img_byte =   str_h - bit_y_pos_byte -1;
		starty = Ypos / 8 - str_h;

		
		if(starty > buffer_height)
		{
			starty = buffer_height;
		}
		else if (starty < 0)
		{
			starty = 0;

		}
		else
		{
			//starty--;
			start_img_byte = -1;
		}
		
i=0;
			for(y_dir = starty; y_dir<endy; y_dir++)
		{
			start_img_x_temp =start_img_x;
			cnt=0;
			for (x_dir= startx;x_dir < endx; x_dir ++)
			{
				if(start_img_byte==-1)
				{
				cnt_1 = start_img_x_temp;
				}
				else
				{
				cnt_1 =(start_img_byte)*str_w +start_img_x_temp;
				}
				cnt_3 =(start_img_byte+1)*str_w +start_img_x_temp;
					
				//
				if((cnt%5)==0)
				{
					
					for(j=0;j<5;j++)
					{
								if((cnt_1+j) > (str_len))
								{
									break;
								}
								 if(start_img_byte<0)
										{
											//Draw the beg of the pic
											screen_buffer[y_dir][x_dir] = (ASCII[str[cnt_1/5]-32][j]<<(y_offset));
										}
								else if(start_img_byte<str_h-1)
									{
										
										if(cnt_3 <= str_len)
										{
										screen_buffer[y_dir][x_dir] = (ASCII[str[cnt_3/5]-32][j]<<( y_offset)) | (ASCII[str[cnt_1/5]-32][j]>>(8- y_offset)) ;
										}
										else
										{
											screen_buffer[y_dir][x_dir] = (ASCII[str[cnt_1/5]-32][j]>>(8- y_offset)) ;
										}
									}
								else
									{
										//Draw the end of the pic

											screen_buffer[y_dir][x_dir] = (ASCII[str[cnt_1/5]-32][j]>> (8-y_offset));
											

									}
								start_img_x_temp++;
								cnt++;
								x_dir++;
								if(x_dir >= endx)
									{
										break;
									}
					}
					
					x_dir--;
					
						if(cnt_1 > str_len)
						{
							break;
						}
				}
				else
				{
					start_img_x_temp++;
					cnt++;
				}
				
			
			}
			start_img_byte++;
		}
	//}
}

