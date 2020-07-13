/**
  vecter.c
  
  Copyright (c) 2013-2020 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

extern unsigned long _main_stack[];

void reset_entry(void);

const void *vector[] __attribute__ ((section(".romvector"))) =
{
	_main_stack,		// MSP
	reset_entry + 1,	//  1: Reset
	reset_entry + 1,	//  2: NMI
	reset_entry + 1,	//  3: HardFault
	reset_entry + 1,	//  4: reserved
	reset_entry + 1,	//  5: reserved
	reset_entry + 1,	//  6: reserved
	reset_entry + 1,	//  7: reserved
	reset_entry + 1,	//  8: reserved
	reset_entry + 1,	//  9: reserved
	reset_entry + 1,	// 10: reserved
	reset_entry + 1,	// 11: SVC
	reset_entry + 1,	// 12: reserved
	reset_entry + 1,	// 13: reserved
	reset_entry + 1,	// 14: PendSV
	reset_entry + 1,	// 15: SysTick
	reset_entry + 1,	// 16: External Interrupt PIO0_0
	reset_entry + 1,	// 17: External Interrupt PIO0_1
	reset_entry + 1,	// 18: External Interrupt PIO0_2
	reset_entry + 1,	// 19: External Interrupt PIO0_3
	reset_entry + 1,	// 20: External Interrupt PIO0_4
	reset_entry + 1,	// 21: External Interrupt PIO0_5
	reset_entry + 1,	// 22: External Interrupt PIO0_6
	reset_entry + 1,	// 23: External Interrupt PIO0_7
	reset_entry + 1,	// 24: External Interrupt PIO0_8
	reset_entry + 1,	// 25: External Interrupt PIO0_9
	reset_entry + 1,	// 26: External Interrupt PIO0_10
	reset_entry + 1,	// 27: External Interrupt PIO0_11
	reset_entry + 1,	// 28: External Interrupt PIO1_10
	reset_entry + 1,	// 29: External Interrupt C_CAN
	reset_entry + 1,	// 30: External Interrupt SPC/SSP1
	reset_entry + 1,	// 31: External Interrupt I2C0
	reset_entry + 1,	// 32: External Interrupt
	reset_entry + 1,	// 33: External Interrupt
	reset_entry + 1,	// 34: External Interrupt
	reset_entry + 1,	// 35: External Interrupt
	reset_entry + 1,	// 36: External Interrupt
	reset_entry + 1,	// 37: External Interrupt
	reset_entry + 1,	// 38: External Interrupt
	reset_entry + 1,	// 39: External Interrupt
	reset_entry + 1,	// 40: External Interrupt
	reset_entry + 1,	// 41: External Interrupt
	reset_entry + 1,	// 42: External Interrupt
	reset_entry + 1,	// 43: External Interrupt
	reset_entry + 1,	// 44: External Interrupt
	reset_entry + 1,	// 45: External Interrupt
	reset_entry + 1,	// 46: External Interrupt
	reset_entry + 1,	// 47: External Interrupt
	reset_entry + 1,	// 48: External Interrupt
	reset_entry + 1,	// 49: External Interrupt
	reset_entry + 1,	// 50: External Interrupt
	reset_entry + 1,	// 51: External Interrupt
	reset_entry + 1,	// 52: External Interrupt
	reset_entry + 1,	// 53: External Interrupt
	reset_entry + 1,	// 54: External Interrupt
	reset_entry + 1,	// 55: External Interrupt
	reset_entry + 1,	// 56: External Interrupt
	reset_entry + 1,	// 57: External Interrupt
	reset_entry + 1,	// 58: External Interrupt
	reset_entry + 1,	// 59: External Interrupt
	reset_entry + 1,	// 60: External Interrupt
	reset_entry + 1,	// 61: External Interrupt
	reset_entry + 1,	// 62: External Interrupt
	reset_entry + 1,	// 63: External Interrupt
};
