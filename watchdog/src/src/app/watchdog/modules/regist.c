#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regist.h"

 int
(*processunit_fun[MAX_CMD]) ( void *para );

int
regist_cmd ( int cmd_id , void (*fun) )
{
  processunit_fun[cmd_id] = fun;
  printf("store fun at %x\n", processunit_fun[cmd_id]);
  return cmd_id;
}


int
unregist_cmd ( int cmd_id )
{
  processunit_fun[cmd_id] = NULL;
  return 0;
}
