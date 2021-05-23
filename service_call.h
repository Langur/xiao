/**
  service_call.h
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __SERVICE_CALL_H__
#define __SERVICE_CALL_H__

#include <defines.h>
#include <itron.h>
#include <kernel.h>

/*
 * サービスコール実装のための定義
 */
enum service_call_id {
	SCID_ACT_TSK = 0,
	SCID_IACT_TSK,
	SCID_EXT_TSK,
	SCID_NUM
};

typedef union {
	struct {
		ER ret;
		ID tskid;
	} act_tsk;

	struct {
		ER ret;
		ID tskid;
	} iact_tsk;

	struct {
		VP tcb;
	} ext_tsk;
} U_SPRM;

typedef U_SPRM			*U_SPRMP;

void service_call(enum service_call_id id, U_SPRMP prmp);
void iservice_call(enum service_call_id id, U_SPRMP prmp);
void isr_service_call(void);

/*
 * サービスコールの実体定義
 */
ER act_tsk_body(ID tskid);
ER iact_tsk_body(ID tskid);
ER act_tsk_common(ID tskid, UW flg);
ER def_inh_body(INHNO inhno, T_DINH *pk_dinh);
ER cre_tsk_body(ID tskid, T_CTSK *pk_ctsk);
void ext_tsk_body(VP tcb);

#endif /* __SERVICE_CALL_H__ */
