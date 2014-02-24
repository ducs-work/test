#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main()
{
	char * p, *q;
	p = malloc(sizeof(char)*20);
	q = p;
	scanf("%s%s",p,q);
	printf("%s%s\n",p,q);

return 0;
}
