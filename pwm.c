#include "defines.h"
#ifdef PWM
#include "pwm.h"
#include "timer.h"
#include "os.h"
#include "syscall.h"
#include "memory.h"
#include "lib.h"

#define IOCON_R_PIO1_1		((volatile unsigned long *)0x4004407C)
#define IOCON_R_PIO1_2		((volatile unsigned long *)0x40044080)
#define IOCON_SWDIO_PIO1_3	((volatile unsigned long *)0x40044090)
#define IOCON_PIO1_4		((volatile unsigned long *)0x40044094)

static volatile lpc11xx_ct32b1_register_t *ct32b1_regs = LPC11XX_CT32B1;

static volatile unsigned long *iocon_ct32b1[] = {
	IOCON_R_PIO1_1,
	IOCON_R_PIO1_2,
	IOCON_SWDIO_PIO1_3,
	IOCON_PIO1_4
};

#define IOCON_SET_CT32B1(reg)	do { *(reg) &= ~0x3F; *(reg) |= (reg != IOCON_PIO1_4) ? 0x83 : 0x82; } while (0);

void
pwm_ct32b1_init(unsigned int channel, unsigned int reset)
{
	volatile lpc11xx_ct32b1_register_t *ct32b1 = ct32b1_regs;

	*SYSAHBCLKCTRL |= AHB_CT32B1;
	ct32b1->pr = 1200 - 1;
	*IOCON_R_PIO1_2 |= 3;
	*IOCON_SWDIO_PIO1_3 |= 3;
	*IOCON_PIO1_4 |= 2;
	ct32b1->pwmc |= 0x0e;
	ct32b1->mcr |= 0x02;
	ct32b1_regs->mr0 = 99;
	ct32b1_regs->mr1 = 100;
	ct32b1_regs->mr2 = 25;
	ct32b1_regs->mr3 = 100;
	ct32b1->tcr = 1;
}

void
pwm_ct32b1_set_value(int channel, unsigned int value)
{
	volatile unsigned long *ct32b1_mr = &(ct32b1_regs->mr0);
	
	ct32b1_mr[channel] = value;
}

unsigned int
pwm_ct32b1_get_reg(unsigned int offset)
{
	return *(unsigned int *)(LPC11XX_CT32B1 + offset);
}

void
pwm_send_set_val(unsigned int channel, unsigned int value)
{
	pwm_msg_set_t *msg;
	msg = malloc(sizeof(pwm_msg_set_t));
	msg->id = PWM_MSG_ID_SET;
	msg->channel = channel;
	msg->value = value;
	send_message(MSGBOX_ID_PWM, sizeof(pwm_msg_set_t), (char *)msg);
}

int 
pwm_main(int argc, char *argv[])
{
	pwm_msg_t *msg;
	int size;
	unsigned int ch, val;

	while (1) {
		receive_message(MSGBOX_ID_PWM, &size, (char **)&msg);
		if (!msg) {
			continue;
		}
		switch (msg->id) {
		case PWM_MSG_ID_SET:
			ch = msg->msg_set.channel;
			val = msg->msg_set.value;
			pwm_ct32b1_set_value(ch, val);
		case 1:
			;
		}
		free(msg);
	}

	return 0;
}
#endif /* PWM */
