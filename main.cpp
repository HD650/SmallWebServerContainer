#include "Server.h"
#include "ThreadPool.h"

void* testf(void* param)
{
	int* temp=(int*)param;
	int a=0;
	printf("%d\n",*temp);
	//for(int i=0;i<1000000;i++)
	//{
		//a++;
	//}
}

int main()
{
	ThreadPool test(10);
	//Server myserver(4000,1000);
	//myserver.start_server();
	test.start_pool();
	for(int i=0;i<20000;i++)
	{
		int ii=i;
		test.add_job(testf,(void*)&ii);
	}
	test.join_pool();
	printf("all finished!");
	return 0;
}

