/**
  syscall.h
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "defines.h"
#include "interrupt.h"

/* システムコール番号の定義 */
typedef enum {
	SYSCALL_TYPE_RUN_TASK = 0,
	SYSCALL_TYPE_EXIT_TASK,
	SYSCALL_TYPE_WAIT_TASK,
	SYSCALL_TYPE_SLEEP_TASK,
	SYSCALL_TYPE_WAKEUP_TASK,
	SYSCALL_TYPE_GET_TASKID,
	SYSCALL_TYPE_CHANGE_PRIORITY,
	SYSCALL_TYPE_GET_MEMORY,
	SYSCALL_TYPE_RELEASE_MEMORY,
	SYSCALL_TYPE_SEND_MESSAGE,
	SYSCALL_TYPE_RECEIVE_MESSAGE,
	SYSCALL_TYPE_SET_HANDLER,
} syscall_type;

/* システムコール呼び出し時のパラメータ格納域の定義 */
typedef struct {
	union {
		struct {
			taskfunc func;
			char *name;
			int priority;
			int stacksize;
			int argc;
			char **argv;
			task_id ret;
		} run_task;
		struct {
			int dummy;
		} exit_task;
		struct {
			int ret;
		} wait_task;
		struct {
			int ret;
		} sleep_task;
		struct {
			task_id id;
			int ret;
		} wakeup_task;
		struct {
			task_id ret;
		} get_taskid;
		struct {
			int priority;
			int ret;
		} change_priority;
		struct {
			int size;
			void *ret;
		} get_memory;
		struct {
			char *p;
			int ret;
		} release_memory;
		struct {
			msgbox_id id;
			int size;
			char *p;
			int ret;
		} send_message;
		struct {
			msgbox_id id;
			int *sizep;
			char **pp;
			task_id ret;
		} receive_message;
		struct {
			softvec_type type;
			softhandler handler;
			int ret;
		} set_handler;
	} un;
} syscall_param_t;

/* システムコール */
task_id run_task(taskfunc func, char *name, int proproty, int stacksize, int argc, char *argv[]);
void exit_task(void);

/* 
 * current taskをready queueの後ろに繋ぎなおすことで
 * current taskを切り替える
 */
int wait_task(void);

/*
 * taskをready queueからはずしてsleep状態にする
 */
int sleep_task(void);

/*
 * sleep状態のtaskをready queueに繋ぎ直して(wake up)
 * ready状態に戻す
 */
int wakeup_task(task_id id);

/*
 * 自身のtask IDを取得する
 */
task_id get_taskid(void);

/*
 * taskの優先度を変更する
 */
int change_priority(int priority);

/*
 * ヒープ領域にメモリを割り当てる
 */
void *_get_memory(int size, char *, int);
#define get_memory(size) _get_memory(size, __FILE__, __LINE__)
/*
 * ヒープ領域に確保したメモリを解放する
 */
int release_memory(void *p);

/* 
 * メッセージの送信処理
 */
int send_message(msgbox_id id, int size, char *p);

/* 
 * メッセージの受信処理 
 */
task_id receive_message(msgbox_id id, int *sizep, char **pp);

int set_handler(softvec_type type, softhandler handler);

/* サービスコール */
int i_wakeup_task(task_id id);
void *i_get_memory(int size);
int i_release_memory(void *p);
int i_send_message(msgbox_id id, int size, char *p);

#endif /* __SYSCALL_H__ */
