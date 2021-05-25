/**
  wup_tsk.c
  
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
wup_tsk(ID tskid)
{
	U_SPRM prm;

	prm.wup_tsk.ret   = E_OK;
	prm.wup_tsk.tskid = tskid;

	if (tskid != TSK_SELF) {
		prm.wup_tsk.tskid = tskid;
	} else {
		if (get_tid(&(prm.wup_tsk.tskid)) != E_OK) {
			return E_ID;
		}
	}

	service_call(SCID_WUP_TSK, &prm);
	
	return prm.wup_tsk.ret;
}

ER
wup_tsk_body(ID tskid)
{
	ER ret;
	UW flg = 0;

	TA_SCF_BIT_ENABLE(flg, TA_SCF_CALL_FROM_TASK_CONTEXT);
	
	ret = wup_tsk_common(tskid, flg);

	return ret;
}

ER
iwup_tsk(ID tskid)
{
	U_SPRM prm;

	if (tskid == TSK_SELF) {
		return E_ID;
	}

	prm.iwup_tsk.ret   = E_OK;
	prm.iwup_tsk.tskid = tskid;

	iservice_call(SCID_IWUP_TSK, &prm);

	return prm.iwup_tsk.ret;
}

ER
iwup_tsk_body(ID tskid)
{
	ER ret;
	UW flg = 0;

	TA_SCF_BIT_DISABLE(flg, TA_SCF_CALL_FROM_TASK_CONTEXT);
	
	ret = wup_tsk_common(tskid, flg);

	return ret;
}

ER
wup_tsk_common(ID tskid, UW flg)
{
	ER ret;
	int idx;

	if (tskid == TSK_SELF &&
	    (flg & TA_SCF_CALL_FROM_TASK_CONTEXT) == 0) {
		return E_ID;
	}

	idx = TSKID_IDX(tskid);

	if (task_control_blocks[idx].state == TA_TSK_STAT_WAITING) {
		ret = task_change_state(&(task_control_blocks[idx]), TA_TSK_STAT_READY);
	} else if (task_control_blocks[idx].state == TA_TSK_STAT_WAITING_SUSPENDED) {
		ret = task_change_state(&(task_control_blocks[idx]), TA_TSK_STAT_SUSPENDED);
	} else if (task_control_blocks[idx].state == TA_TSK_STAT_DORMANT) {
		ret = E_OBJ;
	} else {
		ret = E_OBJ;
	}

	task_reschedule();

	return ret;
}
