#include "ports.h"

unsigned char read_port(unsigned short _port) {
	unsigned char result;
	__asm__ ("in %%dx, %%al" : "=a" (result) : "d" (_port));
	return result;
}

void write_port(unsigned short _port, unsigned char _data) {
	__asm__ ("out %%al, %%dx" : : "a" (_data), "d" (_port));
}
