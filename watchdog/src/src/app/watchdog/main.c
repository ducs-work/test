#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <pthread.h>

#include "monitor.h"
#include "process.h"
#include "receive.h"

#include "util.h"
#include "thttpd.h"
#include "zmeng.h"

static pthread_t tid[3];

void
sigchld_handler ( int s )
{
  int status;
  pid_t rc;

  //////debug(LOG_//////debug, "Handler for SIGCHLD called. Trying to reap a child");

  rc = waitpid(-1, &status, WNOHANG);

  //////debug(LOG_//////debug, "Handler for SIGCHLD reaped child PID %d", rc);
}

void
termination_handler ( int s )
{
  static pthread_mutex_t sigterm_mutex = PTHREAD_MUTEX_INITIALIZER;

  if (pthread_mutex_trylock(&sigterm_mutex))
    {
      //debug(LOG_INFO, "Another thread already began global termination handler. I'm exiting");
      pthread_exit(NULL);
    }
  else
    {
      //debug(LOG_INFO, "Cleaning up and exiting");
    }

  if (tid[0])
    {
      //debug(LOG_INFO, "Explicitly killing the fw_counter thread");
      pthread_kill(tid[0], SIGKILL);
    }
  if (tid[1])
    {
      //debug(LOG_INFO, "Explicitly killing the fw_sync thread");
      pthread_kill(tid[1], SIGKILL);
    }
  if (tid[2])
    {
      //debug(LOG_INFO, "Explicitly killing the ping thread");
      pthread_kill(tid[2], SIGKILL);
    }
  //debug(LOG_NOTICE, "Exiting...");
  exit(s == 0 ? 1 : 0);
}

int
regist_me ( )
{

  char *et0mac = NULL;
  char * m_model = NULL;
  char * m_ver = NULL;

  et0mac = get_my_mac();
  if (et0mac == NULL)
    {
      printf("get mac fail!\n");
      return -1;

    }

  FILE * fp;
  unsigned long m_model;
  unsigned long m_ver;

  if ((fp = fopen("/usr/sbin/m_model", "r")))
    {
      fscanf(fp, "%lu", &m_model);
      fclose(fp);

    }
  else
    {
      printf("fopen fail!\n");
      return -1;

    }

  if ((fp = fopen("/usr/sbin/m_ver", "r")))
    {
      fscanf(fp, "%lu", &m_ver);
      fclose(fp);

    }
  else
    {
      printf("fopen fail!\n");
      return -1;

    }
  char path = malloc(100);
  sprintf(path, "/api/add_me/?mac=%s&model=%lu&ver=%lu", et0mac, m_model, m_ver);
  char * response;
  while (1)
    {
      if (!http_connect("cif.zmeng123.cn", path, 80, &response))
        {
          if (strstr(response, "error"))
            {
              printf("return error\n");

            }
          else if (strstr(response, "added"))
            {
              printf("added already!\n");
              break;
            }
          else
            {
              char tmp[100];
              strcpy(tmp, "ZMDevice_0");
              strcat(tmp, response);
              z_nvram_set("router_name", tmp);
              z_nvram_set("lan_ipaddr", "192.168.2.1");
              break;

            }
        }
      sleep(1);
    }
  free(path);
  free(response);
  free(et0mac);

  /*
   et0mac = `nvram
   get et0macaddr
   `
   rmodel = `cat / usr / sbin / m_model
   `
   rver = `cat / usr / sbin / m_ver
   `
   while [ true ]
   do
   {
   m_nameid=`wget -qO - "http://$CIFHOST/api/add_me/?mac=$et0mac&model=$rmodel&ver=$rver"`
   if [ "$m_nameid" = "error" ];then
   echo $m_nameid
   elif [ "$m_nameid" = "added" ];then
   echo "router added"
   break;
   elif [ `zbusybox is_num $m_nameid` = "1" ];then
   nvram set router_name=ZMDevice_0$m_nameid
   nvram set lan_ipaddr=192.168.2.1
   return 1;
   break;
   fi
   sleep 1
   }
   done
   */
  return 0;

}

