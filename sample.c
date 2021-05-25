/**
  sample.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <kernel_id.h>
#include <lib.h>

void
task_sample(VP_INT extif)
{
	int i = 0;
	int id = (int)extif;
	while (1) {
		for (i = 0; i < 5; i++) {
	        	puts("[task");
			putxval(id, 1);
			puts("]\r\n");
		}
		rot_rdq(TPRI_SELF);
	}
}
