#pragma once
#include <functional>
#include <type_traits>

void command_run();

//template<typename F, class... Args,
//	class = typename std::enable_if<
//	!std::is_member_function_pointer<F>::value>::type>
//	void Wrap(F&& f, Args&& ... args)
//{
//	return f(std::forward<Args>(args)...);
//}
//
//template<class R, class C, class... DArgs,class P, class... Args>
//void Wrap(R(C::* f)(DArgs...), P&& p, Args&& ...args)
//{
//	return (*p.*f)(std::forward<Args>(args)...);
//}

template<typename R=void>
class Command
{
private:
	std::function<R()> _f;

public:
	template<class F, class... Args, class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
	void Wrap(F&& f, Args &&... args)
	{
		_f = [&]() { return f(args...); };
	}
	template<class R, class C, class... DArgs, class P, class... Args>
	void Wrap(R(C::*f)(DArgs...) const, P && p, Args && ... args)
	{
		_f = [&, f]() { return (*p.*f)(args...); };
	}
	template<class R ,class C, class... DArgs, class P, class... Args>
	void Wrap(R(C::* f)(DArgs...), P&& p, Args&& ... args)
	{
		_f = [&, f]() {return (*p.*f)(args...); };
	}

	R Excecute()
	{
		return _f();
	}
};	
