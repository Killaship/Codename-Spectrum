#pragma once



static inline int cpuid_string(int code, int where[4]) {
  __asm__ volatile ("cpuid":"=a"(*where),"=b"(*(where+0)),
               "=d"(*(where+1)),"=c"(*(where+2)):"a"(code));
  return (int)where[0];
}
 
const char * const cpu_string() {
	static char s[16] = "BogusProces!";
	cpuid_string(0, (int*)(s));
	return s;
}

void reboot()
{
    uint8_t good = 0x02;
    while (good & 0x02)
        good = read_port(0x64);
    write_port(0x64, 0xFE);
    halt();
}


