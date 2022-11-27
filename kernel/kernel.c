/*
* Parts of this was made by Arjun Sreedharan, all credit where due.
* License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
*/
#define my_sizeof(type) (char *)(&type+1)-(char*)(&type)
#include <stdint.h>
#include <stddef.h>
#include "common.h"
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define osversion "Codename Spectrum v0.6.2"


extern void keyboard_handler(void);

#include "keyboard_map.h"
#include "tty.h"

#include "panic.h"
#include "cpu.h"
#include "rtc.h"
#include "mem.h"

void printcpu() {
	kprint("Vendor ID: ", 0x07);
	kprint(cpu_string(), 0x0C);
}

void printtime() {
	asm volatile ("cli");
    	read_rtc();
    	asm volatile ("sti");
	
	kprint("System Time: ",0x07);
	kprint(itoa(hour),0x07);
	kprint(":",0x07);
	kprint(itoa(minute),0x07);
	kprint(":",0x07);
	kprint(itoa(second),0x07);
}

#include "kb.h"


void kmain(void) {
	clear_screen();
	idt_init();
	kb_init();
	sh_init();
	while(1);
}
