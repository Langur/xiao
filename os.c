/**
  os.c
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include "defines.h"
#include "os.h"
#include "interrupt.h"
#include "handler.h"
#include "consdrv.h"
#include "syscall.h"
#include "memory.h"
#include "lib.h"
#include "timer.h"

#define TASK_NUM 8
#define PRIORITY_NUM 16

/* メッセージバッファ */
typedef struct msgbuf_t {
	struct msgbuf_t *next;
	task_control_block_t *sender;
	struct {
		int size;
		char *p;
	} param;
} msgbuf_t;

/* メッセージボックス */
typedef struct msgbox_t {
	task_control_block_t *receiver;
	msgbuf_t *head;
	msgbuf_t *tail;
} msgbox_t;

/* タスクのレディーキュー */
static struct {
	task_control_block_t *head;
	task_control_block_t *tail;
} readyque[PRIORITY_NUM];


task_control_block_t *running = NULL;
task_control_block_t *current = NULL;
static task_control_block_t tcb[TASK_NUM];
static softhandler handlers[SOFTVEC_TYPE_NUM];
static msgbox_t msgboxes[MSGBOX_ID_NUM];


const unsigned long offsetof_task_context = (int)(&((task_control_block_t *)0)->context);
int canDispatch = 1;

static int
get_current_task(void)
{
	if (current == NULL) {
		return -1;
	}
	if (!(current->flags & TASK_FLAG_READY)) {
		return 1;
	}

	readyque[current->priority].head = current->next;
	if (readyque[current->priority].head == NULL) {
		readyque[current->priority].tail = NULL;
	}
	current->flags &= ~TASK_FLAG_READY;
	current->next = NULL;

	return 0;
}

static int
put_current_task(void)
{
	if (current == NULL) {
		return -1;
	}
	if (current->flags & TASK_FLAG_READY) {
		return 1;
	}

	if (readyque[current->priority].tail) {
		readyque[current->priority].tail->next = current;
	} else {
		readyque[current->priority].head = current;
	}
	readyque[current->priority].tail = current;
	current->flags |= TASK_FLAG_READY;

	return 0;
}

static void
task_end(void)
{
	exit_task();
}

static void
task_init(task_control_block_t *tcbp)
{
	tcbp->init.func(tcbp->init.argc, tcbp->init.argv);
	task_end();
}

static task_id
run_task_body(taskfunc func, char *name, int priority,
	      int stacksize, int argc, char *argv[])
{
	int i;
	task_control_block_t *tcbp;
	unsigned long *sp;
	extern char _userstack;
	static char *task_stack = &_userstack;

	for (i = 0; i < TASK_NUM; i++) {
		tcbp = &tcb[i];
		if (!tcbp->init.func)
			break;
	}
	if (i == TASK_NUM) {
		return -1;
	}
	
	memset(tcbp, 0, sizeof(*tcbp));
	
	strcpy(tcbp->name, name);
	tcbp->next = NULL;
	tcbp->priority = priority;
	tcbp->flags = 0;
	tcbp->init.func = func;
	tcbp->init.argc = argc;
	tcbp->init.argv = argv;

	memset(task_stack, 0, stacksize);
	task_stack += stacksize;

	tcbp->stack = task_stack;

	sp = (unsigned long *)tcbp->stack;
	*(--sp) = (unsigned long)0x01000000; /* EPSR.T=1 */
	*(--sp) = (unsigned long)task_init;  /* R15(PC) */
	*(--sp) = (unsigned long)task_end;   /* R14(LR): タスク終了時の戻り先 */
	*(--sp) = 0; 			     /* R12 */
	*(--sp) = 0; 			     /* R3 */
	*(--sp) = 0; 			     /* R2 */
	*(--sp) = 0; 			     /* R1 */
	*(--sp) = (unsigned long)tcbp;	     /* R0 */
	*(--sp) = 0;			     /* R7 */
	*(--sp) = 0;			     /* R6 */
	*(--sp) = 0;			     /* R5 */
	*(--sp) = 0;			     /* R4 */
	*(--sp) = 0;			     /* R11 */
	*(--sp) = 0;			     /* R10 */
	*(--sp) = 0;			     /* R9 */
	*(--sp) = 0;			     /* R8 */

	tcbp->context.sp = (unsigned long)sp;
	put_current_task();
	current = tcbp;
	put_current_task();

	return (task_id)current;
}

static int
exit_task_body(void)
{
	puts(current->name);
	puts(" EXIT.\n");
	memset(current, 0, sizeof(*current));
	return 0;
}

static int
wait_task_body(void)
{
	put_current_task();
	return 0;
}

static int
sleep_task_body(void)
{
	return 0;
}

