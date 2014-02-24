#include <stdio.h>
#include <stdlib.h>
#include "http.h"
#include <string.h>

int main()
{//http://cif.zmeng123.cn/api/ping/
//http://cif.zmeng123.cn/api/adwall/auth
//http://www.360doc.com/readroom.html
	char *host = strdup("cif.zmeng123.cn");
puts("da");
	char *path = strdup("/api/ping");
puts("1d");
	char  *response = malloc(10240);
puts("1ddddddddd");
	http_connect (  host ,  path , 80, response );
	printf("got data:%s",response);
	free(response);
return 0;

}
