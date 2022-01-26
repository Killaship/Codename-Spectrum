#pragma once

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
	kprint("fault: divide-by-zero (0x00) #DE",0x40);
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
	kprint("fault/trap: debug (0x01) #DB",0x40);
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
	kprint("trap: overflow detected (0x04) #OF",0x40);
	kprint_newline();
	kprint_newline();
	hang();
}

void badop_handler() {
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
	kprint("fault: invalid opcode (0x06) #UD",0x40);
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
	kprint("fault: bound range exceeded (0x05) #BR",0x40);
	kprint_newline();
	kprint_newline();
	hang();
}

void ss_handler() {
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
	kprint("fault: stack segment fault (0x0C) #SS",0x40);
	kprint_newline();
	kprint_newline();
	hang();
}

void gp_handler() {
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
	kprint("fault: GP fault (0x0D) #GP",0x40);
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
	kprint("abort: double fault (0x08) #DF",0x40);
	kprint_newline();
	kprint_newline();
	kprint_newline();
	kprint_newline();
	kprint("This is a pretty rare error, how'd you manage to screw up the kernel this much?!",0x40);
	hang();
}



void badtss_handler() {
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
	kprint("fault: bad tss #TS (0x0A)",0x40);
	kprint_newline();
	kprint_newline();
	hang();
}


void reserved_handler() {
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
	kprint("RESERVED (I don't know what it's for either)",0x40);
	kprint_newline();
	kprint_newline();
	hang();
}
