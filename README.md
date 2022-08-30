# Check YAOS for a version that has a fancier error screen, and might be updated more by me.
YAOS is supposed to basically be a rewrite. I don't like how I hardly had anything useful in this OS's kernel. 
YAOS is less about placing functionality and UI in the kernel, and more about trying to do kernel things in the kernel. However, YAOS, as of writing, is no longer updated on Github, and neither is this, because Github is mandating 2FA in 2023, and I don't have a phone. 

Differences between YAOS and Spectrum (and Proton)
YAOS is more about kernel stuff like memory management
Spectrum is a kernel shell
Proton is a 16 bit kernel shell that relies on BIOS interrupts.
# Codename Spectrum
An open-source operating system that I'm making as a side project. This isn't meant for real hardware. 

Keep in mind all this is subject to change, especially since I rarely update the readme.

IMPORTANT: The OS is 32 bit, and is targeted for legacy hardware. Use a VM, preferably QEMU.


![image](https://user-images.githubusercontent.com/69988679/187515188-42264d6e-fb92-4f5a-94d2-ccba0fd9135c.png)

<img width="515" alt="image" src="https://user-images.githubusercontent.com/69988679/187515262-89597a8d-938b-40aa-a889-a5a836bd123f.png">










# The 'main' branch is usually is a WIP, refer to Releases for stable versions.

Use ```bash build.sh``` to build the OS.

# To-Do: (in no specific order)

Installation capabilities via a separate entry in GRUB

More extensive CPUID 

Read only Filesystem via abuse of an initrd

Finish adding all of the exception handlers

Serial communication 

Add support for shift in keyboard.

Make it limy limy limine time






Some of the code is taken from the following repo: https://github.com/arjun024/mkeykernel
Changes made: expanding on the kernel, taking the code for the keyboard handler, adding more IDT entries and a kernel panic screen, etc.

