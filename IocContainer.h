#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include "Any.h"

class IocContainer
{
public:
	IocContainer() {};
	~IocContainer() {};

public:
	template <typename T, typename Denped, typename... Args>
	typename std::enable_if<!std::is_base_of<T, Denped>::value>::type
		RegisterType(const std::string& key)
	{
		std::function<T * (Args...)> f = [](Args... args) {
			return new T(new Denped(args...));
		};
		RegisterType(key, f);
	}
	template <typename T, typename Denped, typename... Args>
	typename std::enable_if<std::is_base_of<T, Denped>::value>::type
		RegisterType(const std::string& key)
	{
		std::function<T * (Args...)> f = [](Args... args) {
			return new Denped(args...);
		};
		RegisterType(key, f);
	}
	template <typename T, typename... Args>
	void RegisterSimple(const std::string& key)
	{
		std::function<T * (Args...)> f = [](Args... args) {
			return new T(args...);
		};
		RegisterType(key, f);
	}
	template <typename T, typename... Args>
	T* Resolve(const std::string& key, Args... args)
	{
		auto it = _obj_map.find(key);
		if (it == _obj_map.end()) {
			return nullptr;
		}		
		Any resolver = it->second;
		
		std::function<T * (Args...)> func =
			resolver.AnyCast<std::function<T * (Args...)>>();
		return func(args...);
	}
	template <typename T, typename... Args>
	std::shared_ptr<T> ResolveShared(const std::string& key, Args... args)
	{
		T* ptr = Resolve<T>(key, args...);
		return std::shared_ptr<T>(ptr);
	}
private:
	void RegisterType(const std::string& key, Any creator)
	{
		if (_obj_map.find(key) != _obj_map.end())
			throw std::invalid_argument("alread register");

		_obj_map.emplace(key, creator);
	}
private:
	
	std::unordered_map<std::string, Any> _obj_map;
};

void ioc_container_run();

