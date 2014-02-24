#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <syslog.h>
#include <netdb.h>

#include "http.h"

static pthread_mutex_t ghbn_mutex = PTHREAD_MUTEX_INITIALIZER;

int
connect_server ( t_wd_serv * server )
{
  int sockfd;

  sockfd = _connect_server(server);

  if (sockfd == -1)
    {
      return -1;

    }
  else
    {

    }
  return (sockfd);
}

struct in_addr *
wd_gethostbyname ( const char *name )
{
  struct hostent *he;
  struct in_addr *h_addr, *in_addr_temp;

  /* XXX Calling function is reponsible for free() */

  h_addr = malloc(sizeof(struct in_addr));

  LOCK_GHBN()
  ;

  he = gethostbyname(name);

  if (he == NULL)
    {
      free(h_addr);
      UNLOCK_GHBN()
      ;
      return NULL;
    }

  in_addr_temp = (struct in_addr *) he->h_addr_list[0];
  h_addr->s_addr = in_addr_temp->s_addr;

  UNLOCK_GHBN()
  ;

  return h_addr;
}

/* Helper function called by connect_auth_server() to do the actual work including recursion
 * DO NOT CALL DIRECTLY
 @param level recursion level indicator must be 0 when not called by _connect_auth_server()
 */

int
_connect_server ( t_wd_serv * server )
{

  struct in_addr *h_addr;
  struct in_addr *h_addr1;
  int num_servers = 0;
  char * hostname = NULL;
  char * popular_servers[] =
            { "www.google.com", "www.yahoo.com",
            NULL };
  char ** popularserver;
  char * ip;
  struct sockaddr_in their_addr;
  int sockfd;

  /*
   * Let's resolve the hostname of the top server to an IP address
   */

  hostname = server->wd_serv_hostname;

  h_addr = wd_gethostbyname(hostname);
  if (!h_addr)
    {
      /*
       * DNS resolving it failed
       *
       * Can we resolve any of the popular servers ?
       */

      for (popularserver = popular_servers; *popularserver; popularserver++)
        {
          h_addr = wd_gethostbyname(*popularserver);
          if (h_addr)
            {
              break;
            }
          else
            {
            }
        }

      /*
       * If we got any h_addr buffer for one of the popular servers, in other
       * words, if one of the popular servers resolved, we'll assume the DNS
       * works, otherwise we'll deal with net connection or DNS failure.
       */
      if (h_addr)
        {
          free(h_addr);
          /*
           * Yes

           */

          //try again
          //  return _connect_auth_server(level);
        }
      else
        {
          /*
           * No
           * It's probably safe to assume that the internet connection is malfunctioning
           * and nothing we can do will make it work
           */
          return (-1);
        }
    }
  else
    {
      /*
       * DNS resolving was successful
       */
      ip = strdup(inet_ntoa(*h_addr));

      their_addr.sin_family = AF_INET;

      their_addr.sin_port = htons(server->wd_serv_http_port);

      their_addr.sin_addr = *h_addr;
      memset(&(their_addr.sin_zero), '\0', sizeof(their_addr.sin_zero));
      free(h_addr);

      if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
          return (-1);
        }

      if (connect(sockfd, (struct sockaddr *) &their_addr, sizeof(struct sockaddr)) == -1)
        {

          close(sockfd);

          return -1; /* Yay recursion! */
        }
      else
        {
          /*
           * We have successfully connected
           */

          return sockfd;
        }
    }

}

