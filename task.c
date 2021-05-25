/**
  task.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <kernel_id.h>
#include <task.h>
#include <lib.h>

T_TCB task_control_blocks[TMAX_TSKID];

typedef struct T_TRQ {
	T_TCB *head;
	T_TCB *tail;
} T_TRQ;


static T_TRQ task_ready_queue[TMAX_TPRI];

T_TCB *task_running = NULL;
T_TCB *task_current = NULL;
T_SERVICE_CALL init_service_call = {0, NULL};

const UW task_state_offset = (UW)(&((T_TCB *)0)->state);
const UW task_context_offset = (UW)(&((T_TCB *)0)->ctx);

static ER task_push_queue(T_TCB *tcbp);
static ER task_remove_queue(T_TCB *tcbp);

void
task_init(void)
{
	memset(task_ready_queue, 0, sizeof(T_TRQ) * TMAX_TPRI);
}

void
task_execute(T_TCB *tcbp)
{
	tcbp->task(tcbp->exinf);
	task_remove_queue(tcbp);
	task_reschedule();
}

void
task_terminate(void)
{
	while (1) {
		asm volatile ("wfi");
	}
}

ER
task_change_state(T_TCB *tcbp, STAT state)
{
	ER ret = E_OK;

	if (tcbp == NULL) {
		return E_NOEXS;
	}

	if (tcbp->state == TA_TSK_STAT_READY &&
	    state != TA_TSK_STAT_READY) {
		ret = task_remove_queue(tcbp);
	} else if ((tcbp->state == TA_TSK_STAT_DORMANT &&
		    state == TA_TSK_STAT_READY) ||
		   (tcbp->state == TA_TSK_STAT_WAITING &&
		    state == TA_TSK_STAT_READY) ||
		   (tcbp->state == TA_TSK_STAT_SUSPENDED &&
		    state == TA_TSK_STAT_READY)) {
		ret = task_push_queue(tcbp);
	} else {
		;
	}

	tcbp->state = state;

	return ret;
}

static ER
task_push_queue(T_TCB *tcbp)
{
	UW idx;
	
	if (tcbp == NULL) {
		return E_NOEXS;
	}
	
	if (tcbp->state == TA_TSK_STAT_READY) {
		return E_CTX;
	}

	idx = TPRI_IDX(tcbp->npri);
	if (task_ready_queue[idx].tail) {
		task_ready_queue[idx].tail->next = tcbp;
	}
	if (!task_ready_queue[idx].head) {
		task_ready_queue[idx].head = tcbp;
	}
	task_ready_queue[idx].tail = tcbp;

	return E_OK;
}

static ER
task_remove_queue(T_TCB *tcbp)
{
	UW idx;
	T_TCB *curr, *prev;

	if (tcbp == NULL) {
		return E_NOEXS;
	}
	
	if (tcbp->state != TA_TSK_STAT_READY) {
		return E_CTX;
	}

	idx = TPRI_IDX(tcbp->npri);
	prev = NULL;
	for (curr = task_ready_queue[idx].head; curr; prev = curr, curr = curr->next) {
		if (curr == tcbp) {
			if (prev) {
				prev->next = curr->next;
				curr->next = NULL;
			} else {
				task_ready_queue[idx].head = curr->next;
				curr->next = NULL;
			}

			if (task_ready_queue[idx].tail == curr) {
				task_ready_queue[idx].tail = prev;
			}

			break;
		}
	}
	
	return E_OK;
}

ER
task_rotate_queue(PRI tskpri)
{
	UW idx;
	T_TCB *tcbp;

	idx = TPRI_IDX(tskpri);

	if (task_ready_queue[idx].head == NULL ||
	    task_ready_queue[idx].tail == NULL) {
		return E_PAR;
	}

	if (task_ready_queue[idx].head != task_ready_queue[idx].tail) {
		tcbp = task_ready_queue[idx].head;
		task_ready_queue[idx].head = task_ready_queue[idx].head->next;
		task_ready_queue[idx].tail->next = tcbp;
		task_ready_queue[idx].tail = tcbp;
		tcbp->next = NULL;
	}

	return E_OK;
}

void
task_schedule(void)
{
	int i;

	for (i = 0; i < TMAX_TPRI; i++) {
		if (task_ready_queue[i].head) {
			break;
		}
	}
	if (i == TMAX_TPRI) {
		while (1) {
			asm volatile ("wfi");
		}
	}

	task_current = task_ready_queue[i].head;
}

void
task_reschedule(void)
{
	task_schedule();
	if (task_current != NULL) {
		task_dispatch();
	}
}

void
task_dispatch(void)
{
	asm("cpsid	i\n");
	*ICSR |= PENDSVSET;
	asm("cpsie	i\n"
	    "bx		lr\n");
}