static int
wakeup_task_body(task_id id)
{
	put_current_task();

	current = (task_control_block_t *)id;
	put_current_task();

	return 0;
}

static task_id
get_taskid_body(void)
{
	put_current_task();
	return (task_id)current;
}

static int
change_priority_body(int priority)
{
	int old = current->priority;

	if (priority >= 0) {
		current->priority = priority;
	}
	put_current_task();

	return old;
}

static void *
get_memory_body(int size)
{
	put_current_task();
	return memory_alloc(size);
}

static int
release_memory_body(char *p)
{
	memory_free(p);
	put_current_task();
	return 0;
}

static void
_sendmsg(msgbox_t *mboxp, task_control_block_t *tcbp, int size, char *p, char *file, int line)
{
	msgbuf_t *mp;
	extern int debugalloc;

	if (debugalloc) {
		puts((char *)__func__);
		puts("(): ");
		puts(file);
		puts(":");
		putdval(line, 0);
		puts("\r\n");
	}
	mp = (msgbuf_t *)memory_alloc(sizeof(*mp));
	if (mp == NULL) {
		os_sysdown(__FILE__, __LINE__);
	}

	mp->next       = NULL;
	mp->sender     = tcbp;
	mp->param.size = size;
	mp->param.p    = p;

	if (mboxp->tail) {
		mboxp->tail->next = mp;
	} else {
		mboxp->head = mp;
	}
	mboxp->tail = mp;
}

#define sendmsg(mboxp, tcbp, size, p) _sendmsg(mboxp, tcbp, size, p, __FILE__, __LINE__)

static void
recvmsg(msgbox_t *mboxp)
{
	msgbuf_t *mp;
	syscall_param_t *p;

	mp = mboxp->head;
	mboxp->head = mp->next;
	if (mboxp->head == NULL) {
		mboxp->tail = NULL;
	}
	mp->next = NULL;

	p = mboxp->receiver->syscall.param;
	p->un.receive_message.ret = (task_id)mp->sender;
	if (p->un.receive_message.sizep) {
		*(p->un.receive_message.sizep) = mp->param.size;
	}
	if (p->un.receive_message.pp) {
		*(p->un.receive_message.pp) = mp->param.p;
	}

	mboxp->receiver = NULL;

	memory_free(mp);
}

static int
send_message_body(msgbox_id id, int size, char *p)
{
	msgbox_t *mboxp = &msgboxes[id];

	put_current_task();
	sendmsg(mboxp, current, size, p);

	if (mboxp->receiver) {
		current = mboxp->receiver;
		recvmsg(mboxp);
		put_current_task();
	}

	return size;
}

static task_id
receive_message_body(msgbox_id id, int *sizep, char **pp)
{
	msgbox_t *mboxp = &msgboxes[id];

	if (mboxp->receiver) {
		char buf[DEFAULT_BUFFER_SIZE];
		snprintf(buf, sizeof(buf),
			      "MSGBOX[%u] is already used by %s\r\n",
			      id, mboxp->receiver->name);
		puts(buf);
		
		os_sysdown(__FILE__, __LINE__);
	}

	mboxp->receiver = current;
	if (mboxp->head == NULL) {
		return -1;
	}

	recvmsg(mboxp);
	put_current_task();

	return current->syscall.param->un.receive_message.ret;
}

static int
set_handler_body(softvec_type type, softhandler handler)
{
	handlers[type] = handler;
	put_current_task();

	return 0;
}

static void
call_functions(syscall_type type, syscall_param_t *p)
{
	if (type == SYSCALL_TYPE_RUN_TASK) {
		/* run_task() */
		p->un.run_task.ret = run_task_body(p->un.run_task.func,
						   p->un.run_task.name,
						   p->un.run_task.priority,
						   p->un.run_task.stacksize,
						   p->un.run_task.argc,
						   p->un.run_task.argv);
	} else if (type == SYSCALL_TYPE_EXIT_TASK) {
		/* exit_task() */
		exit_task_body();
	} else if (type == SYSCALL_TYPE_WAIT_TASK) {
		/* wait_task() */
		p->un.wait_task.ret = wait_task_body();
	} else if (type == SYSCALL_TYPE_SLEEP_TASK) {
		/* sleep_task() */
		p->un.sleep_task.ret = sleep_task_body();
	} else if (type == SYSCALL_TYPE_WAKEUP_TASK) {
		/* wakeup_task() */
		p->un.wakeup_task.ret = wakeup_task_body(p->un.wakeup_task.id);
	} else if (type == SYSCALL_TYPE_GET_TASKID) {
		/* get_taskid() */
		p->un.get_taskid.ret = get_taskid_body();
	} else if (type == SYSCALL_TYPE_CHANGE_PRIORITY) {
		/* change_priority() */
		p->un.change_priority.ret = change_priority_body(p->un.change_priority.priority);
	} else if (type == SYSCALL_TYPE_GET_MEMORY) {
		/* get_memory() */
		p->un.get_memory.ret = get_memory_body(p->un.get_memory.size);
	} else if (type == SYSCALL_TYPE_RELEASE_MEMORY) {
		/* release_memory() */
		p->un.release_memory.ret = release_memory_body(p->un.release_memory.p);
	} else if (type == SYSCALL_TYPE_SEND_MESSAGE) {
		/* send_message() */
		p->un.send_message.ret = send_message_body(p->un.send_message.id,
							   p->un.send_message.size,
							   p->un.send_message.p);
	} else if (type == SYSCALL_TYPE_RECEIVE_MESSAGE) {
		/* receive_message() */
		p->un.receive_message.ret = receive_message_body(p->un.receive_message.id,
								 p->un.receive_message.sizep,
								 p->un.receive_message.pp);
	} else if (type == SYSCALL_TYPE_SET_HANDLER) {
		/* set_handler() */
		p->un.set_handler.ret = set_handler_body(p->un.set_handler.type,
							 p->un.set_handler.handler);
	} else {
		;
	}
}

