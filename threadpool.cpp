#include "threadpool.h"

using namespace std;

ThreadPool::ThreadPool() {
	
}

ThreadPool::ThreadPool(int nbrThreads) {
	
	shutdown = false;
	threads.reserve(nbrThreads);
	for (int i = 0; i < nbrThreads; i++) {
		threads.emplace_back(std::bind(&ThreadPool::entry, this, i));
	}
	
	
}


ThreadPool::~ThreadPool() {
	
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

void ThreadPool::wait() {
	
	for (auto & thread : threads) {
		thread.join(); //wait all threads 
	}
}

void ThreadPool::stop() {
	unique_lock <std::mutex> loc(queueLock);
	shutdown = true;
	condVar.notify_all();
}

void ThreadPool::addJob(function <void(void)> func) {
	
	
	{
		//cout << "add job\n" << flush;
		unique_lock <mutex> lock(queueLock);
		jobs.push_back(func);
	}
	condVar.notify_one();
	
	//cout << jobs.size() <<endl;
	
}

void ThreadPool::entry(int i) {
	
	function <void (void)> job;
	
	while(true) {
		{
			unique_lock<mutex> lock(queueLock);
			//cout << "while\n" << flush;
			
			//cout << shutdown << endl << flush;
			if (shutdown && jobs.empty()) {
				
				cout << "Thread " << i << " has terminated\n";
				return ;
			}
			
			
			while (!shutdown && jobs.empty()) { //if no jobs available and it doesn't have to shutdown -> wait
				condVar.wait(lock);
			}
			
			
			
			//cout << "Thread " << i << endl;
			job = jobs.front();
			jobs.pop_front();
			
			/*if (jobs.size() > 36900 || jobs.size() < 20) {
				cout << "size : " << jobs.size() <<endl;
			}*/
		}
		
		job(); //all locks are removed when doing the job
	}
}
