#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <functional>
#include <atomic>
#include <list>
#include <memory>

#include "SyncQueue.h"

const int max_task_count = 10;
class ThreadPool
{
public:
	using Task = std::function<void()>;
	ThreadPool(int n_cpu = std::thread::hardware_concurrency());
	~ThreadPool();
	void Stop();
	void AddTask(Task&&);
	void AddTask(const Task&);
private:
	void Start(const int n_thread);
	void StopThreadGroup();
	void Runable();
private:	
	std::list<std::shared_ptr<std::thread>> _threads;
	SyncQueue<Task, std::mutex> _queue;
	std::once_flag _flag;
	std::atomic_bool _running;
	std::atomic_int _finish_task_count;
};

