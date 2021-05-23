/**
  handler.h
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "defines.h"

void handler_nmi(void);
void handler_hardfault(void);
void handler_common(void);
void handler_svc(void);
void handler_pendsv(void);
extern void systick_handler(void);
#endif /* __HANDLER_H__ */
