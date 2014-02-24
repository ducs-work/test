#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int function(char **name)
{
	char *str = malloc(100);
	strcpy(str, "todayisfriday");
	*name = str;

return 0;
}
int fun(char *name)
{
//	function(&name);
	char * str = malloc(100);

	strcpy(str, "todayisfriday");
name =str;
return 0;

}


