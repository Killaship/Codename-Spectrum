; external program to be loaded by the kernel later on
; set eax to some distinguishable number, to read from the log afterwards
mov eax, 0xDEADBEEF
; enter infinite loop, nothing more to do
; $ means "beginning of line", ie. the same instruction
jmp $

