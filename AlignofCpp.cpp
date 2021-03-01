#include "AlignofCpp.h"
#include <iostream>
#include <type_traits>

void AlignofCpp::run() {
	this->storage();
}

struct MyStruct
{
	char a;
	int b;
	short c;
	//double c;
};

namespace {
	struct A
	{
		int avg;
		A(int a, int b) : avg((a + b) / 2) {}

		~A() {
			std::cout << "distructor" << std::endl;
			avg = 0;
		}
	};
	typedef std::aligned_storage<sizeof(A), alignof(A)>::type Aligned_A;
}

void AlignofCpp::foo() {
	int alignsize = std::alignment_of<MyStruct>::value;
	int sz = alignof(MyStruct);
	std::cout << "alignsize := " << alignsize << std::endl;
	std::cout << "sz := " << sz << std::endl;
	std::cout << "sizeof := " << sizeof(MyStruct) << std::endl;
}

void AlignofCpp::storage() {
	Aligned_A a, b;
	new (&a) A(10, 20);
	b = a;
	std::cout << reinterpret_cast<A&>(b).avg << std::endl;
	reinterpret_cast<A&>(a).~A();
	std::cout << "pad " << a._Pad << ", val " << a._Val << std::endl;
	std::cout << reinterpret_cast<A&>(a).avg << std::endl;

}
