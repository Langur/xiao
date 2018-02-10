/**
  lib.c
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include "defines.h"
#include "serial.h"
#include "lib.h"

void *
memset(void *b, int c, long len)
{
	char *p;

	for (p = b; len > 0; len--) {
		*(p++) = c;
	}
	
	return b;
}

void *
memcpy(void *dst, const void *src, long len)
{
	char *d = dst;
	const char *s = src;

	for (; len > 0; len--) {
		*(d++) = *(s++);
	}
	
	return dst;
}

int
memcmp(const void *b1, const void *b2, long len)
{
	const char *p1 = b1, *p2 = b2;

	for (; len > 0; len--) {
		if (*p1 != *p2)
			return (*p1 > *p2) ? 1 : -1;
		p1++;
		p2++;
	}

	return 0;
}

int
strlen(const char *s)
{
	int len;

	for (len = 0; *s; s++, len++) {
		;
	}
	
	return len;
}

char
*strcpy(char *dst, const char *src)
{
	char *d = dst;

	for (;; dst++, src++) {
		*dst = *src;
		if (!*src) {
			break;
		}
	}

	return d;
}

int
strcmp(const char *s1, const char *s2)
{
	while (*s1 || *s2) {
		if (*s1 != *s2) {
			return (*s1 > *s2) ? 1 : -1;
		}
		s1++;
		s2++;
	}

	return 0;
}

int
strncmp(const char *s1, const char *s2, int len)
{
	while ((*s1 || *s2) && (len > 0)) {
		if (*s1 != *s2) {
			return (*s1 > *s2) ? 1 : -1;
		}
		s1++;
		s2++;
		len--;
	}

	return 0;
}

int
putc(unsigned char c)
{
	if (c == '\n') {
		serial_send_byte(SERIAL_DEFAULT_DEVICE, '\r');
	}

	return serial_send_byte(SERIAL_DEFAULT_DEVICE, c);
}

int
puts(char *str)
{
	while (*str) {
		putc(*(str++));
	}

	return 0;
}

int
putuval(unsigned long value, int column)
{
	char buf[11], bufa[11];
	unsigned long tmp1, tmp2, tmp3, tmpa, tmpb;
	int i, j, sw;

	bufa[10] = '\0';
	for (i = 10, tmp1 = value; i > 0; i--) {
		tmp2 = 1;
		tmp3 = 0;
		for (j = 1; j < i; j++) {
			tmpa = tmp2 << 3;
			tmpb = tmp2 << 1;
			tmp2 = tmpa + tmpb;
		}
		while (tmp1 - tmp2 < tmp1) {
			tmp1 -= tmp2;
			tmp3++;
		}
		bufa[10 - i] = "0123456789abcdef"[tmp3];			
	}
	for (i = j = sw = 0; i < 11; i++) {
		if (!sw && bufa[i] != '0') {
			sw = 1;
		}
		if (sw) {
			buf[j++] = bufa[i];
		}
	}

	if (buf[0] == '\0') {
		buf[0] = '0';
		buf[1] = '\0';
	}

       	puts(buf);

	return 0;
}

int
putdval(long value, int column)
{
	unsigned long tmp;

	tmp = (unsigned long)value;
	if (tmp & 0x10000000) {
		tmp = ~tmp + 1;
		putc('-');
	}
	putuval(tmp, 0);

	return 0;
}

int
putxval(unsigned long value, int column)
{
	char buf[9];
	char *p;
	
	p = buf + sizeof(buf) - 1;
	*(p--) = '\0';

	if (!value && !column)
		column++;

	while (value || column) {
		*(p--) = "0123456789abcdef"[value & 0xf];
		value >>= 4;
		if (column) column--;
	}

	puts(p + 1);

	return 0;
}

unsigned char
getc(void)
{
	unsigned char c = serial_recv_byte(SERIAL_DEFAULT_DEVICE);

	c = (c == '\r') ? '\n' : c;
	putc(c); /* エコー・バック */

	return c;
}

int
gets(unsigned char *buf)
{
	int i = 0;
	unsigned char c;

	do {
		c = getc();
		if (c == '\n') {
			c = '\0';
		}
		buf[i++] = c;
	} while (c);

	return i - 1;
}

