#include "Event.h"
#include <iostream>
#include <functional>

namespace {
	struct stA
	{
		int a, b;
		void print(int a, int b) {
			std::cout << a << ", " << b << std::endl;
		}
	};
	void print(int a, int b) {
		std::cout << a << ", " << b << std::endl;
	}
}

void event_foo() {
	Events<std::function<void(int, int)>> myevent;

	auto key = myevent.Connector(print);
	stA t;
	auto lambdakey = myevent.Connector(
		[](int a, int b) {
			std::cout << "a + b:= " << a+b << std::endl;
		});
	std::function<void(int, int)> f =
		std::bind(&stA::print, &t, 
			std::placeholders::_1,
			std::placeholders::_2);
	myevent.Connector(f);
	int a = 1, b = 2;
	myevent.Notify(a, b);
}