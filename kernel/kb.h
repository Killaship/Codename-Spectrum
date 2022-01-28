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








uint8_t scancode;
unsigned char kbstatus;
char c[2];

void keyboard_handler_main(void) {
	int i;
	char buff[64];
	

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
			kprint_newline(); 
			
			buff[64] = 0;
			kprint(buff,0x07);
			memset(buff, 0, 64);
			i = 0;
			kprint_newline();
			input_prompt();
			}
		else if(keyboard_map[(char)scancode] == '\b') {
			backspace();
			buff[i] = 0;
			i--;
			buff[i] = 0;
		}
		else {
		
		c[0] = keyboard_map[scancode];
		c[1] = 0;
		buff[i] = c[0];
		i++;
    		kprint(c,0x07);
		
			
		}
		}
		
	}
	

}


char getchar() {
	return c[0];
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
