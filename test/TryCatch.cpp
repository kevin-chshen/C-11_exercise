#include "TryCatch.h"

#include <iostream>

void TryCatch::run()
{
	this->foo();
}

void TryCatch::foo()
{
	try {
		throw std::invalid_argument("argument is invalid");
	}
	catch (std::exception & e) {
		std::cout << "____________ " << e.what() << std::endl;
	}
}
