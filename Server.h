#ifndef SERVER_H
#define SERVER_H

#include "util.h"
#include <exception>

using namespace std;

struct worker_param
{
	worker_param(int& _cli_sock,struct sockaddr_in& _cli_addr)
	{
		cli_sock=_cli_sock;
		cli_addr=_cli_addr;
	};
	int cli_sock;
	struct sockaddr_in cli_addr;
};

class Server
{
public:
	Server(int _port,int _queue_len):port(_port),queue_len(_queue_len)
	{

	};
	void start_server();
private:
	size_t port,queue_len;
	int listen_sock;
	struct sockaddr_in serv_addr;
	void* worker(void* param);
};

#endif