/**
  timer.h
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __TIMER_H__
#define __TIMER_H__
typedef struct lpc11xx_systick_register_t {
	volatile unsigned long dummy0;		// 0x00 ダミー
	volatile unsigned long dummy1;		// 0x04 ダミー
	volatile unsigned long dummy2;		// 0x08 ダミー
	volatile unsigned long dummy3;		// 0x0C ダミー
	volatile unsigned long csr;		// 0x10 システムタイマ制御及びステータスレジスタ
 	volatile unsigned long rvr;		// 0x14 システムタイマリロード値レジスタ
	volatile unsigned long cvr;		// 0x18 システムタイマ現在値レジスタ
	volatile unsigned long cal;		// 0x1C システムタイマ校正値レジスタ
} lpc11xx_systick_register_t;

typedef struct lpc11xx_ct32b1_register_t {
	volatile unsigned long ir;		// 0x00 割り込みレジスタ
	volatile unsigned long tcr;		// 0x04 タイマ制御レジスタ
	volatile unsigned long tc;		// 0x08 タイマカウンタ
	volatile unsigned long pr;		// 0x0C プリスケールレジスタ
	volatile unsigned long pc;		// 0x10 プリスケールカウンタ
 	volatile unsigned long mcr;		// 0x14 マッチ制御レジスタ
	volatile unsigned long mr0;		// 0x18 マッチレジスタ0
	volatile unsigned long mr1;		// 0x1C マッチレジスタ1
	volatile unsigned long mr2;		// 0x20 マッチレジスタ2
	volatile unsigned long mr3;		// 0x24 マッチレジスタ3
	volatile unsigned long ccr;		// 0x28 キャプチャ制御レジスタ
	volatile unsigned long cr0;		// 0x2c キャプチャレジスタ0
	volatile unsigned long dummy0;		// 0x30 dummy0
	volatile unsigned long dummy1;		// 0x34 dummy0
	volatile unsigned long dummy2;		// 0x38 dummy0
	volatile unsigned long emr;		// 0x3c 外部マッチレジスタ
	volatile unsigned long reserved0;	// 0x40
	volatile unsigned long reserved1;	// 0x44
	volatile unsigned long reserved2;	// 0x48
	volatile unsigned long reserved3;	// 0x4C
	volatile unsigned long reserved4;	// 0x50
	volatile unsigned long reserved5;	// 0x54
	volatile unsigned long reserved6;	// 0x58
	volatile unsigned long reserved7;	// 0x5c
	volatile unsigned long reserved8;	// 0x60
	volatile unsigned long reserved9;	// 0x64
	volatile unsigned long reserveda;	// 0x68
	volatile unsigned long reservedb;	// 0x6c
	volatile unsigned long ctcr;		// 0x70 カウント制御レジスタ
	volatile unsigned long pwmc;		// 0x74 PWM制御レジスタ
} lpc11xx_ct32b1_register_t;

enum {
	CT_CH_CT16B0 = 0,
	CT_CH_CT16B1,
	CT_CH_CT32B0,
	CT_CH_CT32B1,
	CT_CH_NUM
};

#define LPC11XX_SYSTICK		((volatile lpc11xx_systick_register_t *)0xe000e000)
#define LPC11XX_CT32B1		((volatile lpc11xx_ct32b1_register_t *)0x40018000)

#define TMR32BXMCR_MR0Rshell		(1 << 1)

typedef struct timer_t {
	int sw;
	timer_id id;
	unsigned long end;
} timer_t;

void systick_init(void);
void systick_handler(void);
void ct_init(int);
void init_wait(unsigned long);
void wait(int, unsigned long);
void init_timer(int);
#endif /* __TIMER_H__ */
