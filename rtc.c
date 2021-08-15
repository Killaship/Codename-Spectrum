#include "rtc.h"

const unsigned int RTC_ADRESS = 0x70;
const unsigned int RTC_DATA = 0x71;

void initRTC() {
	kprint("RTC: Starting initialization...", 0x0E);

	write_port(RTC_ADRESS, 10);
	char data = read_port(RTC_DATA);
	
	if (data == 0x80) {
		printLine("RTC: Update running. Retrying...", 0x04);
		initRTC();
	} else {
		kprint("RTC: No update running. Continuing...", 0x02);
	}
	
	kprint("RTC: Initialized!", 0x02);
}

int getSeconds() {
	write_port(0x70, 0);
	return convertBCD(read_port(0x71));
}

int getMinutes() {
	write_port(0x70, 0x02);
	return convertBCD(read_port(0x71));
}

int getHours() {
	write_port(0x70, 0x04);
	return convertBCD(read_port(0x71));
}

int getDay() {
	write_port(0x70, 0x07);
	return convertBCD(read_port(0x71));
}

int getMonth() {
	write_port(0x70, 0x08);
	return convertBCD(read_port(0x71));
}

int getYear() {
	write_port(0x70, 0x09);
	return convertBCD(read_port(0x71));
}
