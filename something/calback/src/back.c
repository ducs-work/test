#include <stdio.h>
#include <stdlib.h>
#include "back.h"

 int (*process[10])(void *);

int fun1(char * name )
{
	printf("my name is kevin %s\n",name);
return 0;
}



int regist(int num, void (*fun))
{
	process[num] = fun;
	printf("fun in regist  at %x\n",process[num]);

return num;

}


