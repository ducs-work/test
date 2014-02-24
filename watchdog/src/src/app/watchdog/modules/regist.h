#ifndef __REGIST_H__
#define __REGIST_H__

#define MAX_CMD 1024

extern int
(*processunit_fun[MAX_CMD]) ( void *para );

int
regist_cmd ( int cmd_id , void (*fun) );

int
unregist_cmd ( int cmd_id );

#endif
