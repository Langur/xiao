/**
  memory.c
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#include "defines.h"
#include "os.h"
#include "lib.h"
#include "memory.h"

typedef struct memory_block_t {
	struct memory_block_t *next;
	int size;
} memory_block_t;

typedef struct memory_pool_t {
	int size;
	int num;
	memory_block_t *free;
} memory_pool_t;

static memory_pool_t pool[] = {
	{ 16,  8, NULL }, 
	{ 32,  8, NULL }, 
	{ 64,  4, NULL },
	{ 128, 1, NULL },
};

#define MEMORY_AREA_NUM (sizeof(pool) / sizeof(*pool))

static int
memory_init_pool(memory_pool_t *p)
{
	int i;
	memory_block_t *mp;
	memory_block_t **mpp;
	extern char _freearea; /* リンカスクリプトで定義される空き領域 */
	static char *area = &_freearea;

	mp = (memory_block_t *)area;

	mpp = &p->free;
	for (i = 0; i < p->num; i++) {
		*mpp = mp;
		memset(mp, 0, sizeof(*mp));
		mp->size = p->size;
		mpp = &(mp->next);
		mp = (memory_block_t *)((char *)mp + p->size);
		area += p->size;
	}

	return 0;
}

int
memory_init(void)
{
	int i;

	for (i = 0; i < MEMORY_AREA_NUM; i++) {
		memory_init_pool(&pool[i]);
	}

	return 0;
}

void *
_memory_alloc(int size, char *file, int line)
{
	int i;
	memory_block_t *mp;
	memory_pool_t *p;
	char buf[64];
	extern int debugalloc;

	if (debugalloc) {
		puts((char *)__func__);
		puts("(): ");
		puts(file);
		puts(":");
		putdval(line, 0);
		puts("\r\n");
	}
	for (i = 0; i < MEMORY_AREA_NUM; i++) {
		p = &pool[i];
		if (size <= p->size - sizeof(memory_block_t)) {
			if (p->free == NULL) {
				os_sysdown(__FILE__, __LINE__);
				return NULL;
			}
			mp = p->free;
			p->free = p->free->next;
			mp->next = NULL;

			return mp + 1;
		}
	}

	if (size != 0) {
		snprintf(buf, sizeof(buf),
			 "Request size of %u is not able to allocated.\r\n",
			 size);
		puts(buf);
	}
	os_sysdown(__FILE__, __LINE__);

	return NULL;
}

void
memory_free(void *mem)
{
	int i;
	memory_block_t *mp;
	memory_pool_t *p;

	mp = ((memory_block_t *)mem - 1);

	for (i = 0; i < MEMORY_AREA_NUM; i++) {
		p = &pool[i];
		if (mp->size == p->size) {
			mp->next = p->free;
			p->free = mp;

			return;
		}
	}

	os_sysdown(__FILE__, __LINE__);
}
