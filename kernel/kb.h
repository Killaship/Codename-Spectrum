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


char* last_char;


char readStr()
{
    char buff;
    char* buffstr[64];
    uint8_t i = 0;
    uint8_t reading = 1;
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
                kprint('8',0x07);
                buffstr[i] = '8';
                i++;
                break;
        case 10:
                kprint('9',0x07);
                buffstr[i] = '9';
                i++;
                break;
        case 11:
                kprint('0',0x07);
                buffstr[i] = '0';
                i++;
                break;
        case 12:
                kprint('-',0x07);
                buffstr[i] = '-';
                i++;
                break;
        case 13:
                kprint('=',0x07);
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
                kprint('q',0x07);
                buffstr[i] = 'q';
                i++;
                break;
        case 17:
                kprint('w',0x07);
                buffstr[i] = 'w';
                i++;
                break;
        case 18:
                kprint('e',0x07);
                buffstr[i] = 'e';
                i++;
                break;
        case 19:
                kprint('r',0x07);
                buffstr[i] = 'r';
                i++;
                break;
        case 20:
                kprint('t',0x07);
                buffstr[i] = 't';
                i++;
                break;
        case 21:
                kprint('y',0x07);
                buffstr[i] = 'y';
                i++;
                break;
        case 22:
                kprint('u',0x07);
                buffstr[i] = 'u';
                i++;
                break;
        case 23:
                kprint('i',0x07);
                buffstr[i] = 'i';
                i++;
                break;
        case 24:
                kprint('o',0x07);
                buffstr[i] = 'o';
                i++;
                break;
        case 25:
                kprint('p',0x07);
                buffstr[i] = 'p';
                i++;
                break;
        case 26:
                kprint('[',0x07);
                buffstr[i] = '[';
                i++;
                break;
        case 27:
                kprint(']',0x07);
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
                kprint('a',0x07);
                buffstr[i] = 'a';
                i++;
                break;
        case 31:
                kprint('s',0x07);
                buffstr[i] = 's';
                i++;
                break;
        case 32:
                kprint('d',0x07);
                buffstr[i] = 'd';
                i++;
                break;
        case 33:
                kprint('f',0x07);
                buffstr[i] = 'f';
                i++;
                break;
        case 34:
                kprint('g',0x07);
                buffstr[i] = 'g';
                i++;
                break;
        case 35:
                kprint('h',0x07);
                buffstr[i] = 'h';
                i++;
                break;
        case 36:
                kprint('j',0x07);
                buffstr[i] = 'j';
                i++;
                break;
        case 37:
                kprint('k',0x07);
                buffstr[i] = 'k';
                i++;
                break;
        case 38:
                kprint('l',0x07);
                buffstr[i] = 'l';
                i++;
                break;
        case 39:
                kprint(';',0x07);
                buffstr[i] = ';';
                i++;
                break;
        case 40:
                kprint((char)44,0x07);               //   Single quote (')
                buffstr[i] = (char)44;
                i++;
                break;
        case 41:
                kprint((char)44,0x07);               // Back tick (`)
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
                kprint('z',0x07);
                buffstr[i] = 'z';
                i++;
                break;
        case 45:
                kprint('x',0x07);
                buffstr[i] = 'x';
                i++;
                break;
        case 46:
                kprint('c',0x07);
                buffstr[i] = 'c';
                i++;
                break;
        case 47:
                kprint('v',0x07);
                buffstr[i] = 'v';
                i++;
                break;                
        case 48:
                kprint('b',0x07);
                buffstr[i] = 'b';
                i++;
                break;               
        case 49:
                kprint('n',0x07);
                buffstr[i] = 'n';
                i++;
                break;                
        case 50:
                kprint('m',0x07);
                buffstr[i] = 'm';
                i++;
                break;               
        case 51:
                kprint(',',0x07);
                buffstr[i] = ',';
                i++;
                break;                
        case 52:
                kprint('.',0x07);
                buffstr[i] = '.';
                i++;
                break;            
        case 53:
                kprint('/',0x07);
                buffstr[i] = '/';
                i++;
                break;            
        case 54:
                kprint('.',0x07);
                buffstr[i] = '.';
                i++;
                break;            
        case 55:
               kprint('/',0x07);
                buffstr[i] = '/';
                i++;
                break;            
      /*case 56:
                printch(' ');           Right shift
                buffstr[i] = ' ';
                i++;
                break;*/           
        case 57:
                kprint(' ',0x07);
                buffstr[i] = ' ';
                i++;
                break;
            }
        }
    }
    buffstr[i] = 0;                   
    return buffstr;
}







// todo: massive reform of driver
unsigned char kbstatus;
void keyboard_handler_main(void)
{readStr();
 write_port(0x20, 0x20);}
	
/*
	char keycode;

	// write EOI 
	

	kbstatus = read_port(KEYBOARD_STATUS_PORT);
	// Lowest bit of status will be set if buffer is not empty 
	if (kbstatus & 0x01) {
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
		
		last_char = keyboard_map[(unsigned char) keycode];
		vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
		last_char = vidptr[current_loc];
		vidptr[current_loc++] = 0x07;
		kprint((char) last_char,0x09);
		
	}
	
	
}
*/

