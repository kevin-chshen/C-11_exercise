#include "AlgorithmCpp.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

void AlgorithmCpp::run()
{
	this->foo();
}

void AlgorithmCpp::foo() {
	std::vector<int> v(6);
	std::vector<int> v1(v.size());

	std::iota(v.begin(), v.end(), 2);
	for (auto a : v) {
		std::cout << a << " ";
	}
	std::cout << "\n";

	auto ret = std::minmax_element(v.begin(), v.end());
	std::cout << "max : " << *ret.second << " , min: = " << *ret.first << std::endl;

	auto it = std::copy_if(v.begin(), v.end(), v1.begin(), [](int i) {return i % 2 != 0; });
	v1.resize(std::distance(v1.begin(), it));

	for (int i : v1) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	auto pos = std::is_sorted_until(v.begin(), v.end());
	auto is_sorted = std::is_sorted(v.begin(), v.end());
	std::cout << "pos:= " << (pos == v.end()) << " , is_sorted := " << is_sorted << std::endl;
}