#define ENTER_KEY_CODE 0x1C
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void kb_init(void)
{
	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	write_port(0x21 , 0xFD);

}


void backspace() {
	vidptr[current_loc - 2] = 0x0000;
	current_loc = current_loc - 2;	
}






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


uint8_t scancode;
unsigned char kbstatus;
char c[2];
char buffer[64];

// I can't seem to get this thing to return a string properly so I'm just gonna make a shell a part of the kb driver.

void keyboard_handler_main(void) {
	int i;
	
	write_port(0x20, 0x20); //eoi
	kbstatus = read_port(KEYBOARD_STATUS_PORT);
	// Lowest bit of status will be set if buffer is not empty 
	if (kbstatus & 0x01) {

		scancode = read_port(0x60);
		if (scancode & 0x80)
		{
    			// Released
		}
		else
		{
    		// Pressed
		
		if(keyboard_map[(char)scancode] == '\n') { 

			buffer[64] = 0;
			if(buffer[0] == ';' && buffer[1] == ';') {
				// Literally nothing, just move on, it's a comment
			}
			else if(strcmp("time", buffer) == 0) {
				kprint_newline();
				printtime();
			}
			else if(strcmp("help", buffer) == 0) {
				kprint_newline();
				kprint("Prism Shell for ", 0x07);
				kprint(osversion, 0x08);
				kprint_newline();
				kprint("Prepend \";;\" to a command for it to be a comment!",0x09);
				kprint_newline();
				kprint("COMMANDS:",0x07);
				kprint_newline();
				kprint("help   --   Displays this help message.", 0x07);
				kprint_newline();
				kprint("time   --   Displays the system RTC time.", 0x07);
				kprint_newline();
				kprint("cls    --   Clears the screen.", 0x07);
				kprint_newline();
				kprint("cpuid  --   Displays the CPU info.", 0x07);
				kprint_newline();
				kprint("bf     --   A BrainF**k interpreter.", 0x07);
				kprint_newline();
				kprint_newline();
				kprint("https://github.com/Killaship/Codename-Spectrum/", 0x04);
			}
			else if(strcmp("cpuid", buffer) == 0) {
				kprint_newline();
				printcpu();
			}
			else if(strcmp("bf", buffer) == 0) {
				clear_screen();
				
			}			
			else if(strcmp("test", buffer) == 0) {
				kprint_newline();
				while(1) {}
				loadprgm(); // test on loading programs, will not come soon, currently halts cpu
			}			
			else if(strcmp("cls", buffer) == 0) {
				clear_screen();
				kprint("Prism Shell [Kernelspace]", 0x07);
				kprint_newline();
				kprint("Type \"help\" for a list of commands.", 0x07);
				kprint_newline();
				input_prompt();
			}
			else {
				if(strcmp("", buffer) != 0) {
				kprint_newline();
				kprint("Bad command!", 0x07);
				}
				else{
					
				}
			}
			memset(buffer, 0, 64);
			i = 0;
			kprint_newline();
			input_prompt();
	
			}
		else if(keyboard_map[(char)scancode] == '\b') {
			backspace();
			buffer[i] = 0;
			i--;
			buffer[i] = 0;
		}
		else {
		
		c[0] = keyboard_map[scancode];
		c[1] = 0;
		buffer[i] = c[0];
		i++;
    		kprint(c,0x07);

		
		
			
		}
		}
		
	}
	

}





