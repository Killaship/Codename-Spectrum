/*
* Parts of this was made by Arjun Sreedharan, all credit where due.
* License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
*/

#include <stdint.h>
#include <stddef.h>
#include "libk.h"
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define osversion "Codename Spectrum v0.6.1"


//extern void boundrx_handler(void);
//extern void div0_handler(void);
//extern void overf_handler(void);
extern void keyboard_handler(void);
//extern void pit_handler(void);

extern void loadprgm(void);

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

#include "keyboard_map.h"
#include "tty.h"

#include "panic.h"
#include "cpu.h"
#include "rtc.h"





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
	unsigned long badtss_address;
	unsigned long badop_address;
	unsigned long reserved_address;
	unsigned long ss_address;
	unsigned long gp_address;
 
	
	
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
	
	gp_address = (unsigned long)gp_handler;
	IDT[0x00].offset_lowerbits = gp_address & 0xffff;
	IDT[0x00].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x00].zero = 0;
	IDT[0x00].type_attr = INTERRUPT_GATE;
	IDT[0x00].offset_higherbits = (gp_address & 0xffff0000) >> 16;
 
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
	
 	badop_address = (unsigned long)badop_handler;
	IDT[0x08].offset_lowerbits = badop_address & 0xffff;
	IDT[0x08].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x08].zero = 0;
	IDT[0x08].type_attr = INTERRUPT_GATE;
	IDT[0x08].offset_higherbits = (badop_address & 0xffff0000) >> 16; 

	boundrx_address = (unsigned long)boundrx_handler;
	IDT[0x05].offset_lowerbits = boundrx_address & 0xffff;
	IDT[0x05].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x05].zero = 0;
	IDT[0x05].type_attr = INTERRUPT_GATE;
	IDT[0x05].offset_higherbits = (boundrx_address & 0xffff0000) >> 16;
	
	ss_address = (unsigned long)ss_handler;
	IDT[0x0c].offset_lowerbits = ss_address & 0xffff;
	IDT[0x0c].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x0c].zero = 0;
	IDT[0x0c].type_attr = INTERRUPT_GATE;
	IDT[0x0c].offset_higherbits = (ss_address & 0xffff0000) >> 16;
 
  	dfault_address = (unsigned long)dfault_handler;
	IDT[0x08].offset_lowerbits = dfault_address & 0xffff;
	IDT[0x08].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x08].zero = 0;
	IDT[0x08].type_attr = INTERRUPT_GATE;
	IDT[0x08].offset_higherbits = (dfault_address & 0xffff0000) >> 16;
	
	badtss_address = (unsigned long)badtss_handler;
	IDT[0x0a].offset_lowerbits = badtss_address & 0xffff;
	IDT[0x0a].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x0a].zero = 0;
	IDT[0x0a].type_attr = INTERRUPT_GATE;
	IDT[0x0a].offset_higherbits = (badtss_address & 0xffff0000) >> 16;	
	
	reserved_address = (unsigned long)reserved_handler;
	IDT[0x0f].offset_lowerbits = reserved_address & 0xffff;
	IDT[0x0f].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x0f].zero = 0;
	IDT[0x0f].type_attr = INTERRUPT_GATE;
	IDT[0x0f].offset_higherbits = (reserved_address & 0xffff0000) >> 16;		
	
	reserved_address = (unsigned long)reserved_handler;
	IDT[0x16].offset_lowerbits = reserved_address & 0xffff;
	IDT[0x16].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x16].zero = 0;
	IDT[0x16].type_attr = INTERRUPT_GATE;
	IDT[0x16].offset_higherbits = (reserved_address & 0xffff0000) >> 16;
	
	reserved_address = (unsigned long)reserved_handler;
	IDT[0x17].offset_lowerbits = reserved_address & 0xffff;
	IDT[0x17].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x17].zero = 0;
	IDT[0x17].type_attr = INTERRUPT_GATE;
	IDT[0x17].offset_higherbits = (reserved_address & 0xffff0000) >> 16;
	
	reserved_address = (unsigned long)reserved_handler;
	IDT[0x18].offset_lowerbits = reserved_address & 0xffff;
	IDT[0x18].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x18].zero = 0;
	IDT[0x18].type_attr = INTERRUPT_GATE;
	IDT[0x18].offset_higherbits = (reserved_address & 0xffff0000) >> 16;	
	
	reserved_address = (unsigned long)reserved_handler;
	IDT[0x19].offset_lowerbits = reserved_address & 0xffff;
	IDT[0x19].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x19].zero = 0;
	IDT[0x19].type_attr = INTERRUPT_GATE;
	IDT[0x19].offset_higherbits = (reserved_address & 0xffff0000) >> 16;	

	reserved_address = (unsigned long)reserved_handler;
	IDT[0x1a].offset_lowerbits = reserved_address & 0xffff;
	IDT[0x1a].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x1a].zero = 0;
	IDT[0x1a].type_attr = INTERRUPT_GATE;
	IDT[0x1a].offset_higherbits = (reserved_address & 0xffff0000) >> 16;
	
	reserved_address = (unsigned long)reserved_handler;
	IDT[0x1b].offset_lowerbits = reserved_address & 0xffff;
	IDT[0x1b].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x1b].zero = 0;
	IDT[0x1b].type_attr = INTERRUPT_GATE;
	IDT[0x1b].offset_higherbits = (reserved_address & 0xffff0000) >> 16;	
	
 	
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











/*
#define PORT 0x3F8         // COM1
 
static int serial_init() {
   write_port(PORT + 1, 0x00);    // Disable all interrupts
   write_port(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   write_port(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   write_port(PORT + 1, 0x00);    //                  (hi byte)
   write_port(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   write_port(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   write_port(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   write_port(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   write_port(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(read_port(PORT + 0) != 0xAE) {
      return 1;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   write_port(PORT + 4, 0x0F);
   return 0;
}


int serial_received() {
   return read_port(PORT + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
 
   return read_port(PORT);
}

int is_transmit_empty() {
   return read_port(PORT + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   write_port(PORT,a);
}

*/
void printcpu() {
	kprint("Vendor ID: ", 0x07);
	kprint(cpu_string(), 0x0C);
}

void printtime() {
	asm volatile ("cli");
    	read_rtc();
    	asm volatile ("sti");
	
	kprint("System Time: ",0x07);
	kprint(itoa(hour),0x07);
	kprint(":",0x07);
	kprint(itoa(minute),0x07);
	kprint(":",0x07);
	kprint(itoa(second),0x07);
}

#include "kb.h"
// #include "fs.h"

void kmain(void) {
	clear_screen();
	//serial_init();
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
	
	
	const char *str2 = "               \"oh holy crap there's finally a shell (but no fs)\"";
	kprint(osversion, 0x0B);
	kprint_newline();
	kprint(str2, 0x0E);
	kprint_newline();
	kprint_newline();
	
	printcpu();
	kprint_newline();
	printtime();
	
	
	
	sh_init();
	while(1);
}