int
http_connect ( char * host , char * path , int port , char * response )
{
//http://cif.zmeng123.cn/api/ping
//hostname :cif.zmeng123.cn
//path:/api/ping
//port:80

  t_wd_serv *server = malloc(sizeof(t_wd_serv));
if(server == NULL)
{
	fprintf(stderr, "malloc fail!\n");
	exit(1);
}
  server->wd_serv_hostname = strdup(host);
  server->wd_serv_path = strdup(path);
  server->wd_serv_http_port = port;

  ssize_t numbytes;
  size_t totalbytes;
  int sockfd, nfds, done;
  char request[MAX_BUF];
  fd_set readfds;
  struct timeval timeout;
  FILE * fh;
  unsigned long int sys_uptime = 0;
  unsigned int sys_memfree = 0;
  float sys_load = 0;

  memset(response, 0, 10240);

  //get_serv();

  sockfd = connect_server(server);
  if (sockfd == -1)
    {

      return  -1;
    }

  /*
   * Prep & send request
   */
  snprintf(request, sizeof(request) - 1,
          // "GET %s%sgw_id=%s&sys_uptime=%lu&sys_memfree=%u&sys_load=%.2f HTTP/1.0\r\n"
          "GET %s HTTP/1.0\r\nConnection:   Close \r\n"
                  "User-Agent: WatchDog \r\n"
                  "Host: %s\r\n"
                  "\r\n",
          server->wd_serv_path,
          server->wd_serv_hostname);

  send(sockfd, request, strlen(request), 0);

  numbytes = totalbytes = 0;
  done = 0;
  do
    {
      FD_ZERO(&readfds);
      FD_SET(sockfd, &readfds);
      timeout.tv_sec = 30; /* XXX magic... 30 second */
      timeout.tv_usec = 0;
      nfds = sockfd + 1;

      nfds = select(nfds, &readfds, NULL, NULL, &timeout);

      if (nfds > 0)
        {
          /** We don't have to use FD_ISSET() because there
           *  was only one fd. */
          numbytes = read(sockfd, request + totalbytes, MAX_BUF - (totalbytes + 1));
          if (numbytes < 0)
            {

              /* FIXME */
              close(sockfd);
              return -1;
            }
          else if (numbytes == 0)
            {
              done = 1;
            }
          else
            {
              totalbytes += numbytes;
            }
        }
      else if (nfds == 0)
        {
          /* FIXME */
          close(sockfd);
          return -1;
        }
      else if (nfds < 0)
        {
          /* FIXME */
          close(sockfd);
          return -1;
        }
    }
  while (!done);
  close(sockfd);
  request[totalbytes] = '\0';
	printf("DATE:%s\n",request);
	free(server);


  return 0;
}


/*
 int
 http2serv ( char *sip , char *host , long sport , char *surl , char *response )
 {
 int sockfd;
 ssize_t numbytes;
 size_t totalbytes, totalbytes2;
 int totalsend = 0;
 char buffer[16] = "";

 struct sockaddr_in servaddr;
 u_char requestSession[4096];
 u_char response_header[1024];
 int tmp_int;
 int nNetTimeout = 1000;

 int headlen = 0;
 memset(requestSession, 0, 4096);
 sprintf(requestSession, "POST /%s HTTP/1.0\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\nConnection:   Close\r\n\r\n", surl, host, plen);
 headlen = strlen(requestSession);
 memcpy(requestSession + headlen, pdata, plen);

 if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
 {
 fprintf(stderr, "创建网络连接失败,本线程即将终止---socket error!\n");
 return -1;
 };

 //    setsockopt(socket, SOL_S0CKET,SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
 //    setsockopt(socket, SOL_S0CKET,SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

 bzero(&servaddr, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_port = htons(sport);
 if (inet_pton(AF_INET, sip, &servaddr.sin_addr) <= 0)
 {
 fprintf(stderr, "创建网络连接失败,本线程即将终止--inet_pton error!\n");
 return -1;
 };

 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
 {
 fprintf(stderr, "连接到服务器失败,connect error!\n");
 return -1;
 }

 numbytes = totalbytes = 0;
 while (totalbytes < headlen + plen)
 {
 numbytes = write(sockfd, requestSession + totalbytes, headlen + plen - totalbytes);
 if (numbytes == -1)
 {
 printf("send error!%s\n ", strerror(errno));
 exit(0);
 }
 totalbytes += numbytes;
 }

 memset(response, 0, 10240);
 numbytes = totalbytes = totalbytes2 = 0;

 tmp_int = 0;
 while ((numbytes = read(sockfd, buffer, 1)) == 1)
 {
 if (tmp_int < 4)
 {
 if (buffer[0] == '\r' || buffer[0] == '\n')
 {
 tmp_int++;
 }
 else
 {
 tmp_int = 0;
 }
 *(response_header + totalbytes2) = buffer[0];
 totalbytes2 += 1;
 }
 else
 {
 (response + totalbytes)[0] = buffer[0];
 totalbytes += 1;
 }
 }

 *(response + totalbytes) = 0;
 *(response_header + totalbytes2) = 0;

 close(sockfd);

 if (!strstr(response_header, "200 OK"))
 {
 printf("-----------------\n%s", response_header);
 return -1;
 }
 return 0;
 }
 */

