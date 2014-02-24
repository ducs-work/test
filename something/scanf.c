#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
int main()
{

	char * p, *q;
	p = malloc(sizeof(char) * 4);
	printf("%x\t%x\n",&p,&q);
	q  = p;
	scanf("%s%s",p,q);
	printf("%x\t%x\n",&p,*q);
	printf("%s%s\n",p,q);
return 0;
}
