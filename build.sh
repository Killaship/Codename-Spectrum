
fs() {
	echo "compiling"
	gcc -Wall -Wextra -fno-stack-protector -m32 -g -c src/programs/testA.c -o testA.o -ffreestanding
	echo "linking"
	ld -Tlink.ld -g -m elf_i386 -o testA.elf testA.o
	#smlrl -flat32 -origin 0x80000 -o testA.bin testA.o
	echo "convertifying"
	objcopy -O binary testA.elf testA.bin
	xxd -i testA.bin src/testA.h
}

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
help() {
	echo "Usage:"
	echo "-r            || Compile the YAOS source code, and then runs it in QEMU."
	echo "-n            || Just runs YAOS in QEMU. Requires os.iso to be present from prior compilings."
	echo "-c            || Just compiles the YAOS source code, without running it."
	echo "anything else, or -h || Shows this help text."

}
# Get the options
while getopts ":nrc" option; do
   case $option in
      h) # help message
		 help
		 exit;;
      r) # compile and run
		 fs
         compile
         run
         exit;;
      c) # just compile
		 fs
      	 compile
      	 exit;;
      n) # just run
      	 run
      	 exit;;

      \?) # invalid option, shows help message
		 help
		 exit;;
                             
      esac
done

