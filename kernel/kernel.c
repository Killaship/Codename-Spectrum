/*
* Parts of this was made by Arjun Sreedharan, all credit where due.
* License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
*/
#include "drivers/keyboard_map.h"
#include "drivers/utils.h"


#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define ENTER_KEY_CODE 0x1C


extern void boundrx_handler(void);
extern void div0_handler(void);
extern void overf_handler(void);
extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
extern void disable_ints(void);
extern void enable_ints(void);



/* current cursor location */
unsigned int current_loc = 0;
/* video memory begins at address 0xb8000 */
char *vidptr = (char*)0xb8000;

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];
/* TODO: finish idt entry generator and replace all entries with it
void gen_idt(int idtnum, int *type, int handler, int addr) {
	handler
}
*/
void idt_init(void)
{	unsigned long div0_address;
 	unsigned long boundrx_address;
	unsigned long overf_address;
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
	
	
	
	
	div0_address = (unsigned long)div0_handler;
	IDT[0x00].offset_lowerbits = div0_address & 0xffff;
	IDT[0x00].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x00].zero = 0;
	IDT[0x00].type_attr = INTERRUPT_GATE;
	IDT[0x00].offset_higherbits = (div0_address & 0xffff0000) >> 16;
 
 	div0_address = (unsigned long)div0_handler;
	IDT[0x01].offset_lowerbits = div0_address & 0xffff;
	IDT[0x01].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x01].zero = 0;
	IDT[0x01].type_attr = INTERRUPT_GATE;
	IDT[0x01].offset_higherbits = (div0_address & 0xffff0000) >> 16;
 
 	div0_address = (unsigned long)div0_handler;
	IDT[0x02].offset_lowerbits = div0_address & 0xffff;
	IDT[0x02].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x02].zero = 0;
	IDT[0x02].type_attr = INTERRUPT_GATE;
	IDT[0x02].offset_higherbits = (div0_address & 0xffff0000) >> 16;
 
   	overf_address = (unsigned long)overf_handler;
	IDT[0x04].offset_lowerbits = overf_address & 0xffff;
	IDT[0x04].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x04].zero = 0;
	IDT[0x04].type_attr = INTERRUPT_GATE;
	IDT[0x04].offset_higherbits = (overf_address & 0xffff0000) >> 16;
 
 	boundrx_address = (unsigned long)boundrx_handler;
	IDT[0x05].offset_lowerbits = boundrx_address & 0xffff;
	IDT[0x05].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x05].zero = 0;
	IDT[0x05].type_attr = INTERRUPT_GATE;
	IDT[0x05].offset_higherbits = (boundrx_address & 0xffff0000) >> 16;
 
  	overf_address = (unsigned long)overf_handler;
	IDT[0x08].offset_lowerbits = overf_address & 0xffff;
	IDT[0x08].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x08].zero = 0;
	IDT[0x08].type_attr = INTERRUPT_GATE;
	IDT[0x08].offset_higherbits = (overf_address & 0xffff0000) >> 16;

 
 
	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);
	write_port(0xA1 , 0x00);

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}


void kb_init(void)
{
	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	write_port(0x21 , 0xFD);
}

void kprint(const char *str, const int color) {
	
	unsigned int i = 0;
	while (str[i] != '\0') {
		vidptr[current_loc++] = str[i++];
		vidptr[current_loc++] = color;
	}
}

// strlen strcpy strcat

void kprint_newline(void)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = current_loc + (line_size - current_loc % (line_size));
}

void clear_screen(void) {
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x07;
	}

}


void panic0() {
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x44;
	}
	kprint("err: kernel panic!",0x40);
	kprint_newline();
	kprint("err type:",0x40);
	kprint_newline();
	kprint("fault: divide-by-zero (0x00)",0x40);
	kprint_newline();
	kprint_newline();
	asm volatile(
          "1:\n\t"
          "cli\n\t"
          "hlt\n\t"
          "jmp 1b\n\t"
          );	
}

void panic1() {
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x44;
	}
	kprint("err: kernel panic!",0x40);
	kprint_newline();
	kprint("err type:",0x40);
	kprint_newline();
	kprint("fault: bound range exceeded (0x05)",0x40);
	kprint_newline();
	kprint_newline();
	asm volatile(
          "1:\n\t"
          "cli\n\t"
          "hlt\n\t"
          "jmp 1b\n\t"
          );	
}

void panic2() {
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x44;
	}
	kprint("err: kernel panic!",0x40);
	kprint_newline();
	kprint("err type:",0x40);
	kprint_newline();
	kprint("trap: overflow detected (0x04)",0x40);
	kprint_newline();
	kprint_newline();
	asm volatile(
          "1:\n\t"
          "cli\n\t"
          "hlt\n\t"
          "jmp 1b\n\t"
          );	
}


unsigned char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

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
			return;
		}

		vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
		vidptr[current_loc++] = 0x07;
	}
}


	
		
void kmain(void) {
	clear_screen();
	kprint("Color Test:",0x07);
	kprint_newline();
	kprint("0",0x00);
	kprint("0",0x11);
	kprint("0",0x22);
	kprint("0",0x33);
	kprint("0",0x44);
	kprint("0",0x55);
	kprint("0",0x66);
	kprint("0",0x77);
	kprint("0",0x88);
	kprint("0",0x99);
	kprint("0",0xAA);
	kprint("0",0xBB);
	kprint("0",0xCC);
	kprint("0",0xDD);
	kprint("0",0xEE);
	kprint("0",0xFF);
	kprint_newline();
	const char *str = "Codename Spectrum Build 0.4.0";
	kprint(str, 0x0B);
	kprint_newline();
	kprint_newline();
	idt_init();
	kb_init();
	while(1);
}

