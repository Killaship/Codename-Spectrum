/*
* Parts of this was made by Arjun Sreedharan, all credit where due.
* License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
*/

#include "keyboard_map.h"
#include "tty.h"
#include "panic.h"
#include "cpu.h"
#include "rtc.h"



#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define ENTER_KEY_CODE 0x1C


//extern void boundrx_handler(void);
//extern void div0_handler(void);
//extern void overf_handler(void);
extern void keyboard_handler(void);
//extern void pit_handler(void);

void load_idt(unsigned long *idt_ptr) {
	asm volatile ("lidt (%%eax)" :: "a" (idt_ptr));
}

char read_port(unsigned short port) {
	unsigned char value;
	asm volatile ("inb %%dx, %%al" : "=a" (value) : "d" (port));
	return value;
}

void write_port(unsigned short port, unsigned char data) {
	asm volatile ("outb %%al, %%dx" :: "a" (data), "d" (port));
}







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

void idt_init(void) {
	unsigned long div0_address;
 	unsigned long boundrx_address;
	unsigned long overf_address;
 	unsigned long nmi_address;
	unsigned long debg_address;
 	unsigned long dfault_address;
 
	
	
	unsigned long keyboard_address;
	//unsigned long pit_address;
 
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
	/*
	pit_address = (unsigned long)pit_handler;
	IDT[0x22].offset_lowerbits = pit_address & 0xffff;
	IDT[0x22].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x22].zero = 0;
	IDT[0x22].type_attr = INTERRUPT_GATE;
	IDT[0x22].offset_higherbits = (pit_address & 0xffff0000) >> 16;
	*/
	div0_address = (unsigned long)div0_handler;
	IDT[0x00].offset_lowerbits = div0_address & 0xffff;
	IDT[0x00].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x00].zero = 0;
	IDT[0x00].type_attr = INTERRUPT_GATE;
	IDT[0x00].offset_higherbits = (div0_address & 0xffff0000) >> 16;
 
 	debg_address = (unsigned long)debg_handler;
	IDT[0x01].offset_lowerbits = debg_address & 0xffff;
	IDT[0x01].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x01].zero = 0;
	IDT[0x01].type_attr = INTERRUPT_GATE;
	IDT[0x01].offset_higherbits = (debg_address & 0xffff0000) >> 16;
 
	nmi_address = (unsigned long)nmi_handler;
	IDT[0x02].offset_lowerbits = nmi_address & 0xffff;
	IDT[0x02].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x02].zero = 0;
	IDT[0x02].type_attr = INTERRUPT_GATE;
	IDT[0x02].offset_higherbits = (nmi_address & 0xffff0000) >> 16;
 

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
 
  	dfault_address = (unsigned long)dfault_handler;
	IDT[0x08].offset_lowerbits = dfault_address & 0xffff;
	IDT[0x08].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x08].zero = 0;
	IDT[0x08].type_attr = INTERRUPT_GATE;
	IDT[0x08].offset_higherbits = (dfault_address & 0xffff0000) >> 16;
 	

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





char last_char;

char getchar() {
	return last_char;
}

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
		last_char = keyboard_map[(unsigned char) keycode];
		vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
		vidptr[current_loc++] = 0x07;
	}
}


char* itoa(int i)
{
      static char text[12];
      int loc = 11;
      text[11] = 0;
      char neg = 1;
      if (i >= 0)
      {
         neg = 0;
         i = -i;
      }
      while (i)
      {
          text[--loc] = '0' - (i%10);
          i/=10;
      }
      if (loc==11)
          text[--loc] = '0';
      if (neg)
         text[--loc] = '-';      
      return &text[loc];
}



void kmain(void) {
	clear_screen();
	idt_init();
	kb_init();
	kprint_newline();
	
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
	
	const char *str = "Codename Spectrum v0.5.1";
	const char *str2 = "                    Now with more features! (but still no shell)";
	kprint(str, 0x0B);
	kprint_newline();
	kprint(str2, 0x0E);
	kprint_newline();
	kprint_newline();
	
	kprint("Vendor ID: ", 0x07);
	kprint(cpu_string(), 0x0C);
	kprint_newline();
	
	asm volatile ("cli");
    	read_rtc();
    	asm volatile ("sti");
	
	kprint("System Time: ",0x07);
	kprint(itoa(hour),0x07);
	kprint(":",0x07);
	kprint(itoa(minute),0x07);
	kprint(":",0x07);
	kprint(itoa(second),0x07);
	
	sh_init();
	while(1);
}

