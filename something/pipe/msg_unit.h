#ifndef __MSG_UNIT_H__
#define __MSG_UNIT_H__

//#include "../include/command.h"

/**cmd
 *      1.   model-- cmd -- para
 *      2.       cmd     -- para                        !!
 */

typedef struct _msg_t
{
  int key;	//int key/cmd/...
  void *value;		//void *
} msg_t;

msg_t *
msg_create ( int  key , void * value );
void
msg_destroy ( msg_t * );

#endif
