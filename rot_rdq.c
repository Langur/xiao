/**
  rot_rdq.c
  
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

#ifdef TFN_ROT_RDQ
ER
rot_rdq(PRI tskpri)
{
	U_SPRM prm;
	PRI pri = TPRI_SELF;
	ID tskid;
	int idx;

	if (tskpri != TPRI_SELF) {
		pri = tskpri;
	} else {
		if (get_tid(&tskid) != E_OK) {
			return E_PAR;
		}
		idx = TSKID_IDX(tskid);
		pri = task_control_blocks[idx].bpri;
	}

	prm.rot_rdq.ret   = E_OK;
	prm.rot_rdq.tskpri = pri;

	service_call(SCID_ROT_RDQ, &prm);
	
	return prm.rot_rdq.ret;
}

ER
rot_rdq_body(PRI tskpri)
{
        ER ret;
	UW flg = 0;

	TA_SCF_BIT_ENABLE(flg, TA_SCF_CALL_FROM_TASK_CONTEXT);

        ret = rot_rdq_common(tskpri, flg);

        return ret;
}
#endif /* TFN_ROT_RDQ */

#ifdef TFN_IROT_RDQ
ER
irot_rdq(PRI tskpri)
{
	U_SPRM prm;

	if (tskpri == TPRI_SELF) {
		return E_PAR;
	}

	prm.irot_rdq.ret   = E_OK;
	prm.irot_rdq.tskpri = tskpri;

	iservice_call(SCID_IROT_RDQ, &prm);
	
	return prm.irot_rdq.ret;
}

ER
irot_rdq_body(PRI tskpri)
{
        ER ret;
	UW flg = 0;

	TA_SCF_BIT_DISABLE(flg, TA_SCF_CALL_FROM_TASK_CONTEXT);

        ret = rot_rdq_common(tskpri, flg);

        return ret;
}
#endif /* TFN_IROT_RDQ */

#if defined TFN_ROT_RDQ || defined TFN_IROT_RDQ
ER
rot_rdq_common(PRI tskpri, UW flg)
{
	ER ret;

	if (tskpri == TPRI_SELF &&
	    (flg & TA_SCF_CALL_FROM_TASK_CONTEXT) == 0) {
		return E_PAR;
	}

	ret = task_rotate_queue(tskpri);
	task_reschedule();

	return ret;
}
#endif /* TFN_ROT_RDQ || TFN_IROT_RDQ */
