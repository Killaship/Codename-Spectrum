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
unsigned char kbstatus;
void keyboard_handler_main(void) {
	

	write_port(0x20, 0x20); //eoi

	char keycode;
	char buff[2];
	

	kbstatus = read_port(KEYBOARD_STATUS_PORT);
	// Lowest bit of status will be set if buffer is not empty 
	if (kbstatus & 0x01) {

		uint8_t scancode = read_port(0x60);
		if (scancode & 0x80)
		{
    			// Released
		}
		else
		{
    		// Pressed
    		kprint(keyboard_map[scancode],0x07);
    		// Or for string
   		 char c[2] = { keyboard_map[scancode], 0 };
    		kprint(c,0x07);
		}
		
	}
	

}
/*

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
		buff[0] = keyboard_map[(unsigned char) keycode];
		buff[1] = 0;
		kprint(buff,0x07);
		kprint(buff, 0x09);
*/
