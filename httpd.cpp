#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

bool return_file(int client, char* file_name)
{
	FILE* return_file=fopen(file_name,"r");
	if(return_file==NULL)
	{
		printf("can not open file.\n");
		exit(1);
	}
	char return_buf[1024];
	while(!feof(return_file))
	{
		fgets(return_buf,sizeof(return_buf),return_file);
		send(client,return_buf,strlen(return_buf),0);
	}
	if(fclose(return_file))
	{
		printf("can not close file.\n");
		exit(1);
	}
}

int main()
{
	printf("Start a socket.\n");
	int sock_id;
	sock_id=socket(AF_INET,SOCK_STREAM,0);
	if(sock_id<0)
	{
		printf("error in creating a socket.\n");
		return 1;
	}	
	struct sockaddr_in serv_addr;
	int port_num=4000;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(port_num);
	if(bind(sock_id,(const struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
	{
		printf("error in binding the socket.\n");
		return 1;
	}
	listen(sock_id,5);
	printf("listen on the port.\n");

	struct sockaddr_in cli_addr;
	int new_sock_id;
	unsigned int cli_length;
	while(1)
	{
		new_sock_id=accept(sock_id,(struct sockaddr*)&cli_addr,&cli_length);
		char *ip = inet_ntoa(cli_addr.sin_addr);
		printf("client ip:%s\n",ip);
		printf("%d\n",cli_addr.sin_port);
		char buf[1024];
		strcpy(buf,"HTTP/1.0 200 OK\r\n");
		send(new_sock_id,buf,strlen(buf),0);
		sprintf(buf, "Content-Type: text/html\r\n");
		send(new_sock_id, buf, strlen(buf), 0);
		strcpy(buf, "\r\n");
		send(new_sock_id, buf, strlen(buf), 0);
		return_file(new_sock_id,"index.html");
		close(new_sock_id);
	}	
	close(sock_id);
	return 0;
}