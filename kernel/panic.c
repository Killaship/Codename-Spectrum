#include "kernel.h"

void hang() {
		asm volatile(
          "1:\n\t"
          "cli\n\t"
          "hlt\n\t"
          "jmp 1b\n\t"
          );	
}

void div0_handler() {
	kprint_newline();
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x44;
	}
	kprint("err: kernel panic!",0x40);
	kprint_newline();
	kprint("err type:",0x40);
	kprint_newline();
	kprint("fault: divide-by-zero (0x00)",0x40);
	kprint_newline();
	kprint_newline();
	hang();
}

void debg_handler() {
	kprint_newline();
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x44;
	}
	kprint("err: kernel panic!",0x40);
	kprint_newline();
	kprint("err type:",0x40);
	kprint_newline();
	kprint("fault/trap: debug (0x01)",0x40);
	kprint_newline();
	kprint_newline();
	hang();
}

void nmi_handler() {
	kprint_newline();
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x44;
	}
	kprint("err: kernel panic!",0x40);
	kprint_newline();
	kprint("err type:",0x40);
	kprint_newline();
	kprint("fault: non-maskable interrupt (0x02)",0x40);
	kprint_newline();
	kprint_newline();
	hang();
}

void overf_handler() {
	kprint_newline();
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x44;
	}
	kprint("err: kernel panic!",0x40);
	kprint_newline();
	kprint("err type:",0x40);
	kprint_newline();
	kprint("trap: overflow detected (0x04)",0x40);
	kprint_newline();
	kprint_newline();
	hang();
}


void boundrx_handler() {
	kprint_newline();
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x44;
	}
	kprint("err: kernel panic!",0x40);
	kprint_newline();
	kprint("err type:",0x40);
	kprint_newline();
	kprint("fault: bound range exceeded (0x05)",0x40);
	kprint_newline();
	kprint_newline();
	hang();
}

void dfault_handler() {
	kprint_newline();
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x44;
	}
	kprint("err: kernel panic!",0x40);
	kprint_newline();
	kprint("err type:",0x40);
	kprint_newline();
	kprint("abort: double fault (0x08)",0x40);
	kprint_newline();
	kprint_newline();
	kprint_newline();
	kprint_newline();
	kprint("This is a pretty rare error, how'd you manage to screw up the kernel this much?!",0x40);
	hang();
}


