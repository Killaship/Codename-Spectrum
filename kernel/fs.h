struct tar_header
{
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
};

unsigned int getsize(const char *in)
{
 
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;
 
    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);
 
    return size;
 
}

struct tar_header *headers[32];

//  Currently just putting the tarball as an ELF along with the kernel
//  _binary_initrd_tar_start < Symbol to reference (or objdump -t initrd.o)
//  Use the address of the symbol for the initrd for the parsing stuff

unsigned int parse(unsigned int address)
{
 
    unsigned int i;
 
    for (i = 0; ; i++)
    {
 
        struct tar_header *header = (struct tar_header *)address;
 
        if (header->name[0] == '\0')
            break;
 
        unsigned int size = getsize(header->size);
 
        headers[i] = header;
 
        address += ((size / 512) + 1) * 512;
 
        if (size % 512)
            address += 512;
 
    }
 
    return i;
 
}



/*
taken from a reddit thing https://libredd.it/r/osdev/comments/pv2lw2/about_the_fs_is_it_in_one_binary_with_the_os/

There's a few ways to do it.

If you want a file system that's available at boot time, you can compile any arbitrary file as binary data into your kernel with gcc (I'd assume clang let's you do it). I'm on mobile so I won't try and format some example code, but googling that should get you on the right path. Then at runtime you can load your file system 'blob' into memory and just tell your code to treat that data as if it were coming from a storage drive.

Now that works for 1 file, but how about for a number of them? I just tar them all together, leaving off any compression so I don't have to decompress later. This is super easy to add to a makefile or other build system.

Add files to your tarball, turn the tarball into an elf object, add it to your linking stage, and then you're good to go.

This is how I do it personally, as it works with any bootloader and any platform, dosnt require any sort of api or drivers to be available.

You can also put the files you want onto the same iso/img as your kernel and have your bootloader load them for you (check out grubs modules, stivale has similar options I think although I've never used it personally).

Edit: it's ld not gcc that I'm using. Options are --format binary *your_file_here*. ld will generate symbols in your output file like _binary_*full_filename_here*[start|end|size]. To use these you can declare an extern "C" (if in c++land) char *symbol_name*. Then take the address of that symbol and itll give you the address in memory of the file you embedded.

Its worth noting that --format changes the input format of all files after that option, so use it last, after any code.

2

u/nerd4code Sep 25 '21
It’s fairly common to use an initr[am/om]d[isk], which is usually some sort of relatively-well-packed archive (often as a souped up, page-aligned linked list) loaded at the same time as the kernel, providing a handful of drivers, scripts, and basic executable files. This is how most Linux distros boot, and you either need this or for there to be some set of drivers built-in so that other drivers can be located and loaded; typically, once you’ve gotten the kernel to a state where it recognizes the important hardware (CPU, often display, disk, root filesystem[s]) you can “pivot” the root directory to a non-R[AO]Mdisk mount so you can bring up the remainder of the drivers and services needed.

But that’s putting the filesystem data next to the kernel, not in it.

It’s possible to mix code and data more freely if you have a single lower-level interface to target—one filesystem on one of a couple types of disk drive, and preferably one kind of mobo with a limited set of peripherals, or else some consistent BIOS API abstracting these things well enough, so that you don’t need much of a driver-per-se interface, just some static libs built into your kernel. (The BIOS usually does need to have a driver interface, then—e.g., the PC/XT BIOS mainly used driver ROMs for this purpose, which would be autodetected during a boot-time scan of the MMIO region from addx 0xA0000↑. These used an 0xAA55 = 0b10101010_01010101 code to signal bootability of some well-aligned region just like boot sectors, because a bus might float or stick in various ways if there’s nothing listening to/for a particular address, but probably not in a way that yields different, alternating bit patterns in adjacent bytes. Video, network, and disk controller cards usually still offer lower-megabyte driver ROMs, although the BIOS can enumerate hardware and find mappable/bootable stuff without having to scan the address space directly or rely on DIPswitches nowadays.)

But targeting only one kind of machine usually means that any moderately different kind of machine will need substantially different software, makingbit much harder to keep the various implementations in-step enough to share higher-level system software/data—compilers, shells, utilities, etc. are by far the bulk of most modern OS offerings, and most people are unwilling to shell out for software upon which none of their preexisting code will run, once they have preexisting code. (And so all the world’s a VAX, nowadays.)

But to your question more specifically, one fairly important feature of externalized storage formats is that they must be relatively self-contained. For the system drive, you want it to survive a sudden crash or power failure, and if it breaks you want to be able to remove it and poke at it from another computer. For removable drives, you want any two computers you’ll likely use to be able to access the data, at least in a read-only fashion. So there’s not much point in mixing the filesystem driver into the filesystem data (unless it’s as files)—every foreign OS will need its own driver anyway, and if you require use of on-disk code it’ll have to be an emulator, and if the disk drops or corrupts a sector here or there you really don’t want to find that out by crashing the file system driver and anything connected too directly to it.

In general: Use higher-order enumeration to find drives, use magic numbers to detect a filesystem on a (present) medium, use a well-defined format for fields in data structures (— Bi-endian? Record which is used, otherwise force LE or BE. Use mostly byte-granular, well-aligned layouts for things that might be mapped “directly” into memory. —), use checksums or hashes to tell you when important structures have been corrupted, and replicate important structures so there’s no single point of failure and data can ve rescued from a broken/corrupt medium. Come up with a meta-API for drivers relatively early, so you can at least list, locate, and interrogate loaded/built-in drivers and, if possible, heave those out into separate sub-/projects when you get to where you’re loading drivers dynamically. Sometimes you’ll want to have a simpler boot-time driver that you unload and replace with a more extensive driver after bootstrapping, so you’ll want to keep track of built-in drivers’ section/segment info separately, and usually keep driver code/data together so you can deallocate in one piece if you unload it.*/
