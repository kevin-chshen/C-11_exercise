#include "Visitor.h"

void VA::Visit(const EA& e) {
	std::cout << "VA" << std::endl;
	const_cast<EA*>(&e)->Print();
}

class eta
{
public:
	int a;
	void Print2() const {
		std::cout << "eta" << std::endl;
	}
};
class sta
{
public:
	void visit(eta* const e) {
		e->Print2();
		e->a = 10;
		//e = new eta();
	}
};

void visitor_run()
{
	EA ea;
	VA va;
	va.Visit(ea);
}