/**
  act_tsk.c
  
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
	switch (id) {
	case SCID_ACT_TSK:
		prmp->act_tsk.ret = act_tsk_body(prmp->act_tsk.tskid);
		break;
	case SCID_IACT_TSK:
		prmp->iact_tsk.ret = iact_tsk_body(prmp->iact_tsk.tskid);
		break;
	case SCID_EXT_TSK:
		ext_tsk_body(prmp->ext_tsk.tcb);
		break;
	default:
		break;
	}
}
