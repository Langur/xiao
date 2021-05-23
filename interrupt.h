/**
  interrupt.h
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <interrupt_id.h>

#define TMAX_INHNO		(48)

#define INHNO_SERVICE_CALL	(0)

typedef struct t_def_inh_prm {
	INHNO inhno;
	T_DINH t_dinh;
} T_DEF_INH_PRM;

void interrupt_init(void);
void interrupt_handler_execute(INHNO inhno);

extern FP interrupt_handlers[];
extern const T_DEF_INH_PRM def_inh_prms[];
#endif /* __INTERRUPT_H__ */
