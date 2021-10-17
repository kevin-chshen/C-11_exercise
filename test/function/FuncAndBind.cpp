#include "FuncAndBind.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <functional>
#include <string>

void FuncAndBind::run()
{
	this->foo();
}

void FuncAndBind::foo()
{
	class A {
	public:
		int foo(int x, int y) {
			return x + y;
		}
	};
	A a;
	auto fr = std::bind(&A::foo, &a, std::placeholders::_1, std::placeholders::_2);
	std::cout << fr(1, 2) << std::endl;

	std::less<int>();
	std::initializer_list<int> list{ 1,2,3,4,5,6,7 };
	//std::vector<int> list{ 1,2,3,4,5,6,7 };
	auto count = std::count_if(list.begin(), list.end(), std::bind(std::less<int>(), std::placeholders::_1, 5));
	std::cout << "1 count:= " << count << std::endl;

	auto a1 = std::less<int>()(1, 2);
	auto b1 = 1 < 2;
	auto a2 = std::less<const std::string>()("xxxx", "yyyy");
	auto b2 = (std::string("xxxxyyyyy") < std::string("yyyy00000"));
	std::cout << "a2 := " << a2 << std::endl;
	std::cout << "b2 := " << b2 << std::endl;
	auto fr2 = std::bind(std::logical_and<bool>(),
		std::bind(std::less<int>(), std::placeholders::_1, 3),
		std::bind(std::greater<int>(), std::placeholders::_1, 3));
	auto count2 = std::count_if(list.begin(), list.end(), fr2);
	std::cout << "2 count:= " << count2 << std::endl;
}