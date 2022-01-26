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


char last_char;

// todo: massive reform of driver

void keyboard_handler_main(void)
{	
	unsigned char status;
	char keycode;

	// write EOI 
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	// Lowest bit of status will be set if buffer is not empty 
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;

		if(keycode == ENTER_KEY_CODE) {
			kprint_newline();
			//cmdflush();
			input_prompt();
			return;
		}
		
		if(keycode == 0x0E) {
			backspace();
			return;
		}		
		return keycode;
		//last_char = keyboard_map[(unsigned char) keycode];
		vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
		vidptr[current_loc++] = 0x07;
	}
	
}






