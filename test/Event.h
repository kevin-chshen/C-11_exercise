#pragma once

#include <map>
#include <iostream>

void event_foo();

class Noncopyable
{
public:
	Noncopyable() = default;
	~Noncopyable() = default;
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator = (const Noncopyable&) = delete;
};

template<typename Func>
class Events : Noncopyable
{
public:
	Events() {}
	~Events() {}
	int Connector(Func&& f) {
		return Assign(std::forward<Func>(f));
	}
	int Connector(const Func& f) {
		return Assign(f);
	}
	void Disconnector(const Func& f) {
		_map.remove(f);
	}
	template<typename ...Args>
	void Notify(Args&& ...args) {
		for (auto& it : _map) {
			it.second(std::forward<Args>(args)...);
		}
	}
private:
	template<typename F>
	int Assign(F&& f) {		
		_map.emplace(_object_id, std::forward<F>(f));
		return _object_id++;
	}
private:
	int _object_id = 0;
	std::map<int, Func> _map;
};

