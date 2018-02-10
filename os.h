/**
  os.h
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __OS_H__
#define __OS_H__

#include "defines.h"
#include "interrupt.h"
#include "syscall.h"

#define TASK_NAME_SIZE 15

typedef struct task_context_t {
	unsigned long sp;
} task_context_t;

typedef struct task_control_block_t {
	struct task_control_block_t *next;
	char name[TASK_NAME_SIZE + 1];
	int priority;
	char *stack;
	unsigned long flags;
#define TASK_FLAG_READY	(1 << 0)

	/* タスクの初期化時(task_init())に渡すパラメータ */
	struct {
		taskfunc func; /* タスクのメイン関数 */
		int argc;      /* タスクのメイン関数に渡す argc */
		char **argv;   /* タスクのメイン関数に渡す argv */
	} init;

	/* システムコール用バッファ */
	struct {
		syscall_type type;
		syscall_param_t *param;
	} syscall;

	task_context_t context; /* コンテキスト情報 */
} task_control_block_t;

void task_interrupt(softvec_type);

/* ライブラリ関数 */
void os_start(taskfunc, char *, int, int, int, char *[]);
void os_sysdown(char *, int);
void os_stack(void);

void syscall(syscall_type, syscall_param_t *);
void srvcall(syscall_type, syscall_param_t *);

void reschedule(void);

#endif /* __OS_H__ */
