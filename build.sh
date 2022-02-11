

echo "Now Building..."
rm *.o
rm -rf iso
rm *iso
git pull



cd kernel
nasm -f elf32 kernel.asm -o kasm.o
cd -
gcc -Wall -Wextra -fno-stack-protector -m32 -c kernel/kernel.c -o kc.o -ffreestanding

cd install
nasm -f elf32 kernel.asm -o iasm.o
cd -
gcc -Wall -Wextra -fno-stack-protector -m32 -c install/kernel.c -o ic.o -ffreestanding


ld -m elf_i386 -T link.ld -o kernel.elf kernel/kasm.o kc.o 
ld -m elf_i386 -T link.ld -o install.elf install/iasm.o ic.o 


mkdir -p iso/boot/grub
cp kernel.elf iso/boot/kernel.elf
cp install.elf iso/boot/install.elf
cp grub.cfg iso/boot/grub/grub.cfg
grub-mkrescue -o os.iso iso

echo "Build Complete. Now running in QEMU."

export DISPLAY=:0.0 # Replace 0.0 with the address of your X server if needed.
qemu-system-i386 -cdrom os.iso -rtc base=localtime -curses
