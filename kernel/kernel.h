#pragma once

char read_port(unsigned short port);
void write_port(unsigned short port, unsigned char data);

//cpu.c
static inline int cpuid_string(int code, int where[4]);
const char * const cpu_string();
