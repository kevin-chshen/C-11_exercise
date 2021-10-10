#include "ThreadDemo.h"
#include "ThreadPool.h"

#include <iostream>
#include <string>
#include <chrono>
#include <list>
#include <condition_variable>
#include <atomic>
#include <future>

void ThreadDemo::run()
{
	this->thread_pool();
}

void ThreadDemo::foo()
{
	std::thread t([=]() {
			std::cout << "run thread:= " << std::this_thread::get_id() << std::endl;
			while (true) {
				// sleep
			}
		});
	t.detach();
	std::cout << "thread foo end" << std::endl;
}

std::mutex g_lock;

void func(std::string name)
{
	g_lock.lock();
	//if(name == "first thread")
	//std::this_thread::sleep_for(std::chrono::seconds(2));
	
	std::cerr << "thread func " << std::this_thread::get_id() << ", " << name << std::endl;

	g_lock.unlock();
}

void ThreadDemo::threads()
{
	std::thread t(func, "first thread");
	std::cout << "cpu hardware_concurrency:= " << t.hardware_concurrency() << std::endl;
	g_list.push_back(std::move(t));
	g2_list.push_back(std::make_shared<std::thread>(func, "second ptr thread"));

	for (auto& thread : this->g_list) {
		thread.join();
		//std::cout << "g_list" << std::endl;
	}
	//std::cerr << "threads begin ptr" << std::endl;
	for (auto& thread : this->g2_list) {
		thread->join();
		//std::cout << "g2_list" << std::endl;
	}

}

void ThreadDemo::thread_pool()
{
	ThreadPool pool;
	std::vector<int> vt;
	for (int n = 0; n < 100; n++) {
		int index = n;	
		pool.AddTask([index]() {
			std::cout << "task index:= " << index 
				<< ", this thread id:= " << std::this_thread::get_id()
				<< std::endl;
			});
	}
	pool.Stop();
}

namespace
{
	template<typename T>
	class Compex
	{
	private:
		T _mutex;
		int _num;
	public:
		void mul(int x) {
			std::cout << "mul " << this->_num << std::endl;
			std::lock_guard<T> lock(this->_mutex);
			this->_num *= x;
		}
		void div(int x) {
			std::cout << "div " << this->_num << std::endl;
			std::lock_guard<T> lock(this->_mutex);
			this->_num /= x;
		}
		void both(int n, int a, int b)
		{
			this->_num = n;
			std::cout << "both " << this->_num << std::endl;
			std::lock_guard<T> lock(this->_mutex);
			mul(a);
			div(b);
			std::cout << "num:= " << this->_num << std::endl;
		}
	};

	std::timed_mutex _timed_mutex;
	void work()
	{
		std::chrono::milliseconds time_out(1000);
		while (true)
		{
			if (_timed_mutex.try_lock_for(time_out))
			{
				std::cout << "try_lock_for "<< time_out.count() << "ms unlock " << std::this_thread::get_id() << std::endl;
				std::chrono::milliseconds dur(time_out*2);
				std::this_thread::sleep_for(dur);
				_timed_mutex.unlock();
				//break;
			}
			else {
				std::cout << "wait " << time_out.count() << " ms " << ", " << std::this_thread::get_id() << std::endl;
				std::chrono::milliseconds dur(time_out);
				std::this_thread::sleep_for(dur);
			}
		}
	}
}
void ThreadDemo::thread_mutex()
{	
	//Compex<std::mutex> c;
	/*Compex<std::recursive_mutex> c;
	c.both(10, 2, 4);*/
	std::thread t(work);
	std::thread t2(work);
	std::cout << "t1:= " << t.get_id() << std::endl;
	std::cout << "t2:= " << t2.get_id() << std::endl;
	t.join();
	t2.join();
	std::cout << "thread_mutex end";
}

