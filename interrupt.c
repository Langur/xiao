/**
  interrupt.c
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include "defines.h"
#include "interrupt.h"

#define	SystemControlSpace	0xe000e000
#define	NVIC_ISER		(SystemControlSpace + 0x0100)
#define	NVIC_ICER		(SystemControlSpace + 0x0180)
#define	NVIC_ICPR		(SystemControlSpace + 0x0280)

void
interrupt_init(void)
{
	/* 全ての割り込みを禁止 */
	*(volatile unsigned long *)NVIC_ICER = ~0;
	/* 全ての割り込み要因をクリア */
	*(volatile unsigned long *)NVIC_ICPR = ~0;
}

void
interrupt_enable(softvec_type type)
{
	type -= SOFTVEC_TYPE_IRQ(0);
	*(volatile unsigned long *)NVIC_ISER = 1 << (type & 0x1f);
}

void
interrupt_disable_all()
{
	*(volatile unsigned long *)NVIC_ICER = ~0;
}

void
interrupt_disable(softvec_type type)
{
	type -= SOFTVEC_TYPE_IRQ(0);
	*(volatile unsigned long *)NVIC_ICER = 1 << (type & 0x1f);
}

void
interrupt_clear(softvec_type type)
{
	type -= SOFTVEC_TYPE_IRQ(0);
	*(volatile unsigned long *)NVIC_ICPR = 1 << (type & 0x1f);
}

