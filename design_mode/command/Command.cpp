#include "Command.h"
#include <iostream>

struct STA
{
	int _a;
	int operator()() { return _a; }
	int operator()(int n) { return _a + n; }
	int triple0() { 
		std::cout << "triple 0" << std::endl;
		return _a * 3; 
	}
	int triple1(int a) {
		std::cout << "triple 1" << std::endl;
		return _a * 3 + a; 
	}
	int triple2() const { 
		std::cout << "triple 2" << std::endl;
		return _a*3; 
	}
	const int triple3(int a) const { 
		std::cout << "triple 3" << std::endl;
		return _a * 3 + a; 
	}
	void triple4() { 
		std::cout << "triple 4" << std::endl;
	}
};

int add_one(int n)
{
	std::cout << "add_one" << std::endl;
	return n + 1;
}

void command_run() {
	Command<int> cmd;
	cmd.Wrap(add_one, 0);
	cmd.Excecute();
	cmd.Wrap([](int i) {return i + 1; }, 1);
	cmd.Excecute();
	STA t = { 10 };
	int x = 3;
	cmd.Wrap(&STA::triple0, &t);
	cmd.Excecute();
	cmd.Wrap(&STA::triple1, &t, x);
	cmd.Excecute();
	cmd.Wrap(&STA::triple1, &t, 3);
	cmd.Excecute();
	cmd.Wrap(&STA::triple2, &t);
	cmd.Excecute();
	cmd.Wrap(&STA::triple3, &t, 3);

	auto r = cmd.Excecute();

	Command<> cmd1;
	cmd1.Wrap(&STA::triple4, &t);
	cmd1.Excecute();
}

