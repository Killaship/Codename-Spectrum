bits 32

global start
extern kmain

start:
	cli
	mov esp, stack_space
	call kmain
	hlt

section .bss
resb 8192
stack_space:
