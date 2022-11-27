#pragma once

int firstrun = 1;

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


void timer_phase(int hz) {
	asm("sti");
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    write_port(0x43, 0x36);             /* Set our command byte 0x36 */
    write_port(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    write_port(0x40, divisor >> 8);     /* Set high byte of divisor */
}

/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks = 0;

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
void timer_handler() {
	write_port(0x20, 0x20);
    /* Increment our 'tick count' */
    timer_ticks++;
	
    if (timer_ticks % 100 == 0)
    {
     // kprint("One second has passed\n", 0x07);
    }
	
	timer_phase(100);
}


void reboot() {
    uint8_t good = 0x02;
    while (good & 0x02)
        good = read_port(0x64);
    write_port(0x64, 0xFE);
}


void printcpu() {
	kprint("Vendor ID: ", 0x07);
	kprint(cpu_string(), 0x0C);
}


