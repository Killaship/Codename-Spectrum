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
