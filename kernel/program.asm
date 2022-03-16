; external program to be loaded by the kernel later on
; set eax to some distinguishable number, to read from the log afterwards

push 0x01
int 0x80
push 0x02
int 0x80
jmp $
