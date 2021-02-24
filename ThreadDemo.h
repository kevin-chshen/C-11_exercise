#pragma once

#include <vector>
#include <memory>
#include <thread>
#include <mutex>

class ThreadDemo
{
public:
	void run();

private:
	void foo();

	void threads();

	void thread_pool();

	void thread_mutex();

	void thread_condition_mutex();

	void thread_atomic();

	void thread_future();

	void thread_join();
private:
	std::mutex _mutex_lock;
	std::vector<std::thread> g_list;
	std::vector<std::shared_ptr<std::thread>> g2_list;
};

