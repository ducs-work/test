#include <stdio.h>
#include <stdlib.h>
#include "receive.h"
#include "http.h"
#include "util.h"
#include "cJSON.h"
#include "regist.h"

//fix :need interface

void *
receive ( void * p )
{

  msg_t * msg_cmd = NULL;
  cJSON *json, *item, *item1, *sec_list;

  char *response;
  char req_url[128];
  req_url[0] = 0;
  char host[30];
  strcpy(host, "cif.zmeng123.cn");

  int ret;
  while (1)
    { //fix me

      sprintf(req_url, "api/adwall/conf/?mac=%s", get_my_mac());

      ret = http_connect(host, req_url, 80, &response);
      if (ret < 0)
        {
          printf("error while http from server\n");
          exit(1);
        }
      if (ret == 0)
        break;
    }

  json = cJSON_Parse(response);
  if (!json)
    {
      printf("hello....Error before: [%s]\n", cJSON_GetErrorPtr());
      exit(1);
    }
  else
    {
      while (json)
        {

          item = cJSON_GetObjectItem(json, "WIFINAME");
          if ((item != NULL) && (item->type == cJSON_String))
            {
              msg_cmd = msg_create("WIFINAME", safe_strdup(item->valuestring));
              push_pipe(msg_cmd, 1);
            }

          /*
           *
           *
           */

        } //end while(json)
    }

  sleep(100); //update time

} //while(1) loop

