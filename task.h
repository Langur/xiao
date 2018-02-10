/**
  task.h
  
  Copyright (c) 2013-2018 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __TASK_H__
#define __TASK_H__

/* システムタスク */
int consdrv_main(int, char *[]);

/* ユーザタスク */
int command_main(int, char *[]);

#endif /* __TASK_H__ */
