#include "cpp11.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include <typeindex>
#include <type_traits>

enum Flags { good = 0, fail = 1, bad = 2, eof = 4 };
constexpr int operator|(Flags f1, Flags f2)
{
	return Flags(int(f1) * int(f2));
}
void f(Flags x)
{
	switch (x) {
	case bad:         std::cout << "bad" << "\n"; break;
	case eof:         std::cout << "eof" << "\n"; break;
	case bad | eof:    std::cout << "bad | eof" << "\n"; break;
	default:          std::cout << "default" << "\n"; break;
	}
}

void cpp11::run() {
	this->AnyFunc();
	/*Flags f1 = bad;
	auto x = int(Flags(eof | good)| bad);
	std::cout << x << std::endl;*/
}

void cpp11::LambdaFunc()
{
	int n = [](int x, int y) {return x + y; }(5, 6);
	std::cout << n << std::endl;
}

void cpp11::SharedPtrFunc()
{
	class A
	{
	public:
		void foo() {
			std::cout << "print a class" << std::endl;
		}
	};
	std::shared_ptr<A> a = std::make_shared<A>();
	a->foo();

	std::shared_ptr<int> foo = std::make_shared<int>(10);
	std::cout << "*foo: " << *foo << '\n';

	struct C { int* data; };
	std::shared_ptr<C> c = std::make_shared<C>();
	std::cout << "c: " << c->data << '\n';

	std::shared_ptr<int> p1;
	std::shared_ptr<int> p2(nullptr);
	std::shared_ptr<int> p3(new int);
	std::shared_ptr<int> p4(new int, std::default_delete<int>());
	std::shared_ptr<int> p5(new int, [](int* p) {delete p; }, std::allocator<int>());
	std::shared_ptr<int> p6(p5);
	std::shared_ptr<int> p7(std::move(p6));
	std::shared_ptr<int> p8(std::unique_ptr<int>(new int));
	std::shared_ptr<C> obj(new C);
	std::shared_ptr<int> p9(obj, obj->data);

	std::cout << "use_count:\n";
	std::cout << "p1: " << p1.use_count() << '\n';
	std::cout << "p2: " << p2.use_count() << '\n';
	std::cout << "p3: " << p3.use_count() << '\n';
	std::cout << "p4: " << p4.use_count() << '\n';
	std::cout << "p5: " << p5.use_count() << '\n';
	std::cout << "p6: " << p6.use_count() << '\n';
	std::cout << "p7: " << p7.use_count() << '\n';
	std::cout << "p8: " << p8.use_count() << '\n';
	std::cout << "p9: " << p9.use_count() << '\n';
}

void cpp11::AllocatorFunc()
{
	std::allocator<int> alloc;
	//std::default_delete<int> del;

	std::shared_ptr<int> foo = std::allocate_shared<int>(alloc, 10);
	auto bar = std::allocate_shared<int>(alloc, 20);
	auto baz = std::allocate_shared<std::pair<int, int>>(alloc, 30, 40);

	std::cout << "*foo: " << *foo << std::endl;
	std::cout << "*bar: " << *bar << std::endl;
	std::cout << "*baz: " << baz->first << ", " << baz->second << std::endl;
}

void cpp11::AttributesFunc()
{
	std::vector<std::string> v{"123","0123" ,"1123" ,"2123" ,"3123" };
	for (int x  =0; x < 10; ++x)
	for ([[omp::parallel()]] std::size_t i = 0; i < v.size(); ++i) {
		std::cout << v[i] << std::endl;
	}
}

void cpp11::MoveFunc()
{
	std::vector<std::string> tokens{"x","y"};
	std::cout << "move before tokens" << tokens.size() << std::endl;
	std::vector<std::string> t = std::move(tokens); 

	std::cout << "move after tokens:= " << tokens.size() << ", tokens == nullptr? " << tokens.empty() << std::endl;
}
namespace {
	template <typename T>
	void PrintT(T& t)
	{
		std::cout << "lvalue " << t << std::endl;
	}

	template <typename T>
	void PrintT(T&& t)
	{
		std::cout << "rvalue " << t << std::endl;
	}

	template <typename T>
	void TestForward(T&& t)
	{
		PrintT(t);
		PrintT(std::forward<T>(t));
		PrintT(std::move(t));
	}
}

void cpp11::ForwardFunc()
{
	TestForward(1);
	int x = 2;
	TestForward(x);
	TestForward(std::forward<int>(x));
}

namespace {

	template<typename Function, typename... Args>
	inline auto ForwardWapper(Function&& f, Args&& ... args) -> decltype(f(std::forward<Args>(args)...))
	{
		return f(std::forward<Args>(args)...);
	}

	void test0() {
		std::cout << "void" << std::endl;
	}

	int test1() {
		return 1;
	}
}

void cpp11::ForwardFunc2()
{
	ForwardWapper(test0);
	ForwardWapper(test1);
}

