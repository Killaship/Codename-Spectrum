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

	/* write EOI */
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
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
		//last_char = keyboard_map[(unsigned char) keycode];
		vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
		vidptr[current_loc++] = 0x07;
	}
}






string readStr()
{
    char buff;
    string buffstr;
    uint8 i = 0;
    uint8 reading = 1;
    while(reading)
    {
        if(read_port(0x64) & 0x1)                 
        {
            switch(read_port(0x60))
            { 
      /*case 1:
                kprint('(char)27);           Escape button
                buffstr[i] = (char)27;
                i++;
                break;*/
        case 2:
                kprint('1',0x07);
                buffstr[i] = '1';
                i++;
                break;
        case 3:
                kprint('2',0x07);
                buffstr[i] = '2';
                i++;
                break;
        case 4:
                kprint('3',0x07);
                buffstr[i] = '3';
                i++;
                break;
        case 5:
                kprint('4',0x07);
                buffstr[i] = '4';
                i++;
                break;
        case 6:
                kprint('5',0x07);
                buffstr[i] = '5';
                i++;
                break;
        case 7:
                kprint('6',0x07);
                buffstr[i] = '6';
                i++;
                break;
        case 8:
                kprint('7',0x07);
                buffstr[i] = '7';
                i++;
                break;
        case 9:
                kprint('8');
                buffstr[i] = '8';
                i++;
                break;
        case 10:
                kprint('9');
                buffstr[i] = '9';
                i++;
                break;
        case 11:
                kprint('0');
                buffstr[i] = '0';
                i++;
                break;
        case 12:
                kprint('-');
                buffstr[i] = '-';
                i++;
                break;
        case 13:
                kprint('=');
                buffstr[i] = '=';
                i++;
                break;
        case 14:
                backspace();
                i--;
                buffstr[i] = 0;
                break;
       /* case 15:
                printch('\t');          Tab button
                buffstr[i] = '\t';
                i++;
                break;*/
        case 16:
                kprint('q');
                buffstr[i] = 'q';
                i++;
                break;
        case 17:
                kprint('w');
                buffstr[i] = 'w';
                i++;
                break;
        case 18:
                kprint('e');
                buffstr[i] = 'e';
                i++;
                break;
        case 19:
                kprint('r');
                buffstr[i] = 'r';
                i++;
                break;
        case 20:
                kprint('t');
                buffstr[i] = 't';
                i++;
                break;
        case 21:
                kprint('y');
                buffstr[i] = 'y';
                i++;
                break;
        case 22:
                kprint('u');
                buffstr[i] = 'u';
                i++;
                break;
        case 23:
                kprint('i');
                buffstr[i] = 'i';
                i++;
                break;
        case 24:
                kprint('o');
                buffstr[i] = 'o';
                i++;
                break;
        case 25:
                kprint('p');
                buffstr[i] = 'p';
                i++;
                break;
        case 26:
                kprint('[');
                buffstr[i] = '[';
                i++;
                break;
        case 27:
                kprint(']');
                buffstr[i] = ']';
                i++;
                break;
        case 28:
               // printch('\n');
               // buffstr[i] = '\n';
                  i++;
               reading = 0;
                break;
      /*  case 29:
                printch('q');           Left Control
                buffstr[i] = 'q';
                i++;
                break;*/
        case 30:
                kprint('a');
                buffstr[i] = 'a';
                i++;
                break;
        case 31:
                kprint('s');
                buffstr[i] = 's';
                i++;
                break;
        case 32:
                kprint('d');
                buffstr[i] = 'd';
                i++;
                break;
        case 33:
                kprint('f');
                buffstr[i] = 'f';
                i++;
                break;
        case 34:
                kprint('g');
                buffstr[i] = 'g';
                i++;
                break;
        case 35:
                kprint('h');
                buffstr[i] = 'h';
                i++;
                break;
        case 36:
                kprint('j');
                buffstr[i] = 'j';
                i++;
                break;
        case 37:
                kprint('k');
                buffstr[i] = 'k';
                i++;
                break;
        case 38:
                kprint('l');
                buffstr[i] = 'l';
                i++;
                break;
        case 39:
                kprint(';');
                buffstr[i] = ';';
                i++;
                break;
        case 40:
                kprint((char)44);               //   Single quote (')
                buffstr[i] = (char)44;
                i++;
                break;
        case 41:
                kprint((char)44);               // Back tick (`)
                buffstr[i] = (char)44;
                i++;
                break;
     /* case 42:                                 Left shift 
                printch('q');
                buffstr[i] = 'q';
                i++;
                break;
        case 43:                                 \ (< for somekeyboards)   
                printch((char)92);
                buffstr[i] = 'q';
                i++;
                break;*/
        case 44:
                kprint('z');
                buffstr[i] = 'z';
                i++;
                break;
        case 45:
                kprint('x');
                buffstr[i] = 'x';
                i++;
                break;
        case 46:
                kprint('c');
                buffstr[i] = 'c';
                i++;
                break;
        case 47:
                kprint('v');
                buffstr[i] = 'v';
                i++;
                break;                
        case 48:
                kprint('b');
                buffstr[i] = 'b';
                i++;
                break;               
        case 49:
                kprint('n');
                buffstr[i] = 'n';
                i++;
                break;                
        case 50:
                kprint('m');
                buffstr[i] = 'm';
                i++;
                break;               
        case 51:
                kprint(',');
                buffstr[i] = ',';
                i++;
                break;                
        case 52:
                kprint('.');
                buffstr[i] = '.';
                i++;
                break;            
        case 53:
                kprint('/');
                buffstr[i] = '/';
                i++;
                break;            
        case 54:
                kprint('.');
                buffstr[i] = '.';
                i++;
                break;            
        case 55:
               kprint('/');
                buffstr[i] = '/';
                i++;
                break;            
      /*case 56:
                printch(' ');           Right shift
                buffstr[i] = ' ';
                i++;
                break;*/           
        case 57:
                kprint(' ');
                buffstr[i] = ' ';
                i++;
                break;
            }
        }
    }
    buffstr[i] = 0;                   
    return buffstr;
}
