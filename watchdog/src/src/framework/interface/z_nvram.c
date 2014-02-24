#include <stdio.h>
#include <stdlib.h>
#include <z_nvram.h>


char *
z_nvram_get ( const char *name )
{
  char *res = NULL;
  if (!name)
    {
      printf(stderr, "para illegal!");
      return NULL;
    }
#ifdef DD

  res = _z_nvram_get(name);
  if(!res)
    {
      fprintf(stderr,"nvram_get fail!\n");
      return NULL:
    }

  return res;
#endif

#ifdef TT

  res = _z_nvram_get(name);
  if(!res)
    {
      fprintf(stderr,"nvram_get fail!\n");
      return NULL;
    }
#endif
  return res;

}
int
z_nvram_set ( const char *name , const char *value )
{
  if (!name || !value)
    {
      fprintf(stderr, "para is illegle");
      return 1;
    }
#ifdef DD

  int res;
  res = _z_nvram_set(name,value);
  if(res)
    {
      fprintf(stderr,"nvram_set fail!\n");
      return 1;
    }


#endif 
#ifdef TT
  int res;
  res = _z_nvram_set(name,value);
  if(res)
    {
      fprintf(stderr,"nvram set fail!\n");
      return 1;
    }

#endif
  return 0;

}

int
z_nvram_unset ( const char *name )
{
  if (!name)
    {
      fprintf(stderr, "para is illegle");
      return 1;
    }
#ifdef DD
  int res;
  res = _z_nvram_unset(name);
  if(res)
    {
      fprintf(stderr,"nvram_unset fail!\n");
      return 1;
    }
#endif
#ifdef TT
  int res;
  res = _z_nvram_unset(name);
  if(res)
    {
      fprintf(stderr,"nvram unset fail!\n");
      return 1;
    }
#endif
  return 0;
}

int
z_nvram_commit ( void )
{
#ifdef DD
  int res;
  res = _z_nvram_commit();
  if(res)
    {
      fprintf(stderr,"nvram_commit fail!\n");
      return 1;
    }
#endif
#ifdef TT
  int res;
  res = _z_nvram_commit();
  if(res)
    {
      fprintf(stderr,"nvram commit fail!\n");
      return 1;
    }
#endif
  return 0;

}
