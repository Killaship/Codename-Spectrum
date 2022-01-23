echo "NOTE: This requires QEMU installed, and an X server as well."

echo "Now Building..."
rm *.o
rm -rf iso
rm *iso

git pull

#nasm -f elf32 boot/boot.asm -o boot.o
cd kernel
nasm -f elf32 kernel.asm -o kasm.o
cd -
gcc -Wall -Wextra -fno-stack-protector -m32 -c kernel/kernel.c -o kc.o

ld -m elf_i386 -T link.ld -o kernel.elf kernel/kasm.o kc.o #boot.o

mkdir -p iso/boot/grub
cp kernel.elf iso/boot/kernel.elf
cp grub.cfg iso/boot/grub/grub.cfg
grub-mkrescue -o os.iso iso

echo "Build Complete. Now running in QEMU."

export DISPLAY=:0.0 # Replace 0.0 with the address of your X server if needed.
qemu-system-i386 -cdrom os.iso #-curses

