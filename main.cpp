#include "Server.h"

int main()
{
	Server myserver(4000,1000);
	myserver.start_server();
	return 0;
}

