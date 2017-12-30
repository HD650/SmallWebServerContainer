#include "util.h"

bool return_file(int client, char* file_name)
{
	FILE* file=fopen(file_name,"rb");
	if(file==NULL)
	{
		//printf("can not open file.\n");
		return false;
	}

	int file_length=0;
	fseek(file,0,SEEK_END);
	file_length=ftell(file);
	//printf("file size:%d bytes.\n",file_length);
	fseek(file,0,SEEK_SET);

	char return_buf[1024];
	int send_count=0;
	int curr_send=0;
	while(send_count<file_length)
	{
		curr_send=fread(return_buf,1,sizeof(return_buf),file);
		send(client,return_buf,curr_send,0);
		send_count+=curr_send;
		//printf("already send %d bytes to client.\n",send_count);
	}

	if(fclose(file))
	{
		printf("can not close file.\n");
		return false;
	}
	return true;
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
	size_t i=0,j=0,num_char;
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
			path[i++]=buf[j++];
	}
	path[i]='\0';
}
