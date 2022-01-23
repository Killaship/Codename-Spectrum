echo "NOTE: This requires QEMU installed, and an X server as well."

echo "Now Building..."
rm *.o
rm -rf iso
rm *iso

git pull

#nasm -f elf32 boot/boot.asm -o boot.o
cd kernel
nasm -f elf32 kernel.asm -o kasm.o

gcc -Wall -Wextra -fno-stack-protector -m32 kernel.c rtc.c cpu.c panic.c tty.c



ld -m elf_i386 -T ../link.ld -o kernel.elf kasm.o kc.o rtc.o cpu.o panic.o tty.o
cd -

mkdir -p iso/boot/grub
cp kernel.elf iso/boot/kernel.elf
cp grub.cfg iso/boot/grub/grub.cfg
grub-mkrescue -o os.iso iso

echo "Build Complete. Now running in QEMU."

export DISPLAY=:0.0 # Replace 0.0 with the address of your X server if needed.
qemu-system-i386 -cdrom os.iso -rtc base=localtime #-curses

