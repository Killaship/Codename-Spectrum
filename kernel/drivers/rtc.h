#pragma once
#ifndef KERNEL_C

#include "../kernel.c"
#endif
void initRTC();

int getSeconds();
int getMinutes();
int getHours();
int getDay();
int getMonth();
int getYear();
