#pragma once

void input_prompt() {
	kprint("Spectrum@fakeshell: ", 0x09);
}



void shell() {

	
	kprint("Prism Shell [Kernelspace]", 0x07);
	kprint_newline();
	kprint("Type \"help\" for a list of commands.", 0x07);
	kprint_newline();
	kprint_newline();
	input_prompt();
	
	while(1) {	

	}
}

void sh_init() {
	kprint_newline();
	kprint_newline();
	shell();
}
