void kprint(const char *str, const int color);
void kprint_newline(void);
void clear_screen(void);
char read_port(unsigned short port);
void write_port(unsigned short port, unsigned char data);
void disable_ints(void);
void enable_ints(void);
