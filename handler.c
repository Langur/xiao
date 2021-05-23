/**
  handler.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include <defines.h>
#include <itron.h>
#include <kernel.h>
#include <task.h>
#include <interrupt.h>
#include <handler.h>
#include <lib.h>

extern T_TCB *task_running;
extern T_TCB *task_current;
extern const UW task_context_offset;
volatile VP_INT sp;

void
handler_nmi(void)
{
	asm("mrs	r0, ipsr\n"
	    "b		interrupt_handler_execute\n");
}

void
handler_hardfault(void)
{
	puts("HardFault\n");
	asm("mrs	r0, ipsr\n"
	    "b		interrupt_handler_execute\n");
}

void
handler_common(void)
{
	asm("mrs	r0, ipsr\n"
	    "b		interrupt_handler_execute\n");
}

void
handler_svc(void)
{
	asm("mrs	r0, msp\n"
	    "mov	r1, lr\n"
	    "lsl	r1, r1, #28\n"
	    "lsr	r1, r1, #28\n"
	    "cmp	r1, #0xd\n"
	    "bne	1f\n"
	    "mrs	r0, psp\n"
	    "1:\n");

	asm("ldr	r0, [r0, #24]\n"
	    "sub	r0, r0, #2\n");

	asm("ldrb	r0, [r0]\n"
	    "b          interrupt_handler_execute\n");
}

void
handler_pendsv(void)
{
	asm("cpsid	i\n");

	*ICSR |= PENDSVCLR;

	asm("ldr	r2, =task_running\n"
	    "ldr	r0, [r2]\n"
	    "ldr	r1, =task_current\n"
	    "ldr	r1, [r1]\n"
	    "cmp	r0, r1\n"
	    "beq	no_dispatch\n"
	    "str	r1, [r2]\n");

	asm("ldr	r3, =task_context_offset\n"
	    "ldr	r3, [r3]\n"
	    "add	r0, r0, r3\n"
	    "add	r1, r1, r3\n");

	asm("mrs	r3, psp\n"
	    "sub	r3, r3, #0x10\n"
	    "stmia	r3!, {r4-r7}\n"
	    "mov	r4, r8\n"
	    "mov	r5, r9\n"
	    "mov	r6, r10\n"
	    "mov	r7, r11\n"
	    "sub	r3, r3, #0x20\n"
	    "stmia	r3!, {r4-r7}\n"
	    "sub	r3, r3, #0x10\n"
	    "str	r3, [r0]\n");

	asm("ldr	r3, [r1]\n"
	    "ldmia	r3!, {r4-r7}\n"
	    "mov	r8, r4\n"
	    "mov	r9, r5\n"
	    "mov	r10, r6\n"
	    "mov	r11, r7\n"
	    "ldmia	r3!, {r4-r7}\n"
	    "msr	psp, r3\n");
	asm("isb	\n");

	asm("no_dispatch:\n");
	asm("cpsie	i\n"
	    "bx	lr\n");
}
