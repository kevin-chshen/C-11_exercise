#include "Allocator.h"

#include <string>
#include <memory>
#include <iostream>

namespace Msl
{
	void Allocator::Run()
	{
		this->Foo2();
	}
	void Allocator::Foo()
	{
		std::allocator<std::string> alloc;
		int n{5};
		std::cout << n << "\n";

		auto const p = alloc.allocate(n);
		auto q = p;
		//alloc.construct(q++);
		//alloc.construct(q++, 10, 'c');
		//alloc.construct(q++, "hi");
	}
	void Allocator::Foo2()
	{
		//std::allocator<int> a1;   // int 的默认分配器
		//int* a = a1.allocate(1);  // 一个 int 的空间
		//a1.construct(a, 7);       // 构造 int
		//std::cout << a[0] << '\n';
		//a1.deallocate(a, 1);      // 解分配一个 int 的空间

		//// string 的默认分配器
		//std::allocator<std::string> a2;

		//// 同上，但以 a1 的重绑定获取
		//decltype(a1)::rebind<std::string>::other a2_1;

		//// 同上，但通过 allocator_traits 由类型 a1 的重绑定获取
		//std::allocator_traits<decltype(a1)>::rebind_alloc<std::string> a2_2;

		//std::string* s = a2.allocate(2); // 2 个 string 的空间

		//a2.construct(s, "foo");
		//a2.construct(s + 1, "bar");

		//std::cout << s[0] << ' ' << s[1] << '\n';

		//a2.destroy(s);
		//a2.destroy(s + 1);
		//a2.deallocate(s, 2);
	}
	void Allocator::Foo3()
	{
		long double n = 1L;
		std::cout << sizeof(n) / sizeof(char) << ", " << n << std::endl;
		std::cout << alignof(std::max_align_t) << '\n';
		auto ptr = ::operator new(2);
		std::cout << typeid(::operator new(2)).name() << std::endl;
		std::cout << static_cast<size_t>(0) << std::endl;
	}
}