#include "Common.h"
#include <type_traits>


void Common::run()
{
	this->EnableIfFunc();
}


//// STRUCT TEMPLATE enable_if
//template <bool _Test, class _Ty = void>
//struct enable_if {}; // no member "type" when !_Test
//
//template <class _Ty>
//struct enable_if<true, _Ty> { // type is _Ty for _Test
//	using type = _Ty;
//};


//template <typename T, std::enable_if<true, typename T>>
//void enableIf(T& t)
//{
//	std::cout << typeid(t).name() << std::endl;
//}
void Common::EnableIfFunc()
{
	
}
