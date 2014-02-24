#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "a.h"

int main(char argc, char **argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage...\n");
	}
	else
		printf("hello %s\n",argv[1]);

	char name[100];
	function(name);
	
	puts(name);

return 0;
}
