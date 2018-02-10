/**
  timer.c
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include "defines.h"
#include "os.h"
#include "timer.h"
#include "lib.h"

volatile unsigned long ticks = 0;
static volatile lpc11xx_systick_register_t *regs = LPC11XX_SYSTICK;
volatile timer_t timer[TIMER_ID_NUM];

void
systick_init(void)
{
	volatile lpc11xx_systick_register_t *systick = regs;
	
	systick->rvr = 0x0176F; /* 1ms */
//	systick->rvr = 0x0EA5F; /* 10ms */
	ticks = 0;
	systick->cvr = 0;
	systick->csr = 0x07;
}

void
systick_handler(void)
{
	ticks++;
}

void
ct_init(int channel)
{
	*SYSAHBCLKCTRL |= (AHB_CT16B0 << channel);
}

void
init_wait(unsigned long msec)
{
	int end;

	end = ticks + msec;
	while (ticks <= end);
}

void
wait(int timer_id, unsigned long msec)
{
	int size;
	char *p;
	
	timer[timer_id].end = ticks + msec;
	timer[timer_id].sw = 1;
	receive_message(MSGBOX_ID_TIMER + timer_id, &size, &p);
}

void
init_timer(int id)
{
	timer[id].end = ticks + 200;
	timer[id].sw = 1;
}
