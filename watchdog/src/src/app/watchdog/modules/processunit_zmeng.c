#include <stdio.h>
#include <stdlib.h>
#include "processunit_zmeng.h"
#include "regist.h"

void
process_zmeng_init ( )
{
  regist(0, zmeng_start);
  regist(1, zmeng_stop);
  regist(2, zmeng_set);
  regist(3, zmeng_status);
}

