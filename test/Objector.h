#pragma once
#include <list>
#include <iostream>

void objector_run();

class Subject;

class Objector
{
public:
	virtual ~Objector() {}
	virtual void Update(Subject* const s);
};

class Subject
{
public:
	Subject() {}
	virtual ~Subject() {}
	virtual void Attach(Objector* o) {
		_objectors.push_back(o);
	}
	virtual void Detach(Objector* o) {
		_objectors.remove(o);
	}
	virtual void Notify() {
		for (auto o : _objectors) {
			o->Update(this);
		}
	}
private:
	std::list<Objector*> _objectors;
};
