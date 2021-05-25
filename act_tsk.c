/**
  act_tsk.c
  
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
act_tsk(ID tskid)
{
	U_SPRM prm;

	prm.act_tsk.ret   = E_OK;
	prm.act_tsk.tskid = tskid;

	service_call(SCID_ACT_TSK, &prm);
	
	return prm.act_tsk.ret;
}

ER
act_tsk_body(ID tskid)
{
	ER ret;
	UW flg = 0;

	TA_SCF_BIT_ENABLE(flg, TA_SCF_CALL_DISPATCH);

	ret = act_tsk_common(tskid, flg);

	return ret;
}

ER
iact_tsk(ID tskid)
{
	U_SPRM prm;

	prm.iact_tsk.ret   = E_OK;
	prm.iact_tsk.tskid = tskid;

	iservice_call(SCID_IACT_TSK, &prm);

	return prm.iact_tsk.ret;
}

ER
iact_tsk_body(ID tskid)
{
	ER ret;
	UW flg = 0;

	TA_SCF_BIT_ENABLE(flg, TA_SCF_CALL_DISPATCH);
	
	ret = act_tsk_common(tskid, flg);

	return ret;
}

ER
act_tsk_common(ID tskid, UW flg)
{
	VP_INT *sp;
	int idx = TSKID_IDX(tskid);
	ER ret = E_OK;
	task_control_blocks[idx].npri = task_control_blocks[idx].bpri;
	
	/* stackの初期化 */
	sp = (VP_INT *)task_control_blocks[idx].stk;
	*(--sp) = (VP_INT)0x01000000;			/* EPSR.T=1 */
	*(--sp) = (VP_INT)task_execute;			/* R15(PC) */
	*(--sp) = (VP_INT)task_terminate;		/* R14(LR): タスク終了時の戻り先 */
	*(--sp) = 0;					/* R12 */
	*(--sp) = 0;					/* R3 */
	*(--sp) = 0;					/* R2 */
	*(--sp) = 0;					/* R1 */
	*(--sp) = (VP_INT)&task_control_blocks[idx];	/* R0 */
	*(--sp) = 0;					/* R7 */
	*(--sp) = 0;					/* R6 */
	*(--sp) = 0;					/* R5 */
	*(--sp) = 0;					/* R4 */
	*(--sp) = 0;					/* R11 */
	*(--sp) = 0;					/* R10 */
	*(--sp) = 0;					/* R9 */
	*(--sp) = 0;					/* R8 */

	task_control_blocks[idx].ctx.sp = (VP_INT)sp;

	/* readyqueueに繋ぐ */
	ret = task_change_state(&(task_control_blocks[idx]), TA_TSK_STAT_READY);

	if (flg & TA_SCF_CALL_DISPATCH) {
		/* dispatch */
		task_reschedule();
	}
	return ret;
}
