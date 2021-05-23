/**
  reset.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>

__attribute__ ((section(".text.startup")))
void
reset_entry(void)
{
	extern volatile char _data_org[], _data_start[], _data_end[];
	extern volatile char _bss_start[], _bss_end[];
	volatile char *s, *d;

	/* 割込み禁止 */
	asm("cpsid	i\n");

	/* PSPの初期化 */
	asm("ldr	r0, =_process_stack\n"
	    "msr	psp, r0\n");

	asm("mov	r0, #2\n"
	    "msr	control, r0\n");

	asm("isb	\n");
	
	/* 割込みベクタをREMAPしない */
	asm("memmap_init:\n");
	*SYSMEMREMAP = 2;

	/* I/O configuration blockの初期化 */
	asm("clock_init:\n");
	*SYSAHBCLKCTRL |= AHB_IOCON;

	/* data領域の初期化 */
	for(d = _data_start, s = _data_org; d < _data_end; *d++ = *s++);

	/* bss領域の初期化 */
	for(d = _bss_start; d < _bss_end; *d++ = 0);

	asm("b main\n");
}
