echo "NOTE: This requires QEMU installed, and an X server as well."
echo "Now Building..."
git pull
nasm -f elf32 kernel/kernel.asm -o kasm.o
gcc -Wall -fno-stack-protector -m32 -c kernel/kernel.c -o kc.o
ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o
echo "Build Complete. Now running in QEMU."
export DISPLAY=:0.0 # Replace 0.0 with the address of your X server if needed.
qemu-system-i386 -no-reboot -kernel kernel
