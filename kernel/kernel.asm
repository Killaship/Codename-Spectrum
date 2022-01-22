; Parts of this made by Arjun Sreedharan
; License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html



bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002              ;magic
        dd 0x00                    ;flags
        dd - (0x1BADB002 + 0x00)   ;checksum. m+f+c should be zero

%include "gdt.asm"

;global div0_handler
;global boundrx_handler
;global overf_handler
global start
global keyboard_handler
global load_idt
global enable_ints
global disable_ints
global read_port
global write_port

extern kmain 		;this is defined in the c file
extern keyboard_handler_main
global dummy_int
;extern panic0 ;div0
;extern panic1 ;boundrx
;extern panic2 ;overflow

dummy_int:
	int 0xFF
	ret

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

hello: db "It works! (no shit sherlock)",0

;div0_handler:
;	cli
;	call	panic0

;boundrx_handler:
;	cli
;	call	panic1
	
;overf_handler:
;	cli
;	call	panic2
start:
     lgdt [gdt_pointer]  
     jmp CODE_SEG:kmain
    .setcs:
    mov ax, DATA_SEG          ; Setup the segment registers with our flat data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, stack_space        ; set stack pointer			;halt the CPU
    mov esi,hello
    mov ebx,0xb8000
	.loop:
		lodsb
		or al,al
		jz halt
		or eax,0x0100
		mov word [ebx], ax
		add ebx,2
		jmp .loop

section .bss
resb 8192 ; 8KB for stack
stack_space:
