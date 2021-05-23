/**
  serial.h
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __SERIAL_H__
#define __SERIAL_H__

int serial_init(int);
int serial_is_send_enable(int);
int serial_send_byte(int, unsigned char);
int serial_is_recv_enable(int);
unsigned char serial_recv_byte(int);
int serial_intr_is_send_enable(int);
void serial_intr_send_enable(int);
void serial_intr_send_disable(int);
int serial_intr_is_recv_enable(int);
void serial_intr_recv_enable(int);
void serial_intr_recv_disable(int);

#endif
