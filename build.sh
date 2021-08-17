echo "NOTE: This requires QEMU installed, and an X server as well."
echo "Now Building..."
rm kc.o
rm kasm.o
git pull
nasm -f elf32 kernel/kernel.asm -o kasm.o
gcc -Wall -fno-stack-protector -m32 -c kernel/kernel.c -o kc.o
ld -m elf_i386 -T link.ld -r kasm.o kc.o -o loadableimg.o 
echo "Build Complete. To use, link output file with Photon Bootloader."

