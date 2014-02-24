#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "msg_unit.h"
#include <string.h>

msg_t *
msg_create ( int key , void * value )
{
  if (value == NULL)
    {
      fprintf(stderr, "para illegal!\n");
      return NULL;
    }
  char * cur_val = (char *) value;
  msg_t * cur_msg = malloc(sizeof(msg_t));
puts("dd");
  cur_msg->key = key;
  cur_msg->value = strdup(cur_val);
puts("d111d");

  return cur_msg;
}

void
msg_destroy ( msg_t *msg_cmd )
{
  free(msg_cmd);
}