div_t
div(unsigned int x, unsigned int y)
{
	unsigned int *div, *mod;
	div_t result;
	div = &(result.div);
	mod = &(result.mod);
	*mod = x;
	*div = 0;
	while (*mod >= y) {
		*mod -= y;
		(*div)++;
	}

	return result;
}
typedef void* va_list;

#define va_size(type) (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(ap, last)        ((ap) = (va_list)&(last) + va_size(last))
#define va_arg(ap, type) (*(type *)((ap) += va_size(type), (ap) - va_size(type)))
#define va_end(ap)
#define parce_num(p, b, num, base, column, flag)				\
	do {			     						\
		unsigned long tmpnum;						\
		char *ptmp;							\
		div_t divnum;							\
		p = b + sizeof(b) - 1;						\
		*(p--) = '\0';							\
		ptmp = p;							\
		tmpnum = num;							\
		if (base != 16) {						\
			while (tmpnum) {					\
				divnum = div(tmpnum, base);			\
				*(p--) = "0123456789abcdef"[divnum.mod]; 	\
				tmpnum = divnum.div;				\
				column--;					\
			}							\
		} else {							\
			while (tmpnum) {					\
				*(p--) = "0123456789abcdef"[tmpnum & 0x0f]; 	\
				tmpnum >>= 4;					\
				column--;					\
			}							\
		}								\
		if (p == ptmp) {						\
			*(p--) = '0';						\
		}								\
		for (; column > 0; column--) {					\
			*(p--) = (flag) ? '0' : ' ';				\
		}								\
		p++;								\
	} while (0)

int
vsnprintf(char *buf, int n, const char *format, va_list args)
{
        int num;
        int i, j, k;
        char *a, tmp[16];
	int column, flag;

        for (i = j = 0; j < n && format[i] != '\0';) {
                if (format[i] != '%') {
                        buf[j++] = format[i++];
                        continue;
                }
                i++;
		column = -1;
		flag = 0;
		while (format[i] >= '0' && format[i] <= '9') {
			if (column == -1) {
				if (format[i] == '0') {
					flag = 1;
				}
				column = format[i++] - '0';
			} else {
				column *= 10;
				column += format[i++] - '0';
			}
		}
                switch(format[i++]) {
                case 'c':
                        num = va_arg(args, int);
                        buf[j++] = (char)(num & 0x0ff);
                        break;
                case 'u':
                        num = va_arg(args, int);
			parce_num(a, tmp, num, 10, column, flag);
			goto output_buf;
                case 'x':
                        num = va_arg(args, int);
			parce_num(a, tmp, num, 16, column, flag);
			goto output_buf;
                case 's':
                        a = va_arg(args, char *);
		output_buf:
                        for (k = 0; a[k] != '\0';) {
                                buf[j++] = a[k++];
                        }
                        break;
                default:
                        break;
                }
        }
        buf[j] = '\0';
        return j;
}

int
snprintf(char *buf, int n, char *format, ...)
{
        va_list args;
        int ret;

        va_start(args, format);
        ret = vsnprintf(buf, n, format, args);
        va_end(args);

        return ret;
}

char *
hex2dec(unsigned char num)
{
	int i;
	static char val[3];
	val[0] = '0';
	val[1] = '0';
	val[2] = '0';

	if (num & 0x01) {
		val[0] += 1;
	}
	if (num & 0x02) {
		val[0] += 2;
	}
	if (num & 0x04) {
		val[0] += 4;
	}
	if (num & 0x08) {
		val[0] += 8;
	}
	if (num & 0x10) {
		val[1] += 1;
		val[0] += 6;
	}
	if (num & 0x20) {
		val[1] += 3;
		val[0] += 2;
	}
	if (num & 0x40) {
		val[0] += 6;
		val[0] += 4;
	}
	if (num & 0x80) {
		val[2] += 1;
		val[1] += 2;
		val[0] += 8;
	}

	for (i = 0; i < 3; i++) {
		while (!(val[i] >= '0' && val[i] <= '9')) {
			val[i]-=0x0a;
			val[i + 1]++;
		}
	}

	return val;
}
