#pragma once

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

/* current cursor location */
unsigned int current_loc = 0;
/* video memory begins at address 0xb8000 */
char *vidptr = (char*)0xb8000;
/* char buffer for the shell */
char buff[128]; // note to self: don't worry about initialization as this array is global and defaults to a null char.
/* pointer to where we're writing into the buffer */
int buffptr = 0;

void kprint(const char *str, const int color) {
	
	unsigned int i = 0;
	while (str[i] != '\0') {
		vidptr[current_loc++] = str[i++];
		vidptr[current_loc++] = color;
	}
}


void putc(const char str) {
	vidptr[current_loc++] = str;
	vidptr[current_loc++] = 0x07;
	
}
void kprint_newline(void)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = current_loc + (line_size - current_loc % (line_size));
}

void clear_screen(void) {
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x07;
	}
	current_loc = 0;

}



