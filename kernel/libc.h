#pragma once

// Possibly libc stuff
// https://wiki.osdev.org/Meaty_Skeleton
// This isn't really even any sort of libc, more just bits and pieces I've swiped from places. (the osdev wiki)

void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}


char *strncpy(char *s1, const char *s2, size_t n)
{
	unsigned int extern_iter = 0; //when s2's length is shorter than n, this allows the function to continue padding null characters

	unsigned int iterator = 0;
	for (iterator = 0; iterator < n; iterator++) //iterate through s2 up to char n, copying them to s1
	{
		if (s2[iterator] != '\0')
			s1[iterator] = s2[iterator];
		else //the end of s2 was found prematurely - copy the null character, update external iterator and quit for loop
		{
			s1[iterator] = s2[iterator];
			extern_iter = iterator + 1;
			break;
		}
	}

	while (extern_iter < n) //while there are still spaces that need to be filled with null characters, fill them
	{
		s1[extern_iter] = '\0';
		extern_iter++;
	}

	return s1;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int count = 0;
	while (count < n)
	{
		if (s1[count] == s2[count])
		{
			if (s1[count] == '\0') //quit early because null-termination found
				return 0;
			else
				count++;
		}
		else
			return s1[count] - s2[count];
	}}

int memcmp(const void* aptr, const void* bptr, size_t size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void* memmove(void* dstptr, const void* srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (size_t i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

char* itoa(int i)
{
      static char text[12];
      int loc = 11;
      text[11] = 0;
      char neg = 1;
      if (i >= 0)
      {
         neg = 0;
         i = -i;
      }
      while (i)
      {
          text[--loc] = '0' - (i%10);
          i/=10;
      }
      if (loc==11)
          text[--loc] = '0';
      if (neg)
         text[--loc] = '-';      
      return &text[loc];
}

int strcmp(const char *s1, const char *s2)
{
	if (strlen(s1) != strlen(s2))
		return s2-s1;
	return strncmp(s1, s2, strlen(s1)); //It doesn't matter what the n is at this point - they should be the same length anyways
}
