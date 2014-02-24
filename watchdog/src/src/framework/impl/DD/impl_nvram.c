#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 1024


char * _z_nvram_get(const char *name)
{//name wouldn't be NULL
        FILE * fstream = NULL;
        char *line_reader = NULL;
        char cmd[50];

        strcpy(cmd,"nvram get ");
        strcat(cmd,name);
        printf("got shell cmd:%s\n",cmd);
        
        if((line_reader = (char *)malloc(MAX_BUF)) == NULL)
        {
                fprintf(stderr,"malloc fail!\n");
                return NULL;
        }

        if (NULL == (fstream = popen(cmd, "r")))
        {
                fprintf(stderr, "execute command failed");
                return -1;
        }

        if (NULL == fgets(line_reader, sizeof(line_reader), fstream))
        {
                exit(1);
        }
        pclose(fstream);
        return line_reader;

}




int  _z_nvram_set(const char *name, const char * value)
{//name wouldn't be NULL
        FILE * fstream = NULL;
        char *line_reader = NULL;
        char cmd[50];

	sprintf(cmd,"nvram set %s = %s",name,value);
        printf("got shell cmd:%s\n",cmd);

        if (NULL == (fstream = popen(cmd, "r")))
        {
                fprintf(stderr, "execute command failed");
                return -1;
        }
	else
	{
		printf("nvram set success!\n");
	}

        pclose(fstream);
        return 0;

}



int  _z_nvram_unset(const char *name)
{//name wouldn't be NULL
        FILE * fstream = NULL;
        char *line_reader = NULL;
        char cmd[50];

        sprintf(cmd,"nvram unset  %s ",name);
        printf("got shell cmd:%s\n",cmd);

        if (NULL == (fstream = popen(cmd, "r")))
        {
                fprintf(stderr, "execute command failed");
                return -1;
        }
        else
        {
                printf("nvram unset success!\n");
        }

        pclose(fstream);
        return 0;

}






int  _z_nvram_commit()
{//name wouldn't be NULL
        FILE * fstream = NULL;
        char *line_reader = NULL;
        char cmd[50];

        sprintf(cmd,"nvram commit ");
        printf("got shell cmd:%s\n",cmd);

        if (NULL == (fstream = popen(cmd, "r")))
        {
                fprintf(stderr, "execute command failed");
                return -1;
        }
        else
        {
                printf("nvram commit success!\n");
        }

        pclose(fstream);
        return 0;
}






