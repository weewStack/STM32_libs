#include "systick_time.h"
#include "AT24C_Drive.h"
#include "DS3231_Drive.h"
#include "oled_drive.h"

enum {
 Version = 0,
 Memory  = 1,
 Word_size = 3,
 Rec_size = 4,
 Rec_tot = 5,
 Rec_Add = 7
};


// Core functions to manage the logger

char Get_byte(char byte_pos,long num);
short Get_short(char i2c,short start_address);
void ATC24_memo_update_2byte(char i2c,short address,int val);
void logger_init(char i2c);
char logger_rec(char i2c,char * data);
char logger_read(char i2c,short record_number, char * data); 
void logger_reset(char i2c);
void logger_time_rec(char i2c);
//Useful function to optimize code

short get_total_records(char i2c);
void get_time_str(char i2c,short record_number,char * time);
void get_date_str(char i2c,short record_number,char * date);


/// Addon for OLED library
void logger_oled_tot_rec(char i2c,char * info_msg);
void logger_oled_rec_progress(char i2c,short record_number);
void logger_oled_display_time(char i2c,short record_number);
void logger_oled_display_date(char i2c,short record_number);
