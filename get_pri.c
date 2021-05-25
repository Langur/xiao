/**
  get_pri.c
  
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

extern T_TCB *task_current;
extern T_TCB task_control_blocks[];

#ifdef TFN_GET_PRI
ER
get_pri(ID tskid, PRI *p_tskpri)
{
	U_SPRM prm;

	prm.get_pri.ret    = E_OK;
	prm.get_pri.tskpri = 0;

	if (task_current && tskid == TSK_SELF) {
		prm.get_pri.tskid  = task_current->tskid;
	} else {
		prm.get_pri.tskid  = tskid;
	}

	service_call(SCID_GET_PRI, &prm);
	
	if (!p_tskpri) {
		prm.get_pri.ret = E_PAR;
	} else {
		*p_tskpri = prm.get_pri.tskpri;
	}
	
	return prm.get_pri.ret;
}

ER
get_pri_body(ID tskid, PRI *p_tskpri)
{
	int idx = TSKID_IDX(tskid);

	if (tskid > TMAX_TSKID || tskid < 0) {
		return E_NOEXS;
	}

	if (!p_tskpri) {
		return E_PAR;
	}

	if (task_control_blocks[idx].state == TA_TSK_STAT_DORMANT) {
		return E_OBJ;
	}
	
	*p_tskpri = task_control_blocks[idx].npri;

	return E_OK;
}
#endif /* TFN_GET_PRI */
