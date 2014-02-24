#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
char * hello(char* name)
{
	if(!name)
	{
		puts("NULLL");
		return "NULL";
	}
        char *string = malloc(100);
        strcpy(string,"hello ");
        strcat(string,name);
	printf("first %s\n",string);
        return string;
}



int main()
{
	char arr[100] = "kevin";

        char * string  =  NULL;
        string = hello(NULL);
        puts(string);
	//who();
return 0;
}

