#include "Server.h"

int main()
{
	Server myserver(4000,5);
	myserver.start_server();
	return 0;
}

