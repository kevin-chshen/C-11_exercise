#include "ThreadPool.h"

ThreadPool::ThreadPool(int n_cpu)
	:_queue(max_task_count, [](EnumEvent e) {
			std::cout << "______e := " << (int)e << "___" << std::endl;
		}),
	_finish_task_count(0)
{
	Start(n_cpu);
}

ThreadPool::~ThreadPool()
{
	Stop();
}

void ThreadPool::Stop()
{
	std::call_once(_flag, [this]() {
		std::cout << "call once stop" << std::endl;
		StopThreadGroup();
		});
}

void ThreadPool::AddTask(Task&& task)
{
	_queue.Put(std::forward<Task>(task));
}

void ThreadPool::AddTask(const Task& task)
{
	_queue.Put(task);
}

void ThreadPool::Start(const int n_thread)
{
	std::cout << "start" << std::endl;
	_running = true;

	for (int n = 0; n < n_thread; n++)
	{
		_threads.emplace_back(std::make_shared<std::thread>(
			&ThreadPool::Runable, this
		));
	}
}

void ThreadPool::Runable()
{
	std::list<Task> list;
	while (_running)
	{		
		_queue.Take(list);
		for (auto& task : list) {
			if (!_running)
				return;
			task();
			_finish_task_count++;
		}
		std::cout << "runable" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void ThreadPool::StopThreadGroup()
{
	_running = false;

	for (auto& thr : _threads) {
		thr->join();
	}
	_threads.clear();	

	_queue.Stop();
	std::cout << "_finish_task_count:= "
		<< _finish_task_count
		<< std::endl;
}