/**
  ext_tsk.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <service_call.h>
#include <task.h>

extern T_TCB *task_running;

void
ext_tsk(void)
{
	U_SPRM prm;

	prm.ext_tsk.tcb   = task_running;

	service_call(SCID_EXT_TSK, &prm);
	
	return;
}

void
ext_tsk_body(VP tcbp)
{
	task_remove_queue((T_TCB *)tcbp);
	task_terminate();
}
