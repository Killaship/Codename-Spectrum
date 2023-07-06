

compile() {

	cd src
	nasm -f elf32 kernel.asm -o kasm.o
	cd -

	gcc -Wall -Wextra -fno-stack-protector -m32 -g -c src/kernel.c -o kc.o -ffreestanding

	ld -m elf_i386 -T link.ld -g -o kernel.elf src/kasm.o kc.o 
	mkdir -p iso/boot/grub
	mkdir -p iso/modules
	cp kernel.elf iso/boot/kernel.elf
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o os.iso iso
}

run() {
	qemu-system-i386 -cdrom os.iso -rtc base=localtime -curses 
}

compile
run
exit;;