void
onfactory_init ( )
{
  char *buf = NULL;
  int num = 10;

  buf = z_nvram_get("online_setup");
  if (strstr(buf, "1"))
    {
      printf(stderr, "online registed");
    }
  else //it's new device
    {
      if (!check_server_block())
        { //block until ping and return 0
          if (!regist_me())
            { //regist success
              z_nvram_set("online_setup", "1");
              z_nvram_commit();
            }
          else
            {
              puts("regist fail!");
              //do nothing fxi me
            }
        }
    }
  free(buf);
}

void
onstart_init ( )
{
  /*
   insmod xt_mac
   insmod xt_mark
   insmod ipt_webstr
   insmod ipt_ipp2p
   insmod nf_nat_pptp
   insmod nf_conntrack_pptp
   insmod nf_nat_proto_gre
   */

  char cmd[50] =
            { '\0' };
  sprintf(cmd, "cp -fr /usr/sbin/webroot/* /tmp/");
//cp -fr /usr/sbin/webroot/* /tmp/

//nvram set no_crossdetect=1
  if (z_nvram_set("no_crossdetect", "1") < 0)
    {
      fprintf(stderr, "nvram set no_crossdetect fail!\n");
      //return;
    }
  if (z_nvram_commit() < 0)
    {
      fprintf(stderr, "nvram commit  fail!\n");
      //return
    }

  z_shell(cmd);

}

/*
 * Registers all the signal handlers
 */
static void
init_signals ( void )
{
  struct sigaction sa;

  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
      ////////debug(LOG_ERR, "sigaction(): %s", strerror(errno));
      exit(1);
    }

  /* Trap SIGPIPE */
  /* This is done so that when libhttpd does a socket operation on
   * a disconnected socket (i.e.: Broken Pipes) we catch the signal
   * and do nothing. The alternative is to exit. SIGPIPE are harmless
   * if not desirable.
   */
  sa.sa_handler = SIG_IGN;
  if (sigaction(SIGPIPE, &sa, NULL) == -1)
    {
      ////////debug(LOG_ERR, "sigaction(): %s", strerror(errno));
      exit(1);
    }

  sa.sa_handler = termination_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  /* Trap SIGTERM */
  if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
      ////////debug(LOG_ERR, "sigaction(): %s", strerror(errno));
      exit(1);
    }

  /* Trap SIGQUIT */
  if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
      ////////debug(LOG_ERR, "sigaction(): %s", strerror(errno));
      exit(1);
    }

  /* Trap SIGINT */
  if (sigaction(SIGINT, &sa, NULL) == -1)
    {
      ////////debug(LOG_ERR, "sigaction(): %s", strerror(errno));

    }

}

int
main_loop ( )
{

  onfactory_init();
  onstart_init();

  start_http();
  start_zmeng();

  //ready to throw 3 pthread
  int res = 0;

  res = pthread_create(&tid[0], NULL, (void *) monitor, NULL);
  if (res != 0)
    {
      printf("pthread_create monitor fail!\n");
      termination_handler(0);

    }
  pthread_detach(tid[0]);

  res = pthread_create(&tid[1], NULL, (void *) receive, NULL);
  if (res != 0)
    {
      printf("pthread_create receive fail!\n");
      termination_handler(0);

    }
  pthread_detach(tid[1]);

  while (1)
    {
      /*
       * pull pipe
       *
       * pthread_create(&tid[2], NULL, (void *) process, msg_t);
       * pthread_join();......
       */

      //mutex con
      msg_t * msg_get;

      if (pull_pipe(&msg_get, 1) == NULL)
        {
          fprintf(stderr, "pull pipe fail!\n");
          continue;
        }

      res = pthread_create(&tid[2], NULL, (void *) process, (void *) msg_get);
      if (res != 0)
        {
          printf("pthread_create process fail!\n");
          termination_handler(0);

        }

      pthread_join(tid[2]);

    }
  /*need pthread_join()*/ ///fix  ducs
  return 0;

}

/**main process throw 3 pthread and wait **/
int
main ( int argc , char **argv )
{

  //some initination

  init_signals();

  switch (fork())
  {
    case 0:
      setsid();

      main_loop();
      break;

    default:
      exit(0);
      break;
  }
}
