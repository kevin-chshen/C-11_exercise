#pragma once
#include <list>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <functional>

enum class EnumEvent:char
{
	FULL = 0,
	NOFULL
};

template <typename T, typename U>
class SyncQueue
{
public:
	using _Event = std::function<void(EnumEvent)>;
	SyncQueue(int max_size, _Event&& e)
		:_max_size(max_size),
		_need_stop(false),
		_event(std::forward<_Event>(e)){}
	~SyncQueue() {
		_queue.clear();
	}

public:
	void Put(const T& t) {
		Add(t);
	}
	void Put(T&& t) {
		Add(std::forward<T>(t));
	}
	void Take(std::list<T>& list)
	{
		std::cout << "SyncQueue Add "
			<< ", this thread id:= " << std::this_thread::get_id()
			<< std::endl;
		std::unique_lock<U> locker(_mutex);
		_not_empty.wait(locker, [this]() {
			return _need_stop || NotEmpty();
			});
		if (_need_stop) {
			return;
		}

		list = std::move(_queue);
		_not_full.notify_one();
	}
	void Take(T& t) {
		std::unique_lock<U> locker(_mutex);
		_not_empty.wait(locker, [this]() {
			return _need_stop || NotEmpty();
			});
		if (_need_stop) {
			return;
		}

		t = _queue.front();
		_queue.pop_front();
		_not_full.notify_one();
	}
	void Stop() {
		{
			std::lock_guard<U> locker(_mutex);
			_need_stop = true;
		}
		_not_full.notify_all();
		_not_empty.notify_all();
	}
	bool IsEmpty() {
		std::lock_guard<U> locker(_mutex);
		return _queue.empty();
	}
	bool IsFull() {
		std::lock_guard<U> locker(_mutex);
		return _queue.size() >= _max_size;
	}
	int Count() {
		return _queue.size();
	}
	size_t Size() {
		std::lock_guard<U> locker(_mutex);
		return _queue.size();
	}
private:
	template<typename F>
	void Add(F&& x) {
		std::cout << "SyncQueue Add "
			<< ", this thread id:= " << std::this_thread::get_id()
			<< std::endl;
		std::unique_lock<U> locker(_mutex);
		_not_full.wait(locker, [this]() {
			auto lock = _need_stop || NotFull();
			if (lock) {
				_event(EnumEvent::FULL);
			}
			return lock;
			});
		if (_need_stop) {
			return;
		}

		_queue.emplace_back(std::forward<F>(x));
		_not_empty.notify_one();
	}
	bool NotEmpty() {
		auto empty = _queue.empty();
		if (empty) {
			std::cout << "cache buffer is empty, please waitting ..."
				<< " this thread id:= " << std::this_thread::get_id()
				<< std::endl;
		}
		return !empty;
	}
	bool NotFull() {
		auto full = _queue.size() >= _max_size;
		if (full) {
			std::cout << "cache buffer is full, please waitting, thread id: " 
				<< std::this_thread::get_id() << std::endl;
		}
		return !full;
	}
private:	
	std::list<T> _queue;
	U _mutex;

	int _max_size;
	bool _need_stop;

	std::condition_variable_any _not_full;
	std::condition_variable_any _not_empty;

	_Event _event;
};

