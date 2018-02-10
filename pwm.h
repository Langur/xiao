#ifndef __PWM_H__
#define __PWM_H__

#include "defines.h"
#ifdef PWM
#define PWM_CT32B1_CH_NUM	(4)

typedef struct pwm_msg_set_t {
	unsigned int id;
	unsigned int channel;
	unsigned int value;
} pwm_msg_set_t;

typedef union pwm_msg_t {
	unsigned int id;
	pwm_msg_set_t msg_set;
} pwm_msg_t;

enum {
	PWM_MSG_ID_SET = 0,
	PWM_MSG_ID_ZZZ
};

void pwm_ct32b1_init(unsigned int, unsigned int);
void pwm_ct32b1_set_value(int, unsigned int);
unsigned int pwm_ct32b1_get_reg(unsigned int);
void pwm_send_set_val(unsigned int, unsigned int);
int pwm_main(int, char *[]);
#endif /* PWM */
#endif /* __PWM_H__ */
