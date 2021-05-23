/**
  def_inh.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <service_call.h>
#include <interrupt.h>
#include <lib.h>

extern FP interrupt_handlers[];

ER
def_inh_body(INHNO inhno, T_DINH *pk_dinh)
{
	interrupt_handlers[inhno] = pk_dinh->inthdr;

	return E_OK;
}
