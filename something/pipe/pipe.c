#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pipe.h"

/**
 *
 * cond	/mutex		资源锁， 有计数，  用于 pull 的时候 同步等待
 */

/**
 * C 单例   instance
 */
static QLink * queue = NULL;


QLink *
create_queue ( )
{
  QLink *Qu = NULL;
  Qu = (QLink *) malloc(sizeof(QLink));
  if (Qu == NULL)
    {
      printf("queue create fail!\n");
      return NULL;
    }

  Qu->head = NULL;
  Qu->tail = NULL;
  queue =  Qu;

  return queue;
}


int
add_queue_head ( msg_t *msg_cmd )
{
  if (msg_cmd == NULL)
    {
      fprintf(stderr, "para illegal\n");
      return -1;
    }

  QLink *Qu = NULL;
  QNode *node = NULL;
  QNode *ptr = NULL;

  Qu = (QLink *) queue;

  node = (QNode *) malloc(sizeof(QNode));
  if (node == NULL)
    {
      printf("fail!\n");
      return -1;
    }

printf("pack:%d--%s\n",msg_cmd->key, msg_cmd->value);


  node->msg_cmd = (void *) malloc(sizeof(msg_cmd));
   memcpy(node->msg_cmd, msg_cmd, sizeof(msg_cmd));
puts("123");
 // node->msg_cmd = msg_cmd;
puts("1222222");

  if (Qu->head == NULL && Qu->tail == NULL)
    {
      Qu->head = node;
      Qu->tail = node;
      node->next = NULL;
      node->prev = NULL;
    }
  else
    {
      ptr = Qu->head;
      ptr->prev = node;
      node->next = ptr;
      Qu->head = node;
      node->prev = NULL;
    }

  return 0;

}
int
add_queue_tail ( msg_t *msg_cmd )
{
  QLink *Qu = NULL;
  QNode *node = NULL;
  QNode *ptr = NULL;

  Qu = (QLink *) queue;
  node = (QNode *) malloc(sizeof(QNode));
  if (node == NULL)
    {
      printf("qnote create fail\n");
      return -1;
    }

  // node->msg_cmd = (void *) malloc(msg_cmd);

  // memcpy(node->msg_cmd, msg_cmd, sizeof(msg_cmd));
  node->msg_cmd = msg_cmd;

  ptr = Qu->tail;
  /*查看是否是第一个接点*/
  if (Qu->head == NULL && Qu->tail == NULL)
    {
      Qu->head = node;
      Qu->tail = node;
      node->next = NULL;
      node->prev = NULL;
    }
  else
    {
      ptr->next = node;
      node->next = NULL;
      node->prev = ptr;
      Qu->tail = node;
    }
  return 0;

}

//get data to msg_get;
int
get_queue_tail ( msg_t * msg_get )
{
  QLink *Qu = NULL;
  QNode *ptr = NULL;

  Qu = (QLink *) queue;
  ptr = Qu->tail;
  if (Qu->head == NULL && Qu->tail == NULL)
    {
      printf("queue is empty!\n");
      return -1;
    }

//  memcpy(msg_get, ptr->msg_cmd, sizeof(ptr->msg_cmd));
  msg_get = ptr->msg_cmd;
  if (Qu->head == Qu->tail)
    {
      Qu->head = NULL;
      Qu->tail = NULL;
      free(ptr);
    }
  else
    {
      Qu->tail = ptr->prev;
      ptr->prev->next = NULL;
      free(ptr);
    }
  return 0;
}

/***************************************************
 *函数功能 : 删除队列的 头结点
 *参数@queue 接口handle ，传入参数
 *参数@elem，返回删除的数据，传出参数
 ****************************************************/
int
get_queue_head ( msg_t * msg_get )
{
  QLink *Qu = NULL;
  QNode *ptr = NULL;

  Qu = (QLink *) queue;

  ptr = Qu->head;

  if (Qu->head == NULL && Qu->tail == NULL)
    {
      printf("queue is empty !\n");
      return -1;
    }

  memcpy(msg_get, ptr->msg_cmd, sizeof(ptr->msg_cmd));
 // msg_get = ptr->msg_cmd;
  if (Qu->head == Qu->tail)
    {
      Qu->head = NULL;
      Qu->tail = NULL;
      free(ptr);

    }
  else
    {
      Qu->head = ptr->next;
      ptr->next->prev = NULL;
      free(ptr);
    }
  return 0;
}

void
destory_queue ( )
{
  QLink *Qu = NULL;
  QNode *ptr = NULL;

  Qu = (QLink *) queue;

  if (Qu->head == NULL && Qu->tail == NULL)
    {
      printf("queue is empty!\n");
      free(Qu);
      // return -1;
    }

  ptr = Qu->head;

  while (ptr != NULL)
    {
      Qu->head = ptr->next;
      free(ptr);
      ptr = Qu->head;
    }
  Qu->head = NULL;
  Qu->tail = NULL;

#if 0
  while(Qu->head != NULL)
    {

      Qu->head = ptr->next;
      ptr->next->prev = NULL;
      free(ptr);
      ptr = Qu->head;
      if (Qu->head == Qu->tail)
        {
          Qu->head =NULL;
          Qu->tail = NULL;
          free(ptr);
        }
    }
#endif
  free(Qu);

}

QLink *
init_pipe ( )
{

  return create_queue();

}

/*
 * func:get data from pipe
 * para:
 *      msg_get:  must have had malloced
 *      type:   1-->get_queue_head
 *              2-->get_queue_tail
 *
 */
int
pull_pipe ( msg_t *msg_get , int type )
{
  if (type == 1)
    {
      return get_queue_head(msg_get);
    }
  else if (type == 2)
    {
      return get_queue_tail(msg_get);

    }
  else
    {
      puts("type error!");
      return -1;
    }

}

/*
 * func:push data to pipe
 * para:
 *      msg_get:  must have had malloced
 *      type:   1-->add_queue_head
 *              2-->add_queue_tail
 *
 */

int
push_pipe ( msg_t * msg_cmd , int type )
{
  if (type == 1)
    {
      return add_queue_head(msg_cmd);

    }
  else if (type == 2)
    {
      return add_queue_tail(msg_cmd);

    }
  else
    {
      puts("type error!");
      return -1;
    }
}


void
destroy_pipe ( )
{

  destory_queue();

}


/*
void
main ( void )
{
  init_pipe();

  pipe_push(c, 1);

  pipe_pull(c1, 1);

}

*/
