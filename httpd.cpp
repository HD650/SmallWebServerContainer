#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex>

using namespace std;

void return_file(int client, char* file_name)
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

int get_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;

    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        /* DEBUG printf("%02X\n", c); */
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            i++;
        }
        else
            c = '\n';
    }
    buf[i] = '\0';

    return(i);
}

void parse_url(char* method,char* path,int sock_id){
	char buf[2048];
	size_t i=0,j=0;
	size_t num_char;
	num_char=get_line(sock_id,buf,sizeof(buf));
	while(buf[i]!=' '&&(i<num_char)){
		method[i]=buf[i];
		i++;
	}
	method[i]='\0';
	j=i;
	i=0;
	while(buf[j]==' '&&(j<num_char)){
		j++;
	}
	while(buf[j]!=' '&&(j<num_char)){
			path[i]=buf[j];
			i++;j++;
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
		char method[255],path[512];
		parse_url(method,path,new_sock_id);
		//printf("method: %s\n",method);
		//printf("path: %s\n",path);
		char *ip = inet_ntoa(cli_addr.sin_addr);
		//printf("client ip:%s\n",ip);
		//printf("%d\n",cli_addr.sin_port);
		char buf[1024];
		strcpy(buf,"HTTP/1.0 200 OK\r\n");
		send(new_sock_id,buf,strlen(buf),0);
		sprintf(buf, "Content-Type: text/html\r\n");
		send(new_sock_id, buf, strlen(buf), 0);
		strcpy(buf, "\r\n");
		send(new_sock_id, buf, strlen(buf), 0);
		return_file(new_sock_id,&path[1]);
		close(new_sock_id);
	}
	close(sock_id);
	return 0;
}
