#include "TupleOp.h"
#include <iostream>
#include <string>


void TupleOp::run()
{
	this->foo();
}

namespace {
	template <class Tuple, std::size_t N>
	struct TuplePrinter
	{
		static void print(const Tuple& t)
		{
			TuplePrinter<Tuple, N - 1>::print(t);
			std::cout << ", " << std::get<N - 1>(t);
		}
	};
	template <class Tuple>
	struct TuplePrinter<Tuple, 1>
	{
		static void print(const Tuple& t) {
			std::cout << std::get<0>(t);
		}
	};
	template<class... Args>
	void PrintTuple(const std::tuple<Args...>& t)
	{
		std::cout << "(";
		TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
		std::cout << ")\n";
	}
	void TestPrint()
	{
		/*std::tuple<int, short, double, char> tp =std::make_tuple(1,2,3.0,'a');
		PrintTuple(tp);*/
		//std::cout << std::get<0>(tp) << std::endl;
	}
}
namespace {
	template<int...>
	struct IndexTuple {};
	template <int N, int... Indexes>
	struct MakeIndexes : MakeIndexes<N - 1, N - 1, Indexes...> {};
	template<int... indexes>
	struct MakeIndexes<0, indexes...>{
		typedef IndexTuple<indexes...> type;
	};

	template<typename T>
	void Print(T t)
	{
		std::cout << t << std::endl;
	}
	template<typename T, typename... Args>
	void Print(T t, Args... args)
	{
		std::cout << t << std::endl;
		Print(args...);
	}
	//template<typename Tuple, int... Indexes>
	//void Transform(IndexTuple<Indexes...>& in, Tuple& tp)
	//{
	//	Print(std::get<Indexes>(tp)...);
	//}
	template <typename T>
	void Fun(T t)
	{
		std::cout << "t:=" << t << std::endl;
	}
	template<typename Tuple, int... Indexes>
	void Transform(IndexTuple<Indexes...>& in, Tuple& tp)
	{
		int a[] = { (Fun(std::get<Indexes>(tp)), 0)...};
		std::cout << "a size_t:=" << sizeof(a) / sizeof(int) << std::endl;
	}
	void MainPrint()
	{
		//using Tuple = std::tuple<int, double, long>;
		//Tuple tp = std::make_tuple(1,2,3);
		//auto n = std::tuple_size<Tuple>::value;
		//std::cout << "n:=" << n << std::endl;
		//auto type = MakeIndexes<std::tuple_size<Tuple>::value>::type();
		//std::cout << "type:=" << typeid(type).name() << std::endl;
		//Transform(type, tp);
	}
}
namespace detail{
	template <typename T, typename U>
	typename std::enable_if<std::is_convertible<T, U>::value ||
	std::is_convertible<U, T>::value, bool>::type
		compare(T t, U u)
	{
		return t == u;
	}
	bool compare(...)
	{
		return false;
	}
	template<int I, typename T, typename... Args>
	struct find_index
	{
		static int call(std::tuple<Args...> const& t, T&& val)
		{
			return (compare(std::get<I - 1>(t), val) ?
				I - 1 : find_index<I - 1, T, Args...>::call(
					t, std::forward<T>(val)
				));
		}
	};
	template<typename T, typename... Args>
	struct find_index<0, T, Args...>
	{
		static int call(std::tuple<Args...> const& t, T&& val)
		{
			return compare(std::get<0>(t), val) ? 0 : -1;
		}
	};
}

template<typename T, typename... Args>
int find_index(std::tuple<Args...> const& t, T&& val)
{
	return detail::find_index<0, (sizeof...(Args)) - 1, T, Args...>::
		call(t, std::forward<T>(val));
}

//int FindMain()
//{
//	std::tuple<int, double, std::string> tp =
//		std::make_tuple(1, 2, std::string("ok"));
//	int index = find_index(tp, std::string("ok"));
//	std::cout << index << std::endl;
//	return 0;
//}

void TupleOp::foo()
{
	//TestPrint();
	//MainPrint();
	//FindMain();
}