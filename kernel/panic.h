#pragma once

/*
bsodmsg() feeds the error code "420" so the actual bsod function knows that it should ignore other things and just print the custom message
bsod() takes normal stop codes, so the actual thing just ignores the string
and hopefully, when you bsod(420), it prints out the " "
maybe I should change that to an string saying why there isn't an error code
although that should never happen since bsod() is meant to be a kernel-only function, only to be called by error handlers or if there's a serious issue
unless someone gets their hands on the source code and does funny stuff with bsod'ing in the kernel
which is bad
and then builds the kernel and distributes it and I get hit with a bunch of people saying it's broken
which is bad
so maybe I should make it so that the kernel has to be signed by me and there's an early boot program that checks if the kernel is properly signed 
and whoops I just reinvented secure boot (I think)
*/
#define BSOD_COLOR 0x4F
// White on blue: 0x1F (windows)
// Black on red: 0x40 (spectrum)
// Black on green: 0x20 (I dunno)
#define bsod(stopcode) bsod1(" ", stopcode)
#define bsodmsg(str) bsod1(str, 420)
struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};
void stacktrace(unsigned int maxframes) { // Stack Tracer
	char* addr[16];
	struct stackframe *stk;
	asm volatile("movl %%ebp,%0" : "=r"(stk) ::);
	kprint("Stack trace:\n", BSOD_COLOR);
	for(unsigned int frame = 0; stk && frame < maxframes; ++frame) {
        	// Unwind to previous stack frame
		prntnum(stk->eip,16,' ',addr);
		kprint("0x", BSOD_COLOR);
        	kprint(addr, BSOD_COLOR);
		kprint_newline();
        	stk = stk->ebp;
	}
}

void bsod1(const char *str, const int stopcode) { // BSOD Function
	
	write_port(0x3D4, 0x0A); // Disable VGA cursor
	write_port(0x3D5, 0x20);
	
	current_loc = 0; // Paint screen blue
  	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = BSOD_COLOR;
	}
	kprint("A problem has been detected and YAOS has been shut down to prevent damage to" , BSOD_COLOR); // text and technical info
  	kprint_newline();
  	kprint("your computer.", BSOD_COLOR);
	kprint_newline();
	kprint_newline();
	kprint("If this is the first time you've seen this stop error screen, restart" , BSOD_COLOR); 
	kprint_newline();
	kprint("your computer. If this screen appears again, follow these steps:" , BSOD_COLOR);
	kprint_newline();
	kprint_newline();
	kprint("Check to make sure any new hardware or software is properly installed.", BSOD_COLOR);
	kprint_newline();
	kprint("If this is a new install, check your software/hardware vendor for", BSOD_COLOR);
	kprint_newline();
	kprint("any drivers or OS updates you may need.", BSOD_COLOR);
	kprint_newline();
	kprint("If problems continue, remove any newly installed hardware or software.", BSOD_COLOR);
	kprint_newline();
	kprint("Disable BIOS memory options such as caching or shadowing.", BSOD_COLOR);
	kprint_newline();
	kprint_newline();
	kprint("Finally, if all else fails, go to https://github.com/Killaship/yaos/issues", BSOD_COLOR);
	kprint_newline();
	kprint("and submit an issue with the below information.", BSOD_COLOR);
	kprint_newline();
	kprint_newline();
	switch(stopcode) {
		case 0: // #DE
			kprint("Error type: #DE", BSOD_COLOR);
			break;
		case 1: // #DB
			kprint("Error type: #DB", BSOD_COLOR);
			break;
		case 2: // NMI
			kprint("Error type: NMI", BSOD_COLOR);
			break;
		case 3: // #BP
			kprint("Error type: #BP", BSOD_COLOR);
			break;
		case 4: // #OF
			kprint("Error type: #OF", BSOD_COLOR);
			break;
		case 5: // #BR
			kprint("Error type: #BR", BSOD_COLOR);
			break;
		case 6: // #UD
			kprint("Error type: #UD", BSOD_COLOR);
			break;
		case 7: // #NM
			kprint("Error type: #NM", BSOD_COLOR);
			break;
		case 8: // #DF
			kprint("Error type: #DF", BSOD_COLOR);
			break;
		case 9: // CSO
			kprint("Error type: CSO", BSOD_COLOR);
			break;
		case 10: // #TS
			kprint("Error type: #TS", BSOD_COLOR);
			break;
		case 11: // #NP
			kprint("Error type: #NP", BSOD_COLOR);
			break;
		case 12: // #SS
			kprint("Error type: #SS", BSOD_COLOR);
			break;
		case 13: // #GP
			kprint("Error type: #GP", BSOD_COLOR);
			break;
		case 14: // #PF
			kprint("Error type: #PF", BSOD_COLOR);
			break;
		case 15: // Intel Reserved
			kprint("Error type: INTELRSVD", BSOD_COLOR);
			break;
		case 16: // #MF
			kprint("Error type: #MF", BSOD_COLOR);
			break;
		case 17: // #AC
			kprint("Error type: #AC", BSOD_COLOR);
			break;
		case 18: // #MC
			kprint("Error type: #MC", BSOD_COLOR);
			break;
		case 19: // #XF
			kprint("Error type: #XF", BSOD_COLOR);
			break;
		case 20: // #VE
			kprint("Error type: #VE", BSOD_COLOR);
			break;
		case 21: // #CP
			kprint("Error type: #CP", BSOD_COLOR);
			break;
		case 22: // #HV
			kprint("Error type: #HV", BSOD_COLOR);
			break;
		case 23: // #VC
			kprint("Error type: #VC", BSOD_COLOR);
			break;
		case 24: // #SX
			kprint("Error type: #SX", BSOD_COLOR);
			break;
		case 69:
			kprint("Stack Smashing detected!", BSOD_COLOR);
			break;
		case 420:
			kprint("Reason: ", BSOD_COLOR);
			kprint(str, BSOD_COLOR);
			break;
	}
	kprint_newline();
	stacktrace(12);
	
	
}


