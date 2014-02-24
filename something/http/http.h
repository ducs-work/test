#ifndef __HTTP_H__
#define __HTTP_H__

#define STATUS_BUF_SIZ  16384
#define PING_HOST "cif.zmeng123.cn"
#define MAX_BUF 10240
#define debug(level, format...) _debug(__FILE__, __LINE__, level, format)

typedef struct _wd_serv_t {
  char *wd_serv_hostname;
  char *wd_serv_path;
  int wd_serv_http_port;
} t_wd_serv;

int
http_connect ( char * host , char * path , int port , char *response );

#define LOCK_GHBN() do {\
        pthread_mutex_lock(&ghbn_mutex); \
} while (0)

#define UNLOCK_GHBN() do { \
        pthread_mutex_unlock(&ghbn_mutex); \
} while (0)


#endif
