#include <stdio.h>
#include <stdlib.h>
#include "z_shell.h"

//buf should have space
int
z_shell ( char *cmd )
{
  if (cmd == NULL)
    {
      fprintf(stderr, "para illegal\n");
      return -1;
    }
  system(cmd);//fix me ,if fail!
return 0;
}

int
z_shell_buf ( char *cmd , char *buf )
{
  FILE *fstream = NULL;

  char *line_reader = malloc(MAX_BUF);
  if (line_reader == NULL)
    {
      fprintf(stderr, "malloc fail!\n");
      return -1;
    }

  memset(line_reader, 0, sizeof(line_reader));
  buf[0] = 0;

  if (NULL == (fstream = popen(cmd, "r")))
    {
      fprintf(stderr, "execute command failed: %s", "kkk");
      return -1;
    }
  while (NULL != fgets(line_reader, sizeof(line_reader), fstream))
    {
      strcat(buf, line_reader);
    }

  pclose(fstream);

  return 0;

}

