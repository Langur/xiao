/**
  main.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <kernel_id.h>
#include <service_call.h>
#include <interrupt.h>
#include <serial.h>
#include <task.h>
#include <timestamp.h>
#include <lib.h>

void
init(void)
{
	int i;

	interrupt_init();
	task_init();
	serial_init(SERIAL_DEFAULT_DEVICE);

	for (i = 0; i < TMAX_DEF_INH_PRM; i++) {
		def_inh_body(def_inh_prms[i].inhno, (T_DINH *)&(def_inh_prms[i].t_dinh));
	}

	for (i = 0; i < TMAX_CRE_TSK_PRM; i++) {
		cre_tsk_body(cre_tsk_prms[i].tskid, (T_CTSK *)&(cre_tsk_prms[i].t_ctsk));
	}
}

int
main(void)
{
	init();
	puts("xiao ");
	putc('(');
	puts((char *)timestamp);
	putc(')');
	puts("\r\n");

	task_schedule();
	task_dispatch();
	asm volatile ("wfi");

	return 0;
}
