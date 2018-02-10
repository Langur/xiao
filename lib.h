/**
  lib.h
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __LIB_H__
#define __LIB_H__

#include "syscall.h"

typedef struct div_t {
	unsigned int div;
	unsigned int mod;
} div_t;

void *memset(void *, int , long);
void *memcpy(void *, const void *, long);
int memcmp(const void *, const void *, long);
int strlen(const char *);
char *strcpy(char *, const char *);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, int);

int putc(unsigned char);
int puts(char *);
unsigned char getc(void);
int gets(unsigned char *);
int putdval(long, int);
int putuval(unsigned long, int);
int putxval(unsigned long, int);

div_t div(unsigned int, unsigned int);
int snprintf(char *, int, char *, ...);
char *hex2dec(unsigned char);

#define malloc(size)                      get_memory(size)
#define free(address)                     release_memory(address)

#define i_malloc(size)                     i_get_memory(size)

#endif /* __LIB_H__ */
