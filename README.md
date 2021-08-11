# Codename Spectrum
An open-source operating system that I'm making as a side project. The bootloading is currently handled by the -kernel flag in QEMU, I'll add an area on how to set up Limine soon. 

IMPORTANT: The OS is 32 bit.

Build Commands:

```nasm -f elf32 kernel.asm -o kasm.o```

```gcc -m32 -c kernel.c -o kc.o```

```ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o```
Note: If ld returns an error, recompile and use this command instead:

```gcc -fno-stack-protector -m32 -c kernel.c -o bin/kc.o```


NOTE: This part requires QEMU and an X Server. My enviroment is WSL, I haven't tested it anywhere else.

```export DISPLAY=:0```

```qemu-system-i386 -no-reboot -kernel ./path/to/kernel/image``` 

This is under the GPL license now because this: https://github.com/arjun024/mkeykernel/blob/master/LICENSE

Most of the code is taken from the following repo: https://github.com/arjun024/mkeykernel
Changes made: expanding on the kernel, taking the code for the keyboard handler.
Please don't sue me. IDFK much about licenses.

Also naming system: Pick a codeword and make up a better name closer to release.
