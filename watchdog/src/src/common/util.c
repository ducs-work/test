/*
 * util.c
 *
 *  Created on: 2014年2月14日
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "http.h"
#include "util.h"
#include <errno.h>
#include "z_nvram.h"

#define MAX_BUF 10240

int
ping ( )
{ //http://cif.zmeng123.cn/api/ping

  char *host = strdup("cif.zmeng123.cn");
  char * path = strdup("/api/ping");
  int port = 80;
  char *response;
  if (response == NULL)
    {
      fprintf(stderr, "malloc fail!\n");
      return -1;
    }

  http_connect(host, path, port, &response);

  if (strstr(response, "pong") == NULL)
    {
      free(response);
      return -1;
    }
  free(response);
  return 0; //pong

}

int
check_server_block ( )
{
  int res = -1;
  while (1)
    {
      res = ping();
      if (res == 0)
        {

          break;
        }
      else
        {
          sleep(3);

          puts("wait until pong the server");
        }
    }
  return 0;
}

int
check_N_server ( int num )
{
  int res = -1;
  int flag = 0;
  while (num--)
    {
      res = ping();
      if (res == 0)
        {
          flag = 1;
          break;
        }
      else
        {
          sleep(3);
          puts("wait until pong the server");
        }
    }
  if (!flag)
    {
      return -1;
    }
  else
    return 0;
}

int
get_my_mac_sock ( const char * ifname , unsigned char * mac )
{
//mac and ifname must have space, store mac in mac[6];
  int sock, ret;
  struct ifreq ifr;
  sock = socket( AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    {
      perror("socket");
      return -1;
    }
  memset(&ifr, 0, sizeof(ifr));
  strcpy(ifr.ifr_name, ifname);
  ret = ioctl(sock, SIOCGIFHWADDR, &ifr, sizeof(ifr));
  if (ret == 0)
    {
      memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
    }
  else
    {
      perror("ioctl");
    }
  close(sock);
  return ret;

  return 0;
}

char *
get_my_mac ( void )
{
  return z_nvram_get("et0macaddr");

}
