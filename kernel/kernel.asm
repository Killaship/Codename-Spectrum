; Parts of this made by Arjun Sreedharan
; License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html

bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002              ;magic
        dd 0x00                    ;flags
        dd - (0x1BADB002 + 0x00)   ;checksum. m+f+c should be zero
	
global div0_handler
global boundrx_handler
global overf_handler
global start
global keyboard_handler
global read_port
global write_port
global load_idt
global disable_ints
global enable_ints

extern kmain 		;this is defined in the c file
extern keyboard_handler_main
extern panic0 ;div0
extern panic1 ;boundrx
extern panic2 ;overflow

read_port:
	mov edx, [esp + 4]
			;al is the lower 8 bits of eax
	in al, dx	;dx is the lower 16 bits of edx
	ret

disable_ints:
	cli

enable_ints:
	sti

write_port:
	mov   edx, [esp + 4]    
	mov   al, [esp + 4 + 4]  
	out   dx, al  
	ret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti 				;turn on interrupts
	ret

keyboard_handler:                 
	call    keyboard_handler_main
	iretd

div0_handler:
	cli
	call	panic0

boundrx_handler:
	cli
	call	panic1
	
overf_handler:
	cli
	call	panic2
start:
	cli 				;block interrupts
	mov esp, stack_space
	call kmain
	hlt 				;halt the CPU

section .bss
resb 8192; 8KB for stack
stack_space:
