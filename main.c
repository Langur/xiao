/**
  main.c
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include "defines.h"
#include "interrupt.h"
#include "serial.h"
#include "lib.h"
#include "timer.h"
#include "os.h"
#include "task.h"
#include "timestamp.h"
#include "version.h"

#define putnum(x)	do {\
				putxval(x, 0); puts(" \t"); putdval(x, 0); puts("\n"); \
		       	} while (0)

static int
init(void)
{
	serial_init(SERIAL_DEFAULT_DEVICE);
	systick_init();
	return 0;
}

static int
start_tasks(int argc, char *argv[])
{
	run_task(consdrv_main, "CONSDRV",  1, 0x80, 0, NULL);
	run_task(command_main, "COMMAND",  2, 0x100, 0, NULL);

	change_priority(15);
	while (1) {
		asm volatile ("wfi");
	}
	return 0;
}

static void
printBanner(void)
{
	char buf[DEFAULT_BUFFER_SIZE];

	puts("\r\n");
	snprintf(buf, DEFAULT_BUFFER_SIZE, "Xiao Version %u.%02u.%02u (%s)\r\n", V_MAJER, V_MINER, V_PATCH, timeStamp);
	puts(buf);
	puts("  Copyright (c) 2013-2018 Langur All Right Reserved.\r\n");
}

int
main(void)
{
	interrupt_disable(0);
	init();
	printBanner();

	os_start(start_tasks, "IDLE", 0, 0x10, 0, NULL);

	return 0;
}
