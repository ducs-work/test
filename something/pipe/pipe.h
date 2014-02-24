#ifndef pipe_H
#define pipe_H

#include "msg_unit.h"

typedef struct Node
{
  msg_t * msg_cmd;
  struct Node *prev;
  struct Node *next;
} QNode;

typedef struct QLink
{
  QNode *head;
  QNode *tail;
} QLink;
/*
 * need pthread_mutex fix me
 */


/*
 typedef enum
 {
 HEAD = 1
 TAIL = 0;
 };
 */


int
pull_pipe (  msg_t *msg_get , int type );
//void delmsg(*msg);
int
push_pipe ( msg_t *msg_get , int type );

void
destroy_pipe ( );

#endif
