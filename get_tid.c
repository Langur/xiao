/**
  get_tid.c
  
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

extern T_TCB *task_running;

#ifdef TFN_GET_TID
ER
get_tid(ID *p_tskid)
{
	U_SPRM prm;

	prm.get_tid.ret   = E_OK;
	prm.get_tid.tskid = TSK_NONE;

	service_call(SCID_GET_TID, &prm);
	
	if (!p_tskid) {
		prm.get_tid.ret = E_PAR;
	} else {
		*p_tskid = prm.get_tid.tskid;
	}
	
	return prm.get_tid.ret;
}

ER
get_tid_body(ID *p_tskid)
{
        ER ret;

        ret = get_tid_common(p_tskid);

        return ret;
}
#endif /* TFN_GET_TID */

#ifdef TFN_IGET_TID
ER
iget_tid(ID *p_tskid)
{
	U_SPRM prm;

	prm.iget_tid.ret   = E_OK;
	prm.iget_tid.tskid = TSK_NONE;

	iservice_call(SCID_IGET_TID, &prm);
	
	if (!p_tskid) {
		prm.iget_tid.ret = E_PAR;
	} else {
		*p_tskid = prm.iget_tid.tskid;
	}

	return prm.iget_tid.ret;
}

ER
iget_tid_body(ID *p_tskid)
{
        ER ret;

        ret = get_tid_common(p_tskid);

        return ret;
}
#endif /* TFN_IGET_TID */

#if defined TFN_GET_TID || defined TFN_IGET_TID
ER
get_tid_common(ID *p_tskid)
{
	if (!p_tskid) {
		return E_PAR;
	}

	if (task_running) {
		*p_tskid = task_running->tskid;
	} else {
		*p_tskid = TSK_NONE;
	}

	return E_OK;
}
#endif /* TFN_GET_TID || TFN_IGET_TID */
