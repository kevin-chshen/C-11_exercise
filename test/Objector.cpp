#include "Objector.h"

void objector_run() {
	Subject s;
	for (int i = 0; i < 100; ++i) {
		s.Attach(new Objector());
	}
	s.Notify();	
}

void Objector::Update(Subject* const s) {
	std::cout << "Objector do Update" << std::endl;
	//s->Detach(this);
}
