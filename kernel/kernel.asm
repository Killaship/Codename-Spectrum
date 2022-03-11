; Parts of this made by Arjun Sreedharan
; License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
; actually this was changed so much that should I even bother including this anymore?
; it's not even close to the original program, with almost everything being rewritten or added to
; ship of theseus moment

    MAGIC_NUMBER    equ 0x1BADB002      ; define the magic number constant
    ALIGN_MODULES   equ 0x00000001      ; tell GRUB to align modules

    ; calculate the checksum (all options + checksum should equal 0)
    CHECKSUM        equ -(MAGIC_NUMBER + ALIGN_MODULES + 0x00)

bits 32
section .text
        ;multiboot spec
        align 4

        dd MAGIC_NUMBER                 ; write the magic number
        dd ALIGN_MODULES                ; write the align modules instruction
        dd CHECKSUM   
%include "gdt.asm"

;global div0_handler
;global boundrx_handler
;global overf_handler
global start
global keyboard_handler
global loadprgm

extern kmain 		;this is defined in the c file
extern keyboard_handler_main
;extern panic0 ;div0
;extern panic1 ;boundrx
;extern panic2 ;overflow

keyboard_handler:                 
	call    keyboard_handler_main
	iretd

loadprgm:
	jmp $ ; do later

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
     push ebx
     jmp CODE_SEG:kmain
    .setcs:
    mov ax, DATA_SEG          ; Setup the segment registers with our flat data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, stack_space        ; set stack pointer		

section .bss
resb 8192 ; 8KB for stack
stack_space:
