typedef struct fs_node
{
   char name[128];     // The filename.
   u32int mask;        // The permissions mask.
   u32int uid;         // The owning user.
   u32int gid;         // The owning group.
   u32int flags;       // Includes the node type. See #defines above.
   u32int inode;       // This is device-specific - provides a way for a filesystem to identify files.
   u32int length;      // Size of the file, in bytes.
   u32int impl;        // An implementation-defined number.
   read_type_t read;
   write_type_t write;
   open_type_t open;
   close_type_t close;
   readdir_type_t readdir;
   finddir_type_t finddir;
   struct fs_node *ptr; // Used by mountpoints and symlinks.
} fs_node_t;

// http://www.jamesmolloy.co.uk/tutorial_html/8.-The%20VFS%20and%20the%20initrd.html
