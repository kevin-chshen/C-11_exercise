#include "ForDemo.h"

#include <iostream>
#include <vector>
#include <algorithm>

void ForDemo::run()
{

}

void ForDemo::foo()
{
	std::vector<int> v{ 1,2,3,4,5 };
	std::cout << "it := ";
	std::for_each(v.begin(), v.end(), [](auto it) {
		std::cout << it << " ";
		});
	std::cout << std::endl;
	for (auto& n : v) {
		std::cout << ++n << " ";
	}
	std::cout << std::endl;
	for (auto it = v.begin(), end = v.end(); it != end; ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}