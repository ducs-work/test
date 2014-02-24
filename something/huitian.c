#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>


void haha(char * name , char **buf)
{
	buf[0] = 0;
	char * string  =  malloc(100);
	strcpy(string,name);
	strcat(string,"today is friday");
	buf = &string;
	free(string);
}


int main()
{
	char *buf =  malloc(1024);
	haha("kevin ",&buf);
	puts(buf);
	



return 0;
}
