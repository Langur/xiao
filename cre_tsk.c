/**
  cre_tsk.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <kernel_id.h>
#include <service_call.h>
#include <task.h>
#include <lib.h>

extern T_TCB task_control_blocks[];

ER
cre_tsk_body(ID tskid, T_CTSK *pk_ctsk)
{
	extern UW _task_stack;
	static UW *task_stack = &_task_stack;
	UW *p_task_stack;
	SIZE len;
	int idx = TSKID_IDX(tskid);
	
	task_control_blocks[idx].next  = NULL;
	task_control_blocks[idx].bpri  = pk_ctsk->itskpri;
	task_control_blocks[idx].npri  = 0;

	task_control_blocks[idx].state = TA_TSK_STAT_DORMANT;
	task_control_blocks[idx].task  = pk_ctsk->task;
	task_control_blocks[idx].exinf = pk_ctsk->exinf;

	len = pk_ctsk->stksz / sizeof(UW);
	for (p_task_stack = task_stack; len > 0; len--) {
		*(p_task_stack++) = 0;
	}
	task_stack += pk_ctsk->stksz / sizeof(pk_ctsk->stksz);

	task_control_blocks[idx].stk = task_stack;
	task_control_blocks[idx].stksz = pk_ctsk->stksz;

	if ((pk_ctsk->tskatr & TA_ACT) > 0) {
		act_tsk_common(tskid, 0);
	}
	
	return E_OK;
}
