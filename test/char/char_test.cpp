#include "char_test.h"
#include <iostream>


void char_test::foo()
{
	char_test::strlen_test();
	char_test::strnlen_s_test();
	char_test::string_test();
}

void char_test::strlen_test()
{
	char str[] = { 'H', 'e', 'l', 'l', 'o', ',', 'w', 'o', 'r', 'l', 'd' };
	
	size_t len = strlen(str);
	std::cout << "length is " << len << std::endl;
	char st[100] = { 0 };
	std::cout << "sizeof " << sizeof(st) << std::endl;
}

void char_test::strnlen_s_test()
{
	char str[] = { 'H', 'e', 'l', 'l', 'o', ',', 'w', 'o', 'r', 'l', 'd' };
	size_t len = strnlen_s(str, 0);
	std::cout << "length is " << len << std::endl;
}

void char_test::string_test()
{
	std::string str1 = { 'H', 'e', 'l', 'l', 'o', ',', 'w', 'o', 'r', 'l', 'd' };
	std::string str2 = "Hello,world";
	std::string str3 = { 'H', 'e', 'l', 'l', 'o', ',', 'w', 'o', 'r', 'l', 'd', '\0' };
	size_t len1 = std::string(str1).size();
	size_t len2 = std::string(str2).size();
	size_t len3 = std::string(str3).size();
	std::cout << "str1's length is " << len1 << std::endl;
	std::cout << "str2's length is " << len2 << std::endl;
	std::cout << "str3's length is " << len3 << std::endl;
}