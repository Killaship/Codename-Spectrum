#pragma once
#include "rtc.h"

char read_port(unsigned short port);
void write_port(unsigned short port, unsigned char data);

//cpu.c
static inline int cpuid_string(int code, int where[4]);
const char * const cpu_string();

//panic.c
void hang();
void div0_handler();
void debg_handler();
void nmi_handler();
void overf_handler();
void boundrx_handler();
void dfault_handler();

//tty.c
void kprint(const char *str, const int color);
void kprint_newline(void);
void clear_screen(void);
void input_prompt();
void cmdflush();
void shell();
void sh_init();
