; external program to be loaded by the kernel later on
; set eax to some distinguishable number, to read from the log afterwards

cli
mov edx, 0xDEADBEEF
jmp $
