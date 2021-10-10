#include "sync_test.h"
#include <future>
#include <iostream>
#include <chrono>
// condition_variable::wait (with predicate)
#include <thread>             // std::thread, std::this_thread::yield
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
namespace
{
	int find_answer()
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "after find_answer  sleep_for" << std::endl;
		return 1;
	}
}
void sync_test::foo()
{
	std::future<int> answer = std::async(find_answer);
	std::cout << "before get the answer is "  << std::endl;
	std::cout << "the answer is " << answer.get() << std::endl;
	std::cout << "after get the answer is " << std::endl;
}

namespace
{
	std::mutex mtx;
	std::condition_variable cv;

	int cargo = 0;
	bool shipment_available() { return cargo != 0; }

	void consume(int n) {
		for (int i = 0; i < n; ++i) {
			std::unique_lock<std::mutex> lck(mtx);
			//std::cout << "consume wait before " << cargo << '\n';
			cv.wait(lck, shipment_available);
			// consume:
			std::cout << "consume cargo " << cargo << '\n';
			cargo = 0;
		}
	}
}

void sync_test::condition_variable_foo()
{
	std::thread consumer_thread(consume, 10);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	// produce 10 items when needed:
	for (int i = 0; i < 10; ++i) {
		std::cout << "before while " << i << std::endl;
		while (shipment_available()) {
			//std::cout << "before yield " << cargo << std::endl;
			std::this_thread::yield();
			//std::cout << "after yield " << i << std::endl;
		}
		std::unique_lock<std::mutex> lck(mtx);
		cargo = i + 1;
		std::cout << "before notify_one " << cargo << std::endl;
		cv.notify_one();
		std::cout << "after notify_one " << cargo << std::endl;
	}

	consumer_thread.join();
}