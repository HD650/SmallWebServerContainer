#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
	printf("Start a socket.\n");
	int sock_id;
	sock_id=socket(AF_INET,SOCK_STREAM,0);
	if(sock_id<0)
		printf("error in creating a socket.\n");
	struct sockaddr_in serv_addr;
	int port_num=4000;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(port_num);
	if(bind(sock_id,(const struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
		printf("error in binding the socket.\n");
	listen(sock_id,5);
	printf("listen on the port.\n");

	struct sockaddr_in cli_addr;
	int new_sock_id;
	unsigned int cli_length;
	while(1)
	{
		new_sock_id=accept(sock_id,(struct sockaddr*)&cli_addr,&cli_length);
		printf("%d\n",cli_addr.sin_addr);
		printf("%d\n",cli_addr.sin_port);
	}
	return 0;
}