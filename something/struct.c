#include <stdlib.h>
#include <stdio.h>

typedef struct student
{
	int age;
	int id;
	char *sex;
	char name[30];
}stu;


int main()
{
	stu aa;
	printf("SIZEOF =%lld\n",sizeof(aa));
return 0;

}



