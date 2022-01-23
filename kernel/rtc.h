#pragma once
#define CURRENT_YEAR        2022                            // Change this each year!
 
int century_register = 0x00;                                // Set by ACPI table parsing code if possible
 
unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned int year;

 
enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

int get_update_in_progress_flag();
unsigned char get_RTC_register(int reg);
void read_rtc();
