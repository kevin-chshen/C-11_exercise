#include "const_test.h"
#include <iostream>


void const_test::foo()
{
	const_test::constchar2char();
}

void const_test::constchar2char()
{
	std::string str = "dadasda";
	char* c = const_cast<char*>(str.c_str());
	std::cout << "str " << c  << std::endl;
}