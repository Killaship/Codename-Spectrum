#pragma once

void input_prompt() {
	kprint("Spectrum@fakeshell: ", 0x09);
}



void shell() {
	char oldchar = 0;
	char newchar;
	
	kprint("Prism Shell [Kernelspace]", 0x07);
	kprint_newline();
	kprint("Type \"help\" for a list of commands.", 0x07);
	kprint_newline();
	kprint_newline();
	input_prompt();
	
	while(1) {	
		newchar = getchar();
		if(newchar != oldchar) {
			putc(newchar, 0x07);
		}
		else {
			return;
		}
	}
}

void sh_init() {
	kprint_newline();
	kprint_newline();
	shell();
}