void cpp11::EmplaceFunc()
{
	class Complicated
	{
		int _a;
		double _b;
		std::string _c;
	public:
		Complicated(int a, double b, std::string c) : _a(a), _b(b), _c(c) {
			std::cout << "is construct" << std::endl;
		}
		Complicated(const Complicated& other) : _a(other._a), _b(other._b), _c(std::move(other._c)) {
			std::cout << "is moved" << std::endl;
		}
	};

	std::map<int, Complicated> map;
	std::cout << "-- insert --" << std::endl;
	map.insert(std::make_pair(4, Complicated(1, 2.0, "x")));
	std::cout << "-- emplace --" << std::endl;
	map.emplace(std::make_pair(4, Complicated(1, 2.0, "x")));

	std::vector<Complicated> v;
	std::cout << "-- emplace_back --" << std::endl;
	v.emplace_back(1, 2.0, "x");
	std::cout << "-- push back --" << std::endl;
	v.push_back(Complicated(1, 2.0, "x"));
}

namespace {
	template <class, class>
	_INLINE_VAR constexpr bool is_same_self = false; // determine whether arguments are the same type	
	template <class _Ty>
	_INLINE_VAR constexpr bool is_same_self<_Ty, _Ty> = true;


	//template <typename T>
	//typename std::result_of<std::function<T>>::type func()
	//{
	//	auto f = []() {std::cout << "result of function" << std::endl; };
	//	return (std::function<T>(T*()))();
	//}

	template <typename T>
	std::function<T> foo()
	{
		return std::function<T>((T*)([](int a) { return a; }));
	}
#ifdef Cpp14
	template <typename Fn>
	typename std::result_of<Fn(int)>::type foo2(const Fn& f)
	{
		return f(2);
	}

	template <typename T>
	typename std::result_of<std::function<T>(int)>::type foo3()
	{
		auto f = foo<T>();
		return f(2);
	}

	struct Person
	{
		std::string name;
		int age;
		std::string city;
	};

	std::vector<Person> vt = {
		{"aa",20,"shanghai"},
		{"bb",25,"beijing"},
		{"cc",20,"nanjing"},
		{"dd",25,"nanjing"}
	};


	template<typename Fn>
	std::multimap<typename std::result_of<Fn(Person)>::type, Person> GroupBy(const std::vector<Person>& vt, const Fn& keySelect)
	{
		typedef typename std::result_of<Fn(Person)>::type key_type;
		std::multimap<key_type, Person> map;
		std::for_each(vt.begin(), vt.end(),
			[&](const Person& p) {
				map.insert(std::make_pair(keySelect(p), p));
			});
		return map;
	}
#endif // !1
}

void cpp11::IsSameFunc()
{
	std::cout << is_same_self<int, bool> << "\n";
	std::cout << is_same_self<int, int> << "\n";

	//class A {};
	//class B : A {};
	//std::is_same<int, int>();
	//std::cout << __is_base_of(A, B) << std::endl;;
}

void cpp11::ResultOfFunc()
{
	auto f = [](int a) { return a; };
#ifdef Cpp14
	typedef std::result_of<decltype(f) & (int)>::type A;
	std::function<int(int)> fr = f;
	std::cout << "fr:= " << fr(1) << '\n';
	auto x = foo<int(int)>();
	std::cout << x(2) << std::endl;

	typedef int (*Func)(int d);
	//std::cout << foo2<Func(int)>([](int x) {return x; }) << std::endl;
	auto w = foo3<int(int)>();
	std::cout << "w:=" << w << std::endl;
	//auto res = GroupBy(vt, [](const Person& p) { return p.age; });

	//foo2<int(int)>();
	//auto w = foo2<int(int)>(f, 4);
	std::cout << "foo2:=" << std::is_same<int, A>();
	std::cout << '\n';
	/*auto f = func<int(int)>(1);
	std::cout <<  << std::endl;*/
#endif		 
}

namespace
{
	template<typename T>
	void AnyFoo() {
		
	}
}

void cpp11::ClassConstMemberFunc()
{
	class C
	{
	public:
		C(int const& a) :_a(a) {};
		const int _a;
	};

	C c(2);
	std::cout << c._a << std::endl;
}

#include <functional>
void cpp11::AnyFunc()
{
	auto index0 = std::type_index(typeid(typename std::decay<int>::type));
	auto index1 = std::type_index(typeid(typename std::decay<std::string>::type));
	auto index2 = std::type_index(typeid(typename std::decay<long>::type));
	auto index3 = std::type_index(typeid(typename std::decay<char>::type));
	auto index4 = std::type_index(typeid(typename std::decay<double>::type));
	auto index5 = std::type_index(typeid(typename std::decay<float>::type));
	auto index6 = std::type_index(typeid(typename std::decay<bool>::type));
	std::cout << "xxx" << std::endl;
}

