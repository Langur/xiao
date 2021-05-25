/**
  task.h
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __TASK_H__
#define __TASK_H__

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <service_call.h>

#define TA_TSK_STAT_RUNNING		(1)
#define TA_TSK_STAT_READY		(2)
#define TA_TSK_STAT_DORMANT		(3)
#define TA_TSK_STAT_WAITING		(4)
#define TA_TSK_STAT_SUSPENDED		(5)
#define TA_TSK_STAT_WAITING_SUSPENDED	(6)

typedef struct t_tctx {
	UW sp;
} T_TCTX;

typedef struct t_service_call {
	enum service_call_id id;
	U_SPRMP prmp;
} T_SERVICE_CALL;

typedef struct t_tcb {
	struct t_tcb *next;
	ID tskid;
	PRI bpri;
	PRI npri;
	VP stk;
	SIZE stksz;
	STAT state;
  
	FP task;
	VP_INT exinf;

	T_SERVICE_CALL service_call;

	T_TCTX ctx;
} T_TCB;

typedef struct t_cre_tsk_prm {
	ID tskid;
	T_CTSK t_ctsk;
} T_CRE_TSK_PRM;

extern T_TCB *task_current;
extern T_SERVICE_CALL init_service_call;

extern T_TCB task_control_blocks[];
extern const T_CRE_TSK_PRM cre_tsk_prms[];

void task_init(void);

void task_execute(T_TCB *tcbp);
void task_terminate(void);
void task_get_current(void);
ER task_change_state(T_TCB *tcbp, STAT state);
ER task_rotate_queue(PRI tskpri);
void task_schedule(void);
void task_reschedule(void);
void task_dispatch(void);

#endif /* __TASK_H__ */
