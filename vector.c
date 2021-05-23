/**
  vecter.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

extern unsigned long _main_stack[];

void reset_entry(void);
void handler_nmi(void);
void handler_hardfault(void);
void handler_common(void);
void handler_svc(void);
void handler_pendsv(void);

const void *vector[] __attribute__ ((section(".romvector"))) =
{
	_main_stack,		//  0: MSP
	reset_entry + 1,	//  1: Reset
	handler_nmi + 1,	//  2: NMI
	handler_hardfault + 1,	//  3: HardFault
	handler_common + 1,	//  4: reserved
	handler_common + 1,	//  5: reserved
	handler_common + 1,	//  6: reserved
	handler_common + 1,	//  7: reserved
	handler_common + 1,	//  8: reserved
	handler_common + 1,	//  9: reserved
	handler_common + 1,	// 10: reserved
	handler_svc + 1,	// 11: SVC
	handler_common + 1,	// 12: reserved
	handler_common + 1,	// 13: reserved
	handler_pendsv + 1,	// 14: PendSV
	handler_common + 1,	// 15: SysTick
	handler_common + 1,	// 16: External Interrupt PIO0_0
	handler_common + 1,	// 17: External Interrupt PIO0_1
	handler_common + 1,	// 18: External Interrupt PIO0_2
	handler_common + 1,	// 19: External Interrupt PIO0_3
	handler_common + 1,	// 20: External Interrupt PIO0_4
	handler_common + 1,	// 21: External Interrupt PIO0_5
	handler_common + 1,	// 22: External Interrupt PIO0_6
	handler_common + 1,	// 23: External Interrupt PIO0_7
	handler_common + 1,	// 24: External Interrupt PIO0_8
	handler_common + 1,	// 25: External Interrupt PIO0_9
	handler_common + 1,	// 26: External Interrupt PIO0_10
	handler_common + 1,	// 27: External Interrupt PIO0_11
	handler_common + 1,	// 28: External Interrupt PIO1_10
	handler_common + 1,	// 29: External Interrupt C_CAN
	handler_common + 1,	// 30: External Interrupt SPC/SSP1
	handler_common + 1,	// 31: External Interrupt I2C0
	handler_common + 1,	// 32: External Interrupt
	handler_common + 1,	// 33: External Interrupt
	handler_common + 1,	// 34: External Interrupt
	handler_common + 1,	// 35: External Interrupt
	handler_common + 1,	// 36: External Interrupt
	handler_common + 1,	// 37: External Interrupt
	handler_common + 1,	// 38: External Interrupt
	handler_common + 1,	// 39: External Interrupt
	handler_common + 1,	// 40: External Interrupt
	handler_common + 1,	// 41: External Interrupt
	handler_common + 1,	// 42: External Interrupt
	handler_common + 1,	// 43: External Interrupt
	handler_common + 1,	// 44: External Interrupt
	handler_common + 1,	// 45: External Interrupt
	handler_common + 1,	// 46: External Interrupt
	handler_common + 1,	// 47: External Interrupt
};
