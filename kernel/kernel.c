/*
* Parts of this was made by Arjun Sreedharan, all credit where due.
* License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
*/
#define my_sizeof(type) (char *)(&type+1)-(char*)(&type)
// (C) Killaship 2022, lol

#include "multiboot.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>
extern void keyboard_handler(void);
#include "keyboard_map.h"
#include "common.h"
#include "ports.h"
#include "tty.h"
#include "panic.h"
#include "memory.h"
#include "cpu.h"
#include "rtc.h"
#include "kb.h"
#include "idt.h"
#define osversion "Codename Spectrum v0.6.2"


extern void keyboard_handler(void);
#include "keyboard_map.h"
#include "kb.h"




void kmain(void) {
	clear_screen();
	idt_init();
	kprint("IDT init\n", 0x07);
	timer_phase(100);
	kprint("PIT init\n", 0x07);
	kb_init();
	kprint("KB init\n", 0x07);
	sh_init();
	while(1);
}
