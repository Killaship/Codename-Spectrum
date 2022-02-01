/* returns file size and pointer to file data in out */
int tar_lookup(unsigned char *archive, char *filename, char **out) {
    unsigned char *ptr = archive;
 
    while (!memcmp(ptr + 257, "ustar", 5)) {
        int filesize = oct2bin(ptr + 0x7c, 11);
        if (!memcmp(ptr, filename, strlen(filename) + 1)) {
            *out = ptr + 512;
            return filesize;
        }
        ptr += (((filesize + 511) / 512) + 1) * 512;
    }
    return 0;
}
