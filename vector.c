/**
  vecter.c
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include "handler.h"

extern unsigned long _main_stack[];

void reset_entry(void);

const void *vector[] __attribute__ ((section(".romvector"))) =
{
	_main_stack,		// MSP
	reset_entry + 1,	//  1: Reset
	handler_interrupt + 1,	//  2: NMI
	reset_entry + 1,	//  3: HardFault
	handler_interrupt + 1,	//  4: reserved
	handler_interrupt + 1,	//  5: reserved
	handler_interrupt + 1,	//  6: reserved
	handler_interrupt + 1,	//  7: reserved
	handler_interrupt + 1,	//  8: reserved
	handler_interrupt + 1,	//  9: reserved
	handler_interrupt + 1,	// 10: reserved
	handler_svc + 1,	// 11: SVC
	handler_interrupt + 1,	// 12: reserved
	handler_interrupt + 1,	// 13: reserved
	handler_pendsv + 1,	// 14: PendSV
	systick_handler + 1,	// 15: SysTick
	handler_interrupt + 1,	// 16: External Interrupt PIO0_0
	handler_interrupt + 1,	// 17: External Interrupt PIO0_1
	handler_interrupt + 1,	// 18: External Interrupt PIO0_2
	handler_interrupt + 1,	// 19: External Interrupt PIO0_3
	handler_interrupt + 1,	// 20: External Interrupt PIO0_4
	handler_interrupt + 1,	// 21: External Interrupt PIO0_5
	handler_interrupt + 1,	// 22: External Interrupt PIO0_6
	handler_interrupt + 1,	// 23: External Interrupt PIO0_7
	handler_interrupt + 1,	// 24: External Interrupt PIO0_8
	handler_interrupt + 1,	// 25: External Interrupt PIO0_9
	handler_interrupt + 1,	// 26: External Interrupt PIO0_10
	handler_interrupt + 1,	// 27: External Interrupt PIO0_11
	handler_interrupt + 1,	// 28: External Interrupt PIO1_10
	handler_interrupt + 1,	// 29: External Interrupt C_CAN
	handler_interrupt + 1,	// 30: External Interrupt SPC/SSP1
	handler_interrupt + 1,	// 31: External Interrupt I2C0
	handler_interrupt + 1,	// 32: External Interrupt
	handler_interrupt + 1,	// 33: External Interrupt
	handler_interrupt + 1,	// 34: External Interrupt
	handler_interrupt + 1,	// 35: External Interrupt
	handler_interrupt + 1,	// 36: External Interrupt
	handler_interrupt + 1,	// 37: External Interrupt
	handler_interrupt + 1,	// 38: External Interrupt
	handler_interrupt + 1,	// 39: External Interrupt
	handler_interrupt + 1,	// 40: External Interrupt
	handler_interrupt + 1,	// 41: External Interrupt
	handler_interrupt + 1,	// 42: External Interrupt
	handler_interrupt + 1,	// 43: External Interrupt
	handler_interrupt + 1,	// 44: External Interrupt
	handler_interrupt + 1,	// 45: External Interrupt
	handler_interrupt + 1,	// 46: External Interrupt
	handler_interrupt + 1,	// 47: External Interrupt
	handler_interrupt + 1,	// 48: External Interrupt
	handler_interrupt + 1,	// 49: External Interrupt
	handler_interrupt + 1,	// 50: External Interrupt
	handler_interrupt + 1,	// 51: External Interrupt
	handler_interrupt + 1,	// 52: External Interrupt
	handler_interrupt + 1,	// 53: External Interrupt
	handler_interrupt + 1,	// 54: External Interrupt
	handler_interrupt + 1,	// 55: External Interrupt
	handler_interrupt + 1,	// 56: External Interrupt
	handler_interrupt + 1,	// 57: External Interrupt
	handler_interrupt + 1,	// 58: External Interrupt
	handler_interrupt + 1,	// 59: External Interrupt
	handler_interrupt + 1,	// 60: External Interrupt
	handler_interrupt + 1,	// 61: External Interrupt
	handler_interrupt + 1,	// 62: External Interrupt
	handler_interrupt + 1,	// 63: External Interrupt
};
