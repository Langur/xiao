/**
  idle.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <kernel_id.h>
#include <lib.h>
#include <task.h>

void
task_idle(VP_INT extif)
{
	act_tsk(TSKID_TASK1);
	
	while (1) {
	        asm volatile ("wfi");
	}
}