namespace
{
	template <typename T>
	class SyncQuque
	{
	private:
		bool IsFull() const {
			return m_queue.size() == m_maxSize;
		}
		bool IsEmpty() const {
			return m_queue.empty();
		}
	public:
		SyncQuque(int maxSize) :m_maxSize(maxSize) {}
		void Put(const T& x)
		{
			std::cout << __func__ << std::endl;
			std::unique_lock<std::mutex> locker(this->m_mutex);
			m_notFull.wait(locker, [this]() { return !IsFull(); });
			//while (IsFull()) {
			//	std::cout << "is full ... " << std::endl;
			//	m_notFull.wait(this->m_mutex);
			//}
			std::cout << "____________" << std::endl;
			m_queue.push_back(x);
			m_notEmpty.notify_one();
		}
		void Take(T& x)
		{
			std::unique_lock<std::mutex> locker(this->m_mutex);
			m_notEmpty.wait(locker, [this]() { return !IsEmpty(); });
			/*while (IsEmpty())
			{
				std::cout << "is empty ..." << std::endl;
				m_notEmpty.wait(m_mutex);
			}*/
			x = this->m_queue.front();
			m_queue.pop_front();
			m_notFull.notify_one();
		}
		bool Empty()
		{
			std::lock_guard<std::mutex> locker(m_mutex);
			return m_queue.empty();
		}
		bool Fill()
		{
			std::lock_guard<std::mutex> locker(m_mutex);
			return m_queue.size() == m_maxSize;
		}
		size_t Size()
		{
			std::lock_guard<std::mutex> locker(m_mutex);
			return m_queue.size();
		}
		size_t Count()
		{
			return m_queue.size();
		}
	private:
		std::list<T> m_queue;
		std::mutex m_mutex;
		std::condition_variable_any m_notEmpty;
		std::condition_variable_any m_notFull;
		int m_maxSize;
	};
}
void ThreadDemo::thread_condition_mutex()
{
	SyncQuque<int> s(2);
	s.Put(1);
	s.Put(2);
	//int x;
	//s.Take(x);
	s.Put(3);
	std::cout << "cout:=" << s.Count() << std::endl;
}

namespace {
	class Counter
	{
	public:
		std::atomic<int> _counter;

	public:
		void increment() {
			std::cout << "+\n";
			++_counter;
		}
		void decrement() {
			std::cout << "-\n";
			--_counter;
		}
		int get() {
			return _counter.load();
		}
	};	
	Counter counter;

	std::once_flag flag;

	void do_once() {
		std::call_once(flag, []() {
			std::cout << "called once" << std::endl; }
		);
	}

	void CounterIncrementRun()
	{
		do_once();
		do {
			counter.increment();
			std::cout << "CounterIncrementRun thread id:= " << std::this_thread::get_id() << ", c:= " << counter.get() << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		} while (true);
	}
	void CounterDecrementRun()
	{
		do_once();
		do {
			counter.decrement();
			std::cout << "CounterDecrementRun thread id:= " << std::this_thread::get_id() << ", c:= " << counter.get() << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		} while (true);		
	}

	void CounterWork()
	{		
		std::cout << "get():= " << counter.get() << std::endl;
		std::thread t1(CounterIncrementRun);
		std::thread t2(CounterDecrementRun);
		t1.detach();
		t2.detach();

		std::this_thread::sleep_for(std::chrono::seconds(4));
	}

	void Test(){}
}

void ThreadDemo::thread_atomic()
{
	CounterWork();
}

namespace fu
{
	int func(int x) { return x + 2; }
	void package_foo() {
		std::packaged_task<int(int)> task(func);		
		std::future<int> fut = task.get_future();
		std::thread t(std::move(task), 2);
		std::thread(std::move(task),2).detach();

		auto v = fut.get();
		std::cout << "v:= " << v << std::endl;

		std::vector<std::shared_future<int>> vec;
		auto f = std::async(std::launch::async,[](int a, int b) {
			return a + b;
		},2,3);
		auto x = f.share();
		vec.push_back(x);

		//std::cout << "" << vec[0].get() << std::endl;
	}
}

void ThreadDemo::thread_future()
{
	//fu::package_foo();
	std::promise<int> pr;
	std::thread t([](std::promise<int>& p) {
		std::cout << "promise \n";
		//p.set_value_at_thread_exit(9);
		p.set_value(7);
		}, std::ref(pr));
	t.detach();
	std::future<int> f = pr.get_future();
	auto r = f.get();
	std::cout << "r:= " << r << std::endl;
	if (t.joinable())
		std::cout << t.joinable() << std::endl;
	//std::thread future(CounterWork);
	//std::future<int> f = std::async(std::launch::deferred, [] {
	//	std::cout << " async id = " << std::this_thread::get_id() << std::endl;
	//	return 8; });
	std::future_status status;
	do {
		std::cout << "______begin______" << std::this_thread::get_id() << std::endl;
		auto time = std::chrono::milliseconds(10);
		if (!f.valid()) {
			break;
		}
		status = f.wait_for(time);
		switch (status) {
		case std::future_status::timeout:
			std::cout << "future status timeout" << std::endl;
			break;
		case std::future_status::deferred:
			std::cout << "future status deferred" << std::endl;
			f.get();
			break;
		case std::future_status::ready:
			std::cout << "future status ready" << std::endl;
			break;
		}
	} while (status != std::future_status::ready);
}

void ThreadDemo::thread_join()
{
	std::cout << "begin join" << std::endl;
	std::thread t([]() {
		std::cout << "do thread running" << std::endl;
		});
	std::this_thread::sleep_for(std::chrono::seconds(5));
	if (t.joinable()) {
		t.join();
	}
	std::cout << "end join" << std::endl;
}