static void
syscall_proc(syscall_type type, syscall_param_t *p)
{
	get_current_task();
	call_functions(type, p);
}

static void
srvcall_proc(syscall_type type, syscall_param_t *p)
{
	current = NULL;
	call_functions(type, p);
}

void
dispatch(void)
{
	asm("cpsid	i\n");
	*ICSR |= PENDSVSET;
	asm("cpsie	i\n"
	    "bx		lr\n");
}

static void
schedule(void)
{
	int i;

	for (i = 0; i< PRIORITY_NUM; i++) {
		if (readyque[i].head) {
			break;
		}
	}
	if (i == PRIORITY_NUM) {
		os_sysdown(__FILE__, __LINE__);
	}

	current = readyque[i].head;
}

void
reschedule(void)
{
	schedule();
	if (canDispatch) {
		dispatch();
	}
}

static void
syscall_interrupt(void)
{
	syscall_proc(current->syscall.type, current->syscall.param);
}

static void
softerr_interrupt(void)
{
	puts(current->name);
	puts(" DOWN.\n");
	get_current_task();
	exit_task_body();
}

void
task_interrupt(softvec_type type)
{
	if (handlers[type]) {
		handlers[type]();
	}
	schedule();

	if (canDispatch) {
		dispatch();
	}
}

void
os_start(taskfunc func, char *name, int priority, int stacksize,
	 int argc, char *argv[])
{
	memory_init();

	current = NULL;
	
	memset(readyque, 0, sizeof(readyque));
	memset(tcb,      0, sizeof(tcb));
	memset(handlers, 0, sizeof(handlers));
	memset(msgboxes, 0, sizeof(msgboxes));

	set_handler_body(SOFTVEC_TYPE_SYSCALL, syscall_interrupt);
	set_handler_body(SOFTVEC_TYPE_SOFTERR, softerr_interrupt);
	set_handler_body(SOFTVEC_TYPE_SYSTICK, systick_handler);

	current = (task_control_block_t *)run_task_body(func, name, priority, stacksize, argc, argv);

	if (canDispatch) {
		dispatch();
	}

	/* ここには返ってこない */
}

void
os_sysdown(char *file, int line)
{
	char buf[DEFAULT_BUFFER_SIZE];
	extern int debugalloc;
	interrupt_disable_all();
	canDispatch = 0;
	snprintf(buf, DEFAULT_BUFFER_SIZE, "[%s] system error by %s:%u\n", current->name, file, line);
	puts(buf);
	while (1) {
		asm volatile ("wfi");
	}
}

void
syscall(syscall_type type, syscall_param_t *param)
{
	current->syscall.type  = type;
	current->syscall.param = param;
	asm volatile ("swi #0");
}

void
srvcall(syscall_type type, syscall_param_t *param)
{
	srvcall_proc(type, param);
}

void
os_stack(void)
{
	int i, ii, j;
	extern char _userstack;
	extern char _process_stack;
	char *k;

	puts("Stack lists\r\n");
	for (k = &_userstack, j = 0; k < &_process_stack; k += 16) {
		if (j == 0x100 || j == 0) {
			j = 0;
			puts("\r\n0x");
			putxval((unsigned long)k, 8);
			puts("\r\n");
		}
		puts("  ");
		for (i = 0; i < 16; i += 4) {
			for (ii = 3; ii >= 0; ii--) {
				putxval(k[i + ii], 2);
			}
			putc(' ');
		}
		j += i;
		puts("\r\n");
	}
 }