void err_handler() {
	// Say there's been an error, and then hang.
	bsod(0);
	asm volatile ("cli");
	while(1);
}

void err1_handler() {
	// Say there's been an error, and then hang.
	bsod(1);
	asm volatile ("cli");
	while(1);
}

void err2_handler() {
	// Say there's been an error, and then hang.
	bsod(2);
	asm volatile ("cli");
	while(1);
}

void err3_handler() {
	// Say there's been an error, and then hang.
	bsod(3);
	asm volatile ("cli");
	while(1);
}

void err4_handler() {
	// Say there's been an error, and then hang.
	bsod(4);
	asm volatile ("cli");
	while(1);
}

void err5_handler() {
	// Say there's been an error, and then hang.
	bsod(5);
	asm volatile ("cli");
	while(1);
}

void err6_handler() {
	// Say there's been an error, and then hang.
	bsod(6);
	asm volatile ("cli");
	while(1);
}

void err7_handler() {
	// Say there's been an error, and then hang.
	bsod(7);
	asm volatile ("cli");
	while(1);
}

void err8_handler() {
	// Say there's been an error, and then hang.
	bsod(8);
	asm volatile ("cli");
	while(1);
}

void err9_handler() {
	// Say there's been an error, and then hang.
	bsod(9);
	asm volatile ("cli");
	while(1);
}

void err10_handler() {
	// Say there's been an error, and then hang.
	bsod(10);
	asm volatile ("cli");
	while(1);
}

void err11_handler() {
	// Say there's been an error, and then hang.
	bsod(11);
	asm volatile ("cli");
	while(1);
}

void err12_handler() {
	// Say there's been an error, and then hang.
	bsod(12);
	asm volatile ("cli");
	while(1);
}

void err13_handler() {
	// Say there's been an error, and then hang.
	bsod(13);
	asm volatile ("cli");
	while(1);
}

void err14_handler() {
	// Say there's been an error, and then hang.
	bsod(14);
	asm volatile ("cli");
	while(1);
}

void err15_handler() {
	// Say there's been an error, and then hang.
	bsod(15);
	asm volatile ("cli");
	while(1);
}

void err16_handler() {
	// Say there's been an error, and then hang.
	bsod(16);
	asm volatile ("cli");
	while(1);
}

void err17_handler() {
	// Say there's been an error, and then hang.
	bsod(17);
	asm volatile ("cli");
	while(1);
}

void err18_handler() {
	// Say there's been an error, and then hang.
	bsod(18);
	asm volatile ("cli");
	while(1);
}

void err19_handler() {
	// Say there's been an error, and then hang.
	bsod(19);
	asm volatile ("cli");
	while(1);
}

void err20_handler() {
	// Say there's been an error, and then hang.
	bsod(20);
	asm volatile ("cli");
	while(1);
}

void err21_handler() {
	// Say there's been an error, and then hang.
	bsod(21);
	asm volatile ("cli");
	while(1);
}
void err22_handler() {
	// Say there's been an error, and then hang.
	bsod(22);
	asm volatile ("cli");
	while(1);
}
void err23_handler() {
	// Say there's been an error, and then hang.
	bsod(23);
	asm volatile ("cli");
	while(1);
}
void err24_handler() {
	// Say there's been an error, and then hang.
	bsod(24);
	asm volatile ("cli");
	while(1);
}

typedef void (*f)(void);                 //declare typdef

// uuuugh shitty code
f idtfunc[32] = {&err_handler, &err1_handler, &err2_handler, &err3_handler, &err4_handler, &err5_handler, &err6_handler, &err7_handler,&err8_handler,&err9_handler,&err10_handler,&err11_handler,&err12_handler,&err13_handler,&err14_handler,&err15_handler,&err16_handler,&err17_handler,&err18_handler,&err19_handler,&err20_handler,&err21_handler,&err15_handler,&err15_handler,&err15_handler,&err15_handler,&err22_handler,&err23_handler,&err24_handler,&err15_handler,};      
//make array func of type f,
//the pointer to a function
