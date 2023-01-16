# Codename-Spectrum
An open-source operating system that I'm making as a side project, with my own kernel.
# The 'main' branch is usually is a WIP, refer to Releases for stable versions.

Use ```bash build.sh``` to build the OS.

# To-Do: (in no specific order)

Installation capabilities via a separate entry in GRUB (Halfway done, small boot shell; haven't figured out copying to a drive or anything)

More extensive CPUID (this is a good idea)

Read only Filesystem via abuse of an initrd and/or ramdisk (might not do, it'll probably not be of much use, since I'll probably roll my own)

Finish adding all of the exception handlers (gotta grab all that from YAOS, this is most likely the soonest thing done)

Serial communication 

Add support for shift in keyboard (someone please help, I have no clue how to do this)

Make it limy limy limine time (probably won't do this, it would require a switch to 64 bit IIRC)
