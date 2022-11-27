
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define TRAP_GATE 0b10001111
#define KERNEL_CODE_SEGMENT_OFFSET 0x08



void load_idt(unsigned long *idt_ptr) {
	asm volatile ("lidt (%%eax)" :: "a" (idt_ptr));
}


struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

void unmaskIRQ(int irq)
{
       static unsigned short int ocw1 = 0xffff;
        ocw1 &= ~(1 << irq);     // enable propriate bit with shifting to left
                                 // invert to enable the interrupt
                                 // use & to not disturb other bits
        if (irq < 8)
                write_port(0x21, ocw1 & 0xFF);  // AND with 0xFF to clear the high 8
                                                 //  bits because we send to PIC1
        else
                write_port(0xA1, ocw1 >> 8);     // move high 8 bits to low 8 bits
                                                // since we send to PIC2
}

void idt_init(void)
{
	
 	unsigned long addr_address[32] = {0};
	unsigned long idt_address;
	unsigned long idt_ptr[2];
	
	// PIT timer shit
	unsigned int pit_address;

	unsigned int keyboard_address;
	int i = 0;
	while(i < 31) {
			addr_address[i] = (unsigned long)idtfunc[i];
			IDT[i].offset_lowerbits = addr_address[i] & 0xffff;
			IDT[i].selector = KERNEL_CODE_SEGMENT_OFFSET;
			IDT[i].zero = 0;
			IDT[i].type_attr = INTERRUPT_GATE;
			IDT[i].offset_higherbits = (addr_address[i] & 0xffff0000) >> 16;	
		i++;
	}
	/*
	PIT: IRQ0
	KB:  IRQ1
	*/
	// handler for PIT, address is 0x20 instead of 0x00 since PIC was remapped past the exception handlers
	pit_address = &timer_handler;
	IDT[0x20].offset_lowerbits = pit_address & 0xffff;
	IDT[0x20].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x20].zero = 0;
	IDT[0x20].type_attr = INTERRUPT_GATE;
	IDT[0x20].offset_higherbits = (pit_address & 0xffff0000) >> 16;

	// handler for keyboard, address is 0x21
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

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
	write_port(0x21, 0b100); //pin 2 is where the second PIC is connected to
	write_port(0xA1, 2); //second PIC is, well, the second pic.

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);
	unmaskIRQ(0);
	unmaskIRQ(1);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
	
}
