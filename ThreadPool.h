#ifndef THREADPOOL__H
#define THREADPOOL__H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <functional>

using namespace std;

struct Job
{
	void* (*job_function)(void*);
	void* job_param;
	int job_id;

	Job(void* (*_job_function)(void*),void* _job_param,int _job_id)
	{
		job_function=_job_function;
		job_param=_job_param;
		job_id=_job_id;
	}

	//we can just call the struct to run the job
	void operator()()
	{
		job_function(job_param);
	}
};

class ThreadPool
{
public:
	ThreadPool(int num_thread);
	bool start_pool();
	bool add_job(void*(*job_function)(void*),void* job_param);
	bool shutdown();
	//this will wait until the pool finish all the jobs
	void join_pool();
private:
	int global_count;
	//save the pointer of job to speed up the assignment
	list<struct Job*> job_list;
	list<thread*> threads;
	//mutex to control accessing of pool and accessing if contion variable
	mutex pool_mutex;
	mutex empty_mutex;
	//main loop for all threads in pool
	void inner_loop();
};

#endif