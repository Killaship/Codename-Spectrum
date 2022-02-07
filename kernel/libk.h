#pragma once

// Possibly libk stuff
// https://wiki.osdev.org/Meaty_Skeleton


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
	}
	return 0;
}

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

int oct2bin(unsigned char *str, int size) {
    int n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}

size_t
STRSPN (const char *str, const char *accept)
{
  if (accept[0] == '\0')
    return 0;
  if ((accept[1] == '\0'))
    {
      const char *a = str;
      for (; *str == *accept; str++);
      return str - a;
    }
  /* Use multiple small memsets to enable inlining on most targets.  */
  unsigned char table[256];
  unsigned char *p = memset (table, 0, 64);
  memset (p + 64, 0, 64);
  memset (p + 128, 0, 64);
  memset (p + 192, 0, 64);
  unsigned char *s = (unsigned char*) accept;
  /* Different from strcspn it does not add the NULL on the table
     so can avoid check if str[i] is NULL, since table['\0'] will
     be 0 and thus stopping the loop check.  */
  do
    p[*s++] = 1;
  while (*s);
  s = (unsigned char*) str;
  if (!p[s[0]]) return 0;
  if (!p[s[1]]) return 1;
  if (!p[s[2]]) return 2;
  if (!p[s[3]]) return 3;
  s = (unsigned char *) PTR_ALIGN_DOWN (s, 4);
  unsigned int c0, c1, c2, c3;
  do {
      s += 4;
      c0 = p[s[0]];
      c1 = p[s[1]];
      c2 = p[s[2]];
      c3 = p[s[3]];
  } while ((c0 & c1 & c2 & c3) != 0);
  size_t count = s - (unsigned char *) str;
  return (c0 & c1) == 0 ? count + c0 : count + c2 + 2;
}

char * strtok2(char *str, const char *delim)
{
static char *nxt; /* static variable used to advance the string to replace delimiters */
static int size;  /* static variable used to count until the end of the string        */

/* IMPORTANT: any advance to 'nxt' must be followed by a diminution of 'size', and vice verce */

int i; /* counter of delimiter(s) in string */

/* initialize the string when strtok2 is first calles and supplied with a valid string */
if(str != NULL)
{
    nxt = str;
    size = strlen(str);
}

/* if we havn't reached the end of the string, any other call to strtok2 with NULL will come here */
else if(size > 0)
{
    nxt++;      /* last run left nxt on a null terminator, so we have to advance it */
    size--;     /* any advancement must follow diminution of size                   */
    str = nxt;  /* string now points to the first char after the last delimiter     */ 
}

/* if we reached the end of string, return NULL pointer */
else
{
    str = NULL;
}

/* nxt is used to advance the string until a delimiter or a series of delimiters are encountered; 
 * it then stops on the last delimiter which has turned to NULL terminator
 */
while(*nxt)
{
    i = strspn(nxt, delim);
    while(i > 1)        /* turns delimiters to NULL terminator (except the last one) */
    {
        *nxt = '\0';
        nxt++;
        size--;
        i--;
    }                   /* in the last delimiter we have to do something a      */
    if(1 == i)          /* bit different we have to actually take nxt backwards */
    {                   /* one step, to break the while(*nxt) loop              */
        *nxt = '\0';
        if(size > 1)    /* if the delimiter is last char, don't do next lines   */
        {
            nxt--;
            size++;     /* nxt is diminished so size increases                    */
        }
    }
    nxt++;          /* if no delimiter is found, advance nxt                  */
    size--;         /* in case of the last delimiter in a series, we took nxt */
}                   /* a step back, so now moving it a step forward means     */
                    /* it rests on a NULL terminator                          */
return str;
}
