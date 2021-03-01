#include "Singleton.h"

#include <string>
#include <iostream>

namespace app
{
	struct A
	{
		A(const std::string&) { std::cout << "lvalue" << std::endl; }
		A(std::string&&) { std::cout << "rvalue" << std::endl; }
	};
	struct B
	{
		B(const std::string&) { std::cout << "lvalue" << std::endl; }
		B(std::string&&) { std::cout << "rvalue" << std::endl; }
	};
	struct C
	{
		C(int x, double y) { std::cout << x + y << std::endl; }
		void Fun() { std::cout << "test" << std::endl; }
	};
	void run()
	{
		std::string str = "bb";
		Singleton<A>::Instance(str);
		Singleton<B>::Instance(std::move(str));
		Singleton<C>::Instance(1,2.1)->Fun();

		Singleton<A>::DestoryInstance();
		Singleton<B>::DestoryInstance();
		Singleton<C>::DestoryInstance();
	}
}
