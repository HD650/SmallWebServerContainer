#include "Server.h"
#include "ThreadPool.h"


int main()
{
	Server myserver(4000,1000);
	myserver.start_server();
	printf("all finished!");
	return 0;
}

