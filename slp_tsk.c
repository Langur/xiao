/**
  slp_tsk.c
  
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

extern T_TCB task_control_blocks[];
extern T_TCB *task_running;

#ifdef TFN_SLP_TSK
ER
slp_tsk(void)
{
	U_SPRM prm;

	prm.slp_tsk.tskid = task_running->tskid;

	service_call(SCID_SLP_TSK, &prm);
	
	return prm.slp_tsk.ret;
}

ER
slp_tsk_body(ID tskid)
{
	ER ret;
	int idx = TSKID_IDX(tskid);

	ret = task_change_state(&(task_control_blocks[idx]), TA_TSK_STAT_WAITING);
        task_reschedule();

	return ret;
}
#endif /* TFN_SLP_TSK */
