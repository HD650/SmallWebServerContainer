#include "Server.h"

//TODO: change to epoll mode
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

	//init kqueue
	int kq = kqueue();
	//we only need to add a event one time, so size 1 is enough
	struct kevent changes[1];
	//to get the ready sockets, we need a lot slots since there may be a lots client at a time
	struct kevent events[1000];
	//set the first event as our listening  socket
	EV_SET(&changes[0],listen_sock,EVFILT_READ,EV_ADD,0,0,NULL);
	//add the base listening socket event to kqueue, this line will not block
	int ret=kevent(kq,changes,1,NULL,0,NULL); 

	//the ready socket file descriptor
	int fd;
	//the size of the ready socket
	int data;
	try
	{
		
		while(1)
		{
			//wait on all the kqueue
			ret=kevent(kq,NULL,0,events,1000,NULL);
			//manage all ready sockets
			for(int i=0;i<ret;i++)
			{
				fd=events[i].ident; 
		       	data=events[i].data; 
				struct sockaddr_in cli_addr;
				//if the listen socket is ready, that means there are new clients (one or more)
				if ((fd==listen_sock))
				{
					//for every new client, registe them in the kqueue
					for(int ii=0;ii<data;ii++)
					{
						int cli_sock;
						unsigned int cli_length;
						cli_sock=accept(fd,(struct sockaddr*)&cli_addr,&cli_length);
						EV_SET(&changes[0],cli_sock,EVFILT_READ,EV_ADD,0,0,NULL);
						kevent(kq,changes,1,NULL,0,NULL); 
					}
				}
				//old client, but new request, handle the request with worker function
				else
				{
					//old client
					struct worker_param cli_param(fd,cli_addr);
					//TODO: use multi thread here to test the effeicient
					worker((void*)&cli_param);
				}
			}
			//old code for multi thread server
			/*
			struct sockaddr_in cli_addr;
			int cli_sock;
			unsigned int cli_length;
			cli_sock=accept(listen_sock,(struct sockaddr*)&cli_addr,&cli_length);
			
			pthread_t pid;

			struct worker_param cli_param(cli_sock,cli_addr,pid);
			//this->worker((void*)&cli_param);
			pthread_create(&pid,NULL,worker,(void*)&cli_param);
			*/
		}
	}
	catch(exception& error)
	{
		printf("%s\n",error.what());
		close(listen_sock);

	}
	return;
}

void* worker(void* param)
{
	struct worker_param input=*((struct worker_param*)param);
	char method[8],path[1024];
	
	parse_url(method,path,input.cli_sock);
	printf("method: %s\n",method);
	printf("path: %s\n",path);
	
	char *ip = inet_ntoa(input.cli_addr.sin_addr);
	printf("client ip:%s\n",ip);
	printf("%d\n",input.cli_addr.sin_port);
	char buf[1024];
	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	send(input.cli_sock,buf,strlen(buf),0);
	strcpy(buf, "\r\n");
	send(input.cli_sock,buf,strlen(buf),0);
	
	return_file(input.cli_sock,&path[1]);
	close(input.cli_sock);
	return 0;
}