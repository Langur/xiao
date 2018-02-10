/**
  defines.h
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef _DEFINES_H_INCLUDED_
#define _DEFINES_H_INCLUDED_

#define NULL ((void *)0)
#define SERIAL_DEFAULT_DEVICE 0
#define I2C_DEFAULT_DEVICE 0

#define DEFAULT_TASKNAME_SIZE 8
#define DEFAULT_TASKNAME_BUFFER_SIZE (DEFAULT_TASKNAME_SIZE + 1)
#define DEFAULT_BUFFER_SIZE 64

#define	IOCON_PIO0_3		((volatile unsigned long *)0x4004402c)
#define	IOCON_PIO0_7		((volatile unsigned long *)0x40044050)
#define	GPIO0_DIR		((volatile unsigned long *)0x50008000)
#define SYSMEMREMAP		((volatile unsigned long *)0x40048000)
#define PRESETCTRL		((volatile unsigned long *)0x40048004)
#define SYSAHBCLKCTRL		((volatile unsigned long *)0x40048080)
#define PDRUNCFG		((volatile unsigned long *)0x40048238)
#define ICSR			((volatile unsigned long *)0xe000ed04)

#define PDRUNCFG_MASK_ADC_PD	(0x00000010)

#define PENDSVSET		(1 << 28)
#define PENDSVCLR		(1 << 27)
#define AHB_IOCON		(1 << 16)
#define AHB_ADC			(1 << 13)
#define AHB_CT32B1		(1 << 10)
#define AHB_CT32B0		(1 << 9)
#define AHB_CT16B1		(1 << 8)
#define AHB_CT16B0		(1 << 7)
#define AHB_I2C			(1 << 5)

#define SSP0_RST_N		(1 << 0)
#define I2C_RST_N		(1 << 1)
#define SSP1_RST_N		(1 << 2)
#define CAN_RST_N		(1 << 3)

#define I2C0CONSET_AA		(1 << 2)
#define I2C0CONSET_SI		(1 << 3)
#define I2C0CONSET_STO		(1 << 4)
#define I2C0CONSET_STA		(1 << 5)
#define I2C0CONSET_I2EN		(1 << 6)

#define I2C0CONCLR_AAC 	      	(1 << 3)
#define I2C0CONCLR_SIC 	      	(1 << 3)
#define I2C0CONCLR_STAC		(1 << 5)

typedef unsigned long  task_id;
typedef int (*taskfunc)(int argc, char *argv[]);
typedef void (*softhandler)(void);

typedef unsigned long softvec_type;

typedef enum {
	TIMER_ID_COMMAND,
	TIMER_ID_GENERAL,
	TIMER_ID_NUM
} timer_id;

typedef enum {
	MSGBOX_ID_CONSINPUT = 0,
	MSGBOX_ID_CONSOUTPUT,
	MSGBOX_ID_TIMER,
	MSGBOX_ID_NUM = MSGBOX_ID_TIMER + TIMER_ID_NUM,
} msgbox_id;
#endif
