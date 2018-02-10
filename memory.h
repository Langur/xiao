/**
  memory.h
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __MEMORY_H__
#define __MEMORY_H__

int memory_init(void);
void *_memory_alloc(int, char*, int);
void memory_free(void *);

#define memory_alloc(x)	_memory_alloc(x, __FILE__, __LINE__)

#endif /* __MEMORY_H__ */
