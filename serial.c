/**
  serial.c
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include "defines.h"
#include "serial.h"

#define SERIAL_NUM	1
#define	UART0_BASE	0x40008000
#define	UART_CLOCK	(CPU_CLOCK / 1)	// Hz
#define	UART_SPEED	9600		// bps
#define	UART_DIVISOR	(UART_CLOCK / (UART_SPEED * 16))

#define	UART_RBR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x000))	/* R- */
#define	UART_THR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x000))	/* -W */
#define UART_DLL(base)			((volatile unsigned long *)((unsigned long)(base) | 0x000))	/* RW */
#define	UART_DLM(base)			((volatile unsigned long *)((unsigned long)(base) | 0x004))	/* RW */
#define	UART_IER(base)			((volatile unsigned long *)((unsigned long)(base) | 0x004))	/* RW */
#define	UART_IIR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x008))	/* R- */
#define	UART_FCR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x008))	/* -W */
#define	UART_LCR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x00c))	/* RW */
#define	UART_LSR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x014))	/* R- */
#define	UART_SCR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x01C))	/* RW */
#define	UART_ACR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x020))	/* RW */
#define	UART_ICR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x024))	/* RW */
#define	UART_FDR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x028))	/* RW */
#define	UART_TER(base)			((volatile unsigned long *)((unsigned long)(base) | 0x02C))	/* RW */

#define	UART_MCR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x010))	/* RW */
#define	UART_MSR(base)			((volatile unsigned long *)((unsigned long)(base) | 0x018))	/* R- */


/* LPC111X独自のレジスタ：ピンおよび省電力の設定 */
#define	IOCON_PIO1_6			((volatile unsigned long *)0x400440a4)
#define	IOCON_PIO1_7			((volatile unsigned long *)0x400440a8)
#define	AHBCLKCTRL			((volatile unsigned long *)0x40048080)
#define	UARTCLKDIV			((volatile unsigned long *)0x40048098)

/* デバイス初期化 */
int
serial_init(int index)
{
	volatile unsigned long *uart;
	unsigned long base = UART0_BASE;

	if (index != 0) {
		return -1;
	}

	/* GPIOをUARTに設定 */
	*IOCON_PIO1_6 = 0xc1;		/* RXDピンを有効にする */
	*IOCON_PIO1_7 = 0xc1;		/* TXDピンを有効にする */
	*UARTCLKDIV = 0x01;		/* UART_PCLK = system clock / 1 */

	/* UART起動 */
	*AHBCLKCTRL |= (1 << 12);


	uart = UART_IER(base); *uart = 0x00;

	// 送信データがあった場合は送信完了まで待つ
	while (!(*(UART_LSR(base)) & 0x40)) {
		;
	}

	// 分周比レジスタに切り替え
	uart = UART_LCR(base); *uart = 0x80;
	uart = UART_DLM(base); *uart = 0xff;
	uart = UART_DLL(base); *uart = (UART_DIVISOR >> 0) & 0xff;
	uart = UART_DLM(base); *uart = (UART_DIVISOR >> 8) & 0xff;
	uart = UART_FDR(base); *uart = 0x00;
	uart = UART_LCR(base); *uart = 0x03;
	uart = UART_MCR(base); *uart = 0x03;
	uart = UART_FCR(base); *uart = 0x07;

	return 0;
}

int
serial_is_send_enable(int index)
{
	unsigned int base = UART0_BASE;

	if (index != 0) {
		return -1;
	}

	return *(UART_LSR(base)) & 0x20;
}

int
serial_send_byte(int index, unsigned char c)
{
	unsigned int base = UART0_BASE;

	if (index != 0) {
		return -1;
	}

	while (!serial_is_send_enable(index)) {
		;
	}
	*UART_THR(base) =  c;

	return 0;
}

int
serial_is_recv_enable(int index)
{
	unsigned int base = UART0_BASE;

	if (index != 0) {
		return -1;
	}

	return *(UART_LSR(base)) & 0x1f;
}

unsigned char
serial_recv_byte(int index)
{
	unsigned int base = UART0_BASE;
	unsigned char s, c;

	while (1) {
		while (!(s = serial_is_recv_enable(index))) {
			;
		}
		c = (unsigned char)(*UART_RBR(base) & 0xFF);

		if (!(s & 0x1e)) break;
	}

	return c;
}

int
serial_intr_is_send_enable(int index)
{
	unsigned int base = UART0_BASE;

	if (index != 0) {
		return -1;
	}

	return *(UART_IER(base)) & 0x02;
}

void
serial_intr_send_enable(int index)
{
	unsigned int base = UART0_BASE;

	if (index != 0) {
		return;
	}

	*(UART_IER(base)) |= 0x02;
}

void
serial_intr_send_disable(int index)
{
	unsigned int base = UART0_BASE;

	if (index != 0) {
		return;
	}

	*(UART_IER(base)) &= ~0x02;
}

int
serial_intr_is_recv_enable(int index)
{
	unsigned int base = UART0_BASE;

	if (index != 0) {
		return -1;
	}

	return *(UART_IER(base)) & 0x01;
}

void
serial_intr_recv_enable(int index)
{
	unsigned int base = UART0_BASE;

	if (index != 0) {
		return;
	}

	*(UART_IER(base)) |= 0x01;
}

void
serial_intr_recv_disable(int index)
{
	unsigned int base = UART0_BASE;

	if (index != 0) {
		return;
	}

	*(UART_IER(base)) &= ~0x01;
}
