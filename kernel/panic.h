#pragma once

void div0_handler() {
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
	asm volatile(
          "1:\n\t"
          "cli\n\t"
          "hlt\n\t"
          "jmp 1b\n\t"
          );	
}

void boundrx_handler() {
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
	asm volatile(
          "1:\n\t"
          "cli\n\t"
          "hlt\n\t"
          "jmp 1b\n\t"
          );	
}

void overf_handler() {
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
	asm volatile(
          "1:\n\t"
          "cli\n\t"
          "hlt\n\t"
          "jmp 1b\n\t"
          );	
}
