/**
  service_call.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <service_call.h>
#include <task.h>
#include <lib.h>

static void service_call_proc(enum service_call_id id, U_SPRMP prmp);
static void iservice_call_proc(enum service_call_id id, U_SPRMP prmp);
static void service_call_body(enum service_call_id id, U_SPRMP prmp);

void
service_call(enum service_call_id id, U_SPRMP prmp)
{
	if (task_current) {
		task_current->service_call.id  = id;
		task_current->service_call.prmp = prmp;
	} else {
		init_service_call.id = id;
		init_service_call.prmp = prmp;
	}

	asm volatile ("swi #0");
}

void
isr_service_call(void)
{
	if (task_current) {
		service_call_proc(task_current->service_call.id, task_current->service_call.prmp);
	} else {
		service_call_proc(init_service_call.id, init_service_call.prmp);
	}
}

static void
service_call_proc(enum service_call_id id, U_SPRMP prmp)
{
	service_call_body(id, prmp);
}

void
iservice_call(enum service_call_id id, U_SPRMP prmp)
{
	iservice_call_proc(id, prmp);
}

static void
iservice_call_proc(enum service_call_id id, U_SPRMP prmp)
{
	service_call_body(id, prmp);
}

static void
service_call_body(enum service_call_id id, U_SPRMP prmp)
{
	if (id == SCID_ACT_TSK) {
		prmp->act_tsk.ret = act_tsk_body(prmp->act_tsk.tskid);
	}
	else if (id == SCID_IACT_TSK) {
		prmp->iact_tsk.ret = iact_tsk_body(prmp->iact_tsk.tskid);
	}
	else if (id == SCID_EXT_TSK) {
		ext_tsk_body(prmp->ext_tsk.tcb);
	}
#ifdef TFN_GET_TID
	else if (id == SCID_GET_TID) {
		prmp->get_tid.ret = get_tid_body(&(prmp->get_tid.tskid));
	}
#endif /* TFN_GET_TID */
#ifdef TFN_IGET_TID
	else if (id == SCID_IGET_TID) {
		prmp->iget_tid.ret = iget_tid_body(&(prmp->get_tid.tskid));
	}
#endif /* TFN_IGET_TID */
#ifdef TFN_GET_PRI
	else if (id == SCID_GET_PRI) {
		prmp->get_pri.ret = get_pri_body(prmp->get_pri.tskid, &(prmp->get_pri.tskpri));
	}
#endif /* TFN_GET_PRI */
#ifdef TFN_ROT_RDQ
	else if (id == SCID_ROT_RDQ) {
		prmp->rot_rdq.ret = rot_rdq_body(prmp->rot_rdq.tskpri);
	}
#endif /* TFN_ROT_RDQ */
#ifdef TFN_IROT_RDQ
	else if (id == SCID_IROT_RDQ) {
		prmp->irot_rdq.ret = irot_rdq_body(prmp->irot_rdq.tskpri);
	}
#endif /* TFN_IROT_RDQ */
#ifdef TFN_SLP_TSK
	else if (id == SCID_SLP_TSK) {
		prmp->slp_tsk.ret = slp_tsk_body(prmp->slp_tsk.tskid);
	}
#endif /* TFN_SLP_TSK */
#ifdef TFN_TSLP_TSK
	else if (id == SCID_TSLP_TSK) {
		prmp->tslp_tsk.ret = tslp_tsk_body(prmp->slp_tsk->tskid, prmp->slp_tsk->tmout);
	}
#endif /* TFN_TSLP_TSK */
#ifdef TFN_WUP_TSK
	else if (id == SCID_WUP_TSK) {
		prmp->wup_tsk.ret = wup_tsk_body(prmp->wup_tsk.tskid);
	}
#endif /* TFN_WUP_TSK */
#ifdef TFN_IWUP_TSK
	else if (id == SCID_IWUP_TSK) {
		prmp->iwup_tsk.ret = iwup_tsk_body(prmp->iwup_tsk.tskid);
	}
#endif /* TFN_IWUP_TSK */
	else {
		;
	}
}
