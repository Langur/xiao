/**
  interrupt.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <interrupt.h>
#include <task.h>
#include <service_call.h>
#include <lib.h>

#define	SystemControlSpace	0xE000E000
#define	NVIC_ISER		(SystemControlSpace + 0x0100)
#define	NVIC_ICER		(SystemControlSpace + 0x0180)
#define	NVIC_ICPR		(SystemControlSpace + 0x0280)

FP interrupt_handlers[TMAX_INHNO];

void
interrupt_init(void)
{
	/* 全ての割り込みを禁止 */
	//*(volatile uint32_t *)NVIC_ICER = ~0;
	/* 全ての割り込み要因をクリア */
	//*(volatile uint32_t *)NVIC_ICPR = ~0;

	memset(interrupt_handlers, 0, sizeof(FP) * TMAX_INHNO);
}

void
interrupt_handler_execute(INHNO inhno)
{
	if (interrupt_handlers[inhno]) {
		interrupt_handlers[inhno]();
	}
	
	task_reschedule();
}

void
interrupt_disable(INHNO inhno)
{
	inhno -= (inhno + 16);
	*(volatile unsigned long *)NVIC_ICER = 1 << (inhno & 0x1f);
}
