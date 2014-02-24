#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "msg_unit.h"
#include "zmeng.h"
#include "ad_wall.h"
#include "thttpd.h"

/**
 * void run();
 */
void *
monitor ( )
{

  check_server_block(); //will not fail it's block

  while (1)
    {
      if (check_N_server(10) < 0)
        {
          fprintf(stderr, "ping fail!\n");
          /*shutdown*/
          msg_t * msg_cmd = msg_create("ad_wall", "stop");
          /*
           * push into pipe
           *
           */
          push_pipe(msg_cmd, 1);
        }
      else
        {    //pong
          /***monitor zmeng **/
          if (status_zmeng() < 0)
            {
              msg_t * msg_cmd = msg_create("zmeng", "restart");
              /*
               * push msg in pipe
               *
               */
              push_pipe(msg_cmd, 1);
            }

          else
            {
              printf("zmeng is alive\n");
            }

          /***check dog status***/
          if (status_ad_wall() < 0)
            {
              msg_t * msg_cmd = msg_create("ad_wall", "restart");
              /*
               * push msg into pipe
               *
               */
              push_pipe(msg_cmd, 1);
            }
          else
            {
              printf("wifi dog is alive\n");

            }

          /**monitor thttpd status**/

          if (status_http() < 0)
            {
             msg_t * msg_cmd =  msg_create("http", "restart");
              /**
               * push into pipe;
               *
               */
             push_pipe(msg_cmd, 1);
            }
          else
            {
              printf("thttp is alive\n ");
            }

        }
      sleep(300);    // need instead  setitimer
    }

}
