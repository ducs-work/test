#include <stdio.h>
#include <stdlib.h>

/*start the server_name*/
int
z_start ( char *server_name )
{ //server: wifidog zmeng httpd inject
  if (server_name == NULL)
    {
      fprintf(stderr, "para illegal\n");
      return -1;
    }

#ifdef DD
  //wifidog
  if(!strstr(server_name, "wifidog"))
    {
      if( ad_wall("start") < 0)
        {
          printf("start wifidog fail!\n");
          return -1;
        }
      else
        {
          printf("start wifidog success!\n");
          return 0;
        }

    }
  else if(!strstr(server_name, "zmeng"))
    { //zmeng?

      if(zmeng("start") < 0)
        {
          printf("start zmeng fail!\n");
          return -1;
        }
      else
        {
          printf("start zmeng success!\n");
          return 0;
        }
    }
  else
    {
      printf("undefined server name :%s",server_name);
      return 0;
    }
#endif
#ifdef TT
#endif

  return 0;
}

/*stop the server_name*/
int
z_stop ( char * server_name )
{
  if (server_name == NULL)
    {
      fprintf(stderr, "para illegal\n");
      return 1;
    }
#ifdef DD
  if(!strstr(server_name, "wifidog"))
    {
      if( ad_wall("stop") < 0)
        {
          printf("stop wifidog fail!\n");
          return -1;
        }
      else
        {
          printf("stop wifidog success!\n");
          return 0;
        }

    }
  else if(!strstr(server_name, "zmeng"))
    { //zmeng?

      if(zmeng("stop") < 0)
        {
          printf("stop zmeng fail!\n");
          return -1;
        }
      else
        {
          printf("stop zmeng success!\n");
          return 0;
        }
    }
  else
    {
      printf("undefined server name :%s",server_name);
      return 0;
    }

#endif
#ifdef TT
#endif

  return 0;
}

/*restart the server_name*/
int
z_restart ( char * server_name )
{
  if (server_name == NULL)
    {
      fprintf(stderr, "para illegal\n");
      return 1;
    }
#ifdef DD
  if(!strstr(server_name, "wifidog"))
      {
        if( ad_wall("restart") < 0)
          {
            printf("restart wifidog fail!\n");
            return -1;
          }
        else
          {
            printf("restart wifidog success!\n");
            return 0;
          }

      }
    else if(!strstr(server_name, "zmeng"))
      { //zmeng?

        if(zmeng("restart") < 0)
          {
            printf("restart zmeng fail!\n");
            return -1;
          }
        else
          {
            printf("restart zmeng success!\n");
            return 0;
          }
      }
    else
      {
        printf("undefined server name :%s",server_name);
        return 0;
      }



#endif

#ifdef TT
#endif
  return 0;

}

/*upgrade the fireware */
int
z_upgrade ( )
{

#ifdef DD




#endif
#ifdef TT
#endif
  return 0;

}

/*reboot the systerm */
int
z_reboot ( )
{

#ifdef DD
#endif
#ifdef TT
#endif
  return 0;
}

