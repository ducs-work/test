#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

	char * str  = malloc(100);
	sprintf(str, "todayisfriday%d\n",777);
	char arr[60];
	strcpy(arr, "kevin");
	strcat(arr, str);
	puts(arr);	
free(str);

return 0;

}
