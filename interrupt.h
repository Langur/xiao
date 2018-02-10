/**
  interrupt.h
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "defines.h"

/* ソフトウエア・割込みベクタの定義 */
#define SOFTVEC_TYPE_NUM     64
#define SOFTVEC_TYPE_IRQ(x)  ((x) + 16)

#define SOFTVEC_TYPE_SYSCALL 0				// SVC #0
#define SOFTVEC_TYPE_SOFTERR 3				// HardFault
#define SOFTVEC_TYPE_SYSTICK 15				// SysTick
#define SOFTVEC_TYPE_I2CINTR SOFTVEC_TYPE_IRQ(15)
#define SOFTVEC_TYPE_SERINTR SOFTVEC_TYPE_IRQ(21)

void interrupt_init(void);
void interrupt_enable(softvec_type);
void interrupt_disable_all(void);
void interrupt_disable(softvec_type);
void interrupt_clear(softvec_type);

#define INTR_ENABLE	do { interrupt_enable(0); } while (0)
#define INTR_DISABLE	do { interrupt_disable(0); } while (0)

#endif /* __INTERRUPT_H__ */
