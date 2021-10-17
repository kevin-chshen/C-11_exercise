#include "Any.h"
#include <string>
#include <iostream>

void TestAny()
{
	Any n;
	auto r = n.IsNull();
	std::cout << r << std::endl;
	std::string s1 = "hello";
	n = s1;
	n.AnyCast<int>();
	Any n1 = 1;	
	std::cout << n1.Is<int>() << std::endl;
}