#include "ThreadPool.h"

ThreadPool::ThreadPool(int num_thread)
{
	for(int i=0;i<num_thread;i++)
	{
		threads.push_back(new thread(std::bind(&ThreadPool::inner_loop,this)));
	}
	printf("pooling finished\n");
	global_count=0;
}

bool ThreadPool::start_pool()
{

	return true;
}

bool ThreadPool::add_job(void*(*job_function)(void*),void* job_param)
{
	pool_mutex.lock();
	global_count++;
	struct Job* temp=new struct Job(job_function,job_param,global_count);
	job_list.push_back(temp);
	if(job_list.size()==1)
	{
		empty_mutex.unlock();
	}
	pool_mutex.unlock();
}

bool ThreadPool::shutdown()
{

	for(auto i=threads.begin();i!=threads.end();i++)
	{

	}
}

void ThreadPool::join_pool()
{
	while(1)
	{
		pool_mutex.lock();
		if(job_list.empty())
		{
			return;
		}
		else
		{
			
		}
		pool_mutex.unlock();
	}
}

void ThreadPool::inner_loop()
{
	while(1)
	{
		pool_mutex.lock();
		if(job_list.empty())
		{
			pool_mutex.unlock();
			empty_mutex.lock();
		}
		else
		{
			//if the job list is not empty, call a job
			struct Job* temp=job_list.back();
			job_list.pop_back();
			pool_mutex.unlock();
			(*temp)();
			delete temp;
		}
	}
	return ;
}

