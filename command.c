/**
  command.c
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include "defines.h"
#include "os.h"
#include "consdrv.h"
#include "lib.h"
#include "timer.h"
#include "timestamp.h"
#include "memory.h"
#include "version.h"

int debugalloc=0;

static void
send_use(int index)
{
	char *p;

	if (index != 0) {
		return;
	}

	p = malloc(3);
	p[0] = '0';
	p[1] = CONSDRV_CMD_USE;
	p[2] = '0' + index;
	send_message(MSGBOX_ID_CONSOUTPUT, 3, p);
}

static void
send_write(char *str)
{
	char *p;
	int len;

	len = strlen(str);
	p = malloc(len + 2);
	p[0] = '0';
	p[1] = CONSDRV_CMD_WRITE;
	memcpy(&p[2], str, len);
	send_message(MSGBOX_ID_CONSOUTPUT, len + 2, p);
}

int
command_main(int argc, char *argv[])
{
	char *p;
	int size;
	char buf[64];

	send_use(SERIAL_DEFAULT_DEVICE);

	while (1) {
		send_write("> "); /* プロンプト表示 */

		/* コンソールからの受信文字列を受け取る */
		receive_message(MSGBOX_ID_CONSINPUT, &size, &p);
		p[size] = '\0';
		
		if (!strncmp(p, "echo", 4)) {
			send_write(p + 4); /* echoに続く文字列を出力する */
			send_write("\n");
		} else if (!strncmp(p, "uname", 5)) {
			puts("\r\n");
			snprintf(buf, DEFAULT_BUFFER_SIZE, "Xiao Version %u.%02u.%02u (%s)\r\n",
				 V_MAJER, V_MINER, V_PATCH, timeStamp);
			puts(buf);
		} else if (!strncmp(p, "ticks", 5)) { /* ticksコマンド */
			extern unsigned long ticks;
			div_t time;
	
			time = div(ticks, 2000);
			snprintf(buf, sizeof(buf),"Elapsed time from boot = %u[sec]\r\n", time.div);
			send_write(buf);
		} else if (!strncmp(p, "stack", 5)) { /* stackコマンド */
			os_stack();
		} else if (!strncmp(p, "alloc", 5)) { /* allocコマンド */
			debugalloc = 1;
		} else if (!strncmp(p, "command", 7)) { /* commandコマンド */
			send_write("  ticks\r\n  uname\r\n");
		} else {
			if (size != 0) {
				send_write("Invalid command name.\r\n");
			}
		}

		free(p);
	}

	return 0;
}
