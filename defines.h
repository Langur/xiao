/**
  defines.h
  
  Copyright (c) 2013-2020 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

#define NULL ((void *)0)

#define SYSMEMREMAP		((volatile unsigned long *)0x40048000)
#define SYSAHBCLKCTRL		((volatile unsigned long *)0x40048080)

#define AHB_IOCON		(1 << 16)

#endif /* __DEFINES_H__ */
