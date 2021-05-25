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
#ifdef TFN_GET_TID
	SCID_GET_TID,
#endif /* TFN_GET_TID */
#ifdef TFN_IGET_TID
	SCID_IGET_TID,
#endif /* TFN_IGET_TID */
#ifdef TFN_GET_PRI
	SCID_GET_PRI,
#endif /* TFN_GET_PRI */
#ifdef TFN_ROT_RDQ
	SCID_ROT_RDQ,
#endif /* TFN_ROT_RDQ */
#ifdef TFN_IROT_RDQ
	SCID_IROT_RDQ,
#endif /* TFN_IROT_RDQ */
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

#ifdef TFN_GET_TID
	struct {
		ER ret;
		ID tskid;
	} get_tid;
#endif /* TFN_GET_TID */

#ifdef TFN_IGET_TID
	struct {
		ER ret;
		ID tskid;
	} iget_tid;
#endif /* TFN_IGET_TID */

#ifdef TFN_GET_PRI
	struct {
		ER ret;
		ID tskid;
		PRI tskpri;
	} get_pri;
#endif /* TFN_GET_PRI */

#ifdef TFN_ROT_RDQ
	struct {
		ER ret;
		PRI tskpri;
	} rot_rdq;
#endif /* TFN_ROT_RDQ */

#ifdef TFN_IROT_RDQ
	struct {
		ER ret;
		PRI tskpri;
	} irot_rdq;
#endif /* TFN_IROT_RDQ */
} U_SPRM;

typedef U_SPRM			*U_SPRMP;

/* FLAG定義 */
#define TA_SCF_CALL_DISPATCH		(1 << 0)
#define TA_SCF_CALL_FROM_TASK_CONTEXT	(1 << 1)
#define TA_SCF_BIT_ENABLE(flg, bit)	(flg |= bit)
#define TA_SCF_BIT_DISABLE(flg, bit)	(flg &= ((~0) ^ bit))

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
#ifdef TFN_GET_TID
ER get_tid_body(ID *p_tskid);
#endif /* TFN_GET_TID */
#ifdef TFN_IGET_TID
ER iget_tid_body(ID *p_tskid);
#endif /* TFN_IGET_TID */
#if defined TFN_GET_TID || defined TFN_IGET_TID
ER get_tid_common(ID *p_tskid);
#endif /* TFN_GET_TID || TFN_IGET_TID */
#ifdef TFN_GET_PRI
ER get_pri_body(ID tskid, PRI *p_tskpri);
#endif /* TFN_GET_PRI */
#ifdef TFN_ROT_RDQ
ER rot_rdq_body(PRI tskpri);
#endif /* TFN_ROT_RDQ */
#ifdef TFN_IROT_RDQ
ER irot_rdq_body(PRI tskpri);
#endif /* TFN_IROT_RDQ */
#if defined TFN_ROT_RDQ || defined TFN_IROT_RDQ
ER rot_rdq_common(PRI tskpri, UW flg);
#endif /* TFN_ROT_RDQ || TFN_IROT_RDQ */
#endif /* __SERVICE_CALL_H__ */
