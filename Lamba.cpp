#include "Lamba.h"

#include <iostream>


void Lamba::run()
{
	this->foo2();
}

void Lamba::foo()
{
	int a = 0;
	//auto f1 = [=] {return a++; };	//error 无法自增
	auto f2 = [=]() mutable { return a++; };
	//f1();
	std::cout << "f1 a:= " << a << std::endl;
	f2();
	std::cout << "f2 a:= " << a << std::endl;
}


template<typename _Ty1, typename _Ty2>
std::tuple<_Ty1, _Ty2> tupleOp(_Ty1 _Item1, _Ty2 _Item2)
{
	return std::make_tuple(_Item1, _Item2);
}

template<typename... _Types>
std::tuple<_Types...> tupleOp(_Types... Args)
{
	std::cout << "tupleOp type..." << std::endl;
	return std::make_tuple(Args...);
}

template<typename _Ty>
std::tuple<_Ty> tupleOp(_Ty _Item)
{
	return std::make_tuple(_Item);
}

//template<typename _Ty1, typename... _Types>
//void tupleOpImp(_Ty1 _Item1, _Types... _Items)
//{
//	//_Ty1 a;
//	using _Type = std::tuple<std::_Unrefwrap_t<_Types>...>;
//	auto x1 = tupleOp(_Items...);
//	auto y = tupleOp(_Item1);
//	//tupleOpImp(_Items, _Items...);
//	//std::tie(a, b) = tupleOp(_Item1, std::forward<_Types>(_Items)...);
//	//std::cout << "a: " << a << std::endl;	
//	_Type c;
//	//std::cout << "b: " << b << std::endl;
//	//std::cout << "c: " << c << std::endl;
//	//std::cout << "d: " << d << std::endl;
//}

template<typename... _Types>
std::tuple<_Types...> tupleOpImp(_Types... _Items)
{
	return tupleOp(_Items...);
}

template<typename _Ty>
std::tuple<_Ty> tupleOpImp(_Ty _Item)
{
	return tupleOp(_Item);
}

void Lamba::foo2()
{
	int a, b;
	std::string s;
	std::tie(a,s,b) = tupleOpImp(1, "x", 2);
	std::cout << "a: " << a << std::endl;
	std::cout << "s: " << s << std::endl;
	std::cout << "b: " << b << std::endl;
}


