#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pipe.h"
#include "msg_unit.h"




int main()
{
	create_queue();
	msg_t * msg_pack = msg_create(15, "my name is kevin");
	msg_t * msg_get = malloc(sizeof(msg_t));
	if(msg_pack == NULL)
	{
		fprintf(stderr, "msg_create fail!\n");
		exit(1);
	}

	
	push_pipe(msg_pack,1);
	pull_pipe(msg_get,1);
	printf("msg_get :%d\t %s\n",(int)msg_get->key,(char *)msg_get->value);
	free(msg_pack);
	free(msg_get);

return 0;


}
