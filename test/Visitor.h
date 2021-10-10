#pragma once
#include <iostream>

template<typename E>
struct Visitor
{
	virtual void Visit(const E&) = 0;
};

struct Element
{
	virtual void Accept(Visitor<Element>&) = 0;
	virtual void Print() = 0;
};

struct EA final
{
	void Accept(Visitor<EA>& v){
		v.Visit(*this);
	}
	void Print() {
		std::cout << "EA" << std::endl;
	}
};

struct VA final: Visitor<EA>
{
	void Visit(const EA& e) override;
};



void visitor_run();
