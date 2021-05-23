/**
  defines.h
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

#ifdef __INT8_TYPE__
typedef __INT8_TYPE__	int8_t;
#endif
#ifdef __INT16_TYPE__
typedef __INT16_TYPE__	int16_t;
#endif
#ifdef __INT32_TYPE__
typedef __INT32_TYPE__	int32_t;
#endif
#ifdef __INT64_TYPE__
typedef __INT64_TYPE__	int64_t;
#endif
#ifdef __UINT8_TYPE__
typedef __UINT8_TYPE__	uint8_t;
#endif
#ifdef __UINT16_TYPE__
typedef __UINT16_TYPE__	uint16_t;
#endif
#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__	uint32_t;
#endif
#ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__	uint64_t;
#endif

typedef signed int	int_t;
typedef unsigned int	uint_t;
typedef int_t		bool_t;
typedef signed long	intptr_t;
typedef unsigned long	uintptr_t;

#define SYSMEMREMAP		((volatile unsigned long *)0x40048000)
#define SYSAHBCLKCTRL		((volatile unsigned long *)0x40048080)
#define ICSR			((volatile unsigned long *)0xe000ed04)

#define PENDSVSET		(1 << 28)
#define PENDSVCLR		(1 << 27)
#define AHB_IOCON		(1 << 16)

#define SERIAL_DEFAULT_DEVICE 0

#endif /* __DEFINES_H__ */
