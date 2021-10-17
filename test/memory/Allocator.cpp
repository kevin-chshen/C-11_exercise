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
		//std::allocator<int> a1;   // int ��Ĭ�Ϸ�����
		//int* a = a1.allocate(1);  // һ�� int �Ŀռ�
		//a1.construct(a, 7);       // ���� int
		//std::cout << a[0] << '\n';
		//a1.deallocate(a, 1);      // �����һ�� int �Ŀռ�

		//// string ��Ĭ�Ϸ�����
		//std::allocator<std::string> a2;

		//// ͬ�ϣ����� a1 ���ذ󶨻�ȡ
		//decltype(a1)::rebind<std::string>::other a2_1;

		//// ͬ�ϣ���ͨ�� allocator_traits ������ a1 ���ذ󶨻�ȡ
		//std::allocator_traits<decltype(a1)>::rebind_alloc<std::string> a2_2;

		//std::string* s = a2.allocate(2); // 2 �� string �Ŀռ�

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