/**
  syscall.c
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include "defines.h"
#include "os.h"
#include "syscall.h"
#include "lib.h"

/* システムコール */
task_id
run_task(taskfunc func, char *name, int priority, int stacksize,
	 int argc, char *argv[])
{
	syscall_param_t param;

	param.un.run_task.func = func;
	param.un.run_task.name = name;
	param.un.run_task.priority = priority;
	param.un.run_task.stacksize = stacksize;
	param.un.run_task.argc = argc;
	param.un.run_task.argv = argv;
	syscall(SYSCALL_TYPE_RUN_TASK, &param);

	return param.un.run_task.ret;
}

void
exit_task(void)
{
	syscall(SYSCALL_TYPE_EXIT_TASK, NULL);
}

int
wait_task(void)
{
	syscall_param_t param;

	syscall(SYSCALL_TYPE_WAIT_TASK, &param);

	return param.un.wait_task.ret;
}

int
sleep_task(void)
{
	syscall_param_t param;

	syscall(SYSCALL_TYPE_SLEEP_TASK, &param);

	return param.un.sleep_task.ret;
}

int
wakeup_task(task_id id)
{
	syscall_param_t param;

	param.un.wakeup_task.id = id;
	syscall(SYSCALL_TYPE_WAKEUP_TASK, &param);

	return param.un.wakeup_task.ret;
}

task_id
get_taskid(void)
{
	syscall_param_t param;

	syscall(SYSCALL_TYPE_GET_TASKID, &param);

	return param.un.get_taskid.ret;
}

int
change_priority(int priority)
{
	syscall_param_t param;

	param.un.change_priority.priority = priority;
	syscall(SYSCALL_TYPE_CHANGE_PRIORITY, &param);

	return param.un.change_priority.ret;
}

void *
_get_memory(int size, char *file, int line)
{
	syscall_param_t param;
	extern int debugalloc;
	if (debugalloc) {
		puts((char *)__func__);
		puts(": ");
		puts(file);
		puts(":");
		putdval(line, 0);
		puts("\r\n");
	}
	param.un.get_memory.size = size;
	syscall(SYSCALL_TYPE_GET_MEMORY, &param);

	return param.un.get_memory.ret;
}

int
release_memory(void *p)
{
	syscall_param_t param;

	param.un.release_memory.p = p;
	syscall(SYSCALL_TYPE_RELEASE_MEMORY, &param);

	return param.un.release_memory.ret;
}

int
send_message(msgbox_id id, int size, char *p)
{
	syscall_param_t param;

	param.un.send_message.id = id;
	param.un.send_message.size = size;
	param.un.send_message.p = p;
	syscall(SYSCALL_TYPE_SEND_MESSAGE, &param);

	return param.un.send_message.ret;
}

task_id
receive_message(msgbox_id id, int *sizep, char **pp)
{
	syscall_param_t param;

	param.un.receive_message.id = id;
	param.un.receive_message.sizep = sizep;
	param.un.receive_message.pp = pp;
	syscall(SYSCALL_TYPE_RECEIVE_MESSAGE, &param);

	return param.un.receive_message.ret;
}

int
set_handler(softvec_type type, softhandler handler)
{
	syscall_param_t param;

	param.un.set_handler.type = type;
	param.un.set_handler.handler = handler;
	syscall(SYSCALL_TYPE_SET_HANDLER, &param);

	return param.un.set_handler.ret;
}

/* サービスコール */
int
i_wakeup_task(task_id id)
{
	syscall_param_t param;

	param.un.wakeup_task.id = id;
	srvcall(SYSCALL_TYPE_WAKEUP_TASK, &param);

	return param.un.wakeup_task.ret;
}

void *
i_get_memory(int size)
{
	syscall_param_t param;
	extern int debugalloc;

	if (debugalloc) {
		puts((char *)__func__);
		puts("\r\n");
	}
	param.un.get_memory.size = size;
	srvcall(SYSCALL_TYPE_GET_MEMORY, &param);

	return param.un.get_memory.ret;
}

int
i_release_memory(void *p)
{
	syscall_param_t param;

	param.un.release_memory.p = p;
	srvcall(SYSCALL_TYPE_RELEASE_MEMORY, &param);

	return param.un.release_memory.ret;
}

int
i_send_message(msgbox_id id, int size, char *p)
{
	syscall_param_t param;

	param.un.send_message.id = id;
	param.un.send_message.size = size;
	param.un.send_message.p = p;
	srvcall(SYSCALL_TYPE_SEND_MESSAGE, &param);

	return param.un.send_message.ret;
}
