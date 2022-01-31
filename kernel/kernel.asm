; Parts of this made by Arjun Sreedharan
; License: GPL version 2 or higher http://www.gnu.org/licenses/gpl.html
; actually this was changed so much that should I even bother including this anymore?
; it's not even close to the original program, with almost everything being rewritten or added to
; ship of theseus moment


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
