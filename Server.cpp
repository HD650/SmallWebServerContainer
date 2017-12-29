#include "Server.h"

void Server::start_server()
{
	printf("Start a socket.\n");
	listen_sock=socket(AF_INET,SOCK_STREAM,0);
	if(listen_sock<0)
	{
		printf("error in creating a socket.\n");
		exit(1);
	}
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(port);
	if(bind(listen_sock,(const struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
	{
		printf("error in binding the socket.\n");
		exit(1);
	}
	listen(listen_sock,queue_len);
	printf("listen on the port.\n");

	try
	{
		while(1)
		{
			struct sockaddr_in cli_addr;
			int cli_sock;
			unsigned int cli_length;
			cli_sock=accept(listen_sock,(struct sockaddr*)&cli_addr,&cli_length);
			struct worker_param cli_param(cli_sock,cli_addr);
			this->worker((void*)&cli_param);
		}
	}
	catch(exception& error)
	{
		printf("%s\n",error.what());
		close(listen_sock);
	}
	return;
}

void* Server::worker(void* param)
{
	struct worker_param input=*((struct worker_param*)param);
	char method[8],path[1024];
	
	parse_url(method,path,input.cli_sock);
	printf("method: %s\n",method);
	printf("path: %s\n",path);
	
	char *ip = inet_ntoa(input.cli_addr.sin_addr);
	//printf("client ip:%s\n",ip);
	//printf("%d\n",cli_addr.sin_port);
	char buf[1024];
	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	send(input.cli_sock,buf,strlen(buf),0);
	strcpy(buf, "\r\n");
	send(input.cli_sock,buf,strlen(buf),0);
	
	return_file(input.cli_sock,&path[1]);
	close(input.cli_sock);
	return 0;
}