#include "threadpool.h"

using namespace std;

ThreadPool::ThreadPool() {
	
}

ThreadPool::ThreadPool(int nbrThreads) {
	/* Create a thread pool with nbrThreads threads
	 */
	 
	shutdown = false;
	threads.reserve(nbrThreads);
	for (int i = 0; i < nbrThreads; i++) {
		threads.emplace_back(std::bind(&ThreadPool::entry, this, i));
	}
}


ThreadPool::~ThreadPool() {
	/* Wait until all threads have finished their jobs. 
	 * When finished, delete all threads.
	 */
	{
		cout << "deleting pool" << flush << endl;
		unique_lock <std::mutex> loc(queueLock);
		shutdown = true;
	}
	condVar.notify_all();
	
	for (auto & thread : threads) {
		thread.join(); //wait all threads 
	}
}

void ThreadPool::addJob(function <void(void)> func) {
	/* Add a job to the queue
	 */
	{
		unique_lock <mutex> lock(queueLock); //lock the jobs vector
		jobs.push_back(func);
	}
	condVar.notify_one();
}

void ThreadPool::entry(int i) {
	/*
	 * Main fonction for the threads. This is where they are executing their jobs
	 * Infinite loop until there is no jobs or we want to shutdown
	 */
	function <void (void)> job;
	
	while(true) {
		{
			unique_lock<mutex> lock(queueLock); //lock the jobs vector
			
			if (shutdown && jobs.empty()) {
				return ; //if no job available and we want to shutdown : leave the infinite loop
			}
		
			while (!shutdown && jobs.empty()) { //if no jobs available and it doesn't have to shutdown -> wait
				condVar.wait(lock);
			}
			job = jobs.front();  //pick the first job
			jobs.pop_front(); //remove the first job from vector
			
		} //unlock the jobs vector
		
		job(); //all locks are removed when doing the job
	}
}
