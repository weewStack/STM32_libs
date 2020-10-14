#include "data_logger.h"

// Getting a byte position from a short, int, long...
char Get_byte(char byte_pos,long num)
{
	char result = (num >> (byte_pos*8));
	return result;
}



// Incase of reading 2 succuccisve addresses and combining them into 1 number
short Get_short(char i2c,short start_address)
{
	short result = 0;
	char num1 = AT24C_load_byte(i2c,start_address);
	char num2 = AT24C_load_byte(i2c,start_address+1);
	result = (short) num1;
	result =  (result << 8);
	result |=  num2;
	return result;
}


// Incase of recording 1 number into 2 succuccisve addresses
void ATC24_memo_update_2byte(char i2c,short address,int val)
{
	AT24C_save_byte(i2c,address,Get_byte(1,val));
	AT24C_save_byte(i2c,address+1,Get_byte(0,val));
}

/*
//First 32 bytes (Page_0) of data
// Adress 0 : Version of the save 1: byte
// Adress 1 :Data size: Memore size :2-byte
// Adress 2 :Data size: Memore size :2-byte
// Adress 3: word size : 1-2-4-8 byte
// Adress 4: Number of words in a record : 1 byte
// Adress 5: Number of records 2-bytes (unsigned short)
// Adress 6: Number of records 2-bytes  (unsigned short)
// Adress 7: New address to save (2-bytes)
// Adress 8: New address to save (2-bytes)
*/
void logger_init(char i2c)
{
	char check = AT24C_load_byte(i2c,Version);
	if(check == 1)
	{}
	else{
		AT24C_save_byte(i2c,Version,1);
		ATC24_memo_update_2byte(i2c,Memory,0x0fff);
		AT24C_save_byte(i2c,Word_size,1); // 1 byte by word 
		AT24C_save_byte(i2c,Rec_size,8);
		ATC24_memo_update_2byte(i2c,Rec_tot,0x0);
		ATC24_memo_update_2byte(i2c,Rec_Add,32); // Because we start from page 1
	}
}
void logger_reset(char i2c)
{
		AT24C_save_byte(i2c,Version,1);
		ATC24_memo_update_2byte(i2c,Memory,0x0fff);
		AT24C_save_byte(i2c,Word_size,1); // 1 byte by word 
		AT24C_save_byte(i2c,Rec_size,8);
		ATC24_memo_update_2byte(i2c,Rec_tot,0x0);
		ATC24_memo_update_2byte(i2c,Rec_Add,32); // Because we start from page 1
}

char logger_rec(char i2c,char * data)
{
	int i;
	char check =0;
	short memory = Get_short(i2c,Memory);
	short address = Get_short(i2c,Rec_Add);
	short size = AT24C_load_byte(i2c,Rec_size);
	
	if(memory>(address+size))
	{
		check =1;
		short next_page = ((address/32) +1) *32;
		
		if((address +size)> next_page)
		{
			for(i=0;i<=size;i++)
			{
				AT24C_save_byte(i2c,address+i,data[i]);
			}
		}
		else
		{
			AT24C_save_page(i2c,address,data,size);
		}
		address += (size+1);
		ATC24_memo_update_2byte(i2c,Rec_Add,address); 
		ATC24_memo_update_2byte(i2c,Rec_tot,(Get_short(i2c,Rec_tot)+1));
	}
	else
	{
		check = 0;
	}
	return check;
}

char logger_read(char i2c,short record_number, char * data)
{
	
	int i;
	short tot_rec = Get_short(i2c,Rec_tot);
	char check =0;
	if(tot_rec >= record_number)
	{
			check = 1;
			short size = AT24C_load_byte(i2c,Rec_size);
			short address = 32 + size*record_number + record_number;
			short next_page = ((address/32) +1) *32;
			if((address +size)> next_page)
				{
					for(i=0;i<=size;i++)
					{
						data[i] = AT24C_load_byte(i2c,address+i);
					}
				}
				else
				{
					AT24C_load_page(i2c,address,data,size);
				}
	}
	return check;
}

void logger_time_rec(char i2c)
{
	char time_table[10];
	DS3231_get_time(i2c,time_table);
	logger_rec(i2c,time_table);
	str_empty(time_table);
}

short get_total_records(char i2c)
{
	short result = Get_short(2,Rec_tot);
	return result;
}
void get_time_str(char i2c,short record_number,char * time)
{
	char save[10];
	logger_read(2,record_number,save);
	DS3231_Time_Date(save,time,Time);
}
void get_date_str(char i2c,short record_number,char * date)
{
	char save[10];
	logger_read(2,record_number,save);
	DS3231_Time_Date(save,date,Date);
}

/// Addon for OLED library

//Display total records on OLED
void logger_oled_tot_rec(char i2c,char * info_msg)
{
	char num[12];
	int str_size=len_str(info_msg)*5+5;
	str_empty(num);
	int2char(get_total_records(i2c),num);
	//Concatstr(info_msg,num);
	oled_msg(2,1,0,"                     ");
	oled_msg(2,1,0,info_msg);
	oled_msg(2,1,str_size,num);
	str_empty(num);
}
void logger_oled_display_time(char i2c,short record_number)
{
	char time[12];
	str_empty_len(time,12);
	get_time_str(2,record_number,time);
	oled_msg(2,4,30,time);
	str_empty_len(time,12);
}
void logger_oled_display_date(char i2c,short record_number)
{
	char date[12];
	str_empty_len(date,12);
	get_date_str(2,record_number,date);
	oled_msg(2,5,30,date);
	str_empty_len(date,12);
}

void logger_oled_rec_progress(char i2c,short record_number)
{
	char msg[40],num[12];
	str_empty(num);
	str_empty_len(msg,20);
	if(get_total_records(i2c) == 0)
	{
		Concatstr(msg,"0 / 0");
	}
	else{
	int2char(get_total_records(i2c),num);
	Concatstr(msg,num);
	str_empty(num);
	Concatstr(msg," / ");
	int2char((record_number+1),num);
	Concatstr(msg,num);
	str_empty(num);
	}
	oled_msg(2,2,0,"                     ");
	oled_msg(2,2,0,msg);
	str_empty(msg);
}
