#pragma once

#include <iostream>


template<typename _Ty>
class This_Val
{
private:
	_Ty _x;

public:
	This_Val(_Ty x) : _x(x) {
		this->Val();
	}

	void Val() {
		std::cout << "Val:= " << this->_x << std::endl;
	}
};

template<typename... _Types>
class MulTemplate;

template<>
class MulTemplate<>
{
public:
	MulTemplate() {
		std::cout << "MulTemplate<>" << std::endl;
	};
};

template<typename _This, typename... _Rest>
class MulTemplate<_This, _Rest...> : private MulTemplate<_Rest...>
{
public:
	using _This_type = _This;
	using _Mybase = MulTemplate<_Rest...>;
	This_Val<_This> _MyFirst;

	template<typename _This2, typename... _Rets2>
	MulTemplate(_This2 this_arg, _Rets2... args):		
		_Mybase(std::forward<_Rets2>(args)...),
		_MyFirst(std::forward<_This2>(this_arg))
	{
		std::cout << "MulTemplate<_This, _Rest> sizeof Rets2 " << sizeof...(_Rets2) << std::endl;		
		//std::cout << "do first this_arg:= " << this_arg << std::endl;
	};	
};


class TemplateOp
{
public:
	void run();
};



