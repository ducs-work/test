#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "../common/msg_unit.h"
#include "../common/pipe.h"
#include "regist.h"

/**
 * 注册的 管理 结构体 struct Manger
 *
 * process_regist(int cmd, void(*)());
 */
/*
 void manager_init()
 {
 regist(ZMENGSTART, zmeng_start);
 regist(ZMENGSTOP, zmeng_stop);
 .....
 }
 */

void *
process ( void *p )
{
  msg_t * msg_get = (msg_t *) p;

  if (processunit_fun[msg_get->key](msg_get->value) < 0)
    {
      printf("execute cmd fail!\n");
      exit(1);
    }
  else
    {
      printf("exec success!\n");
    }
}
