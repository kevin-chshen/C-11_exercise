#pragma once


namespace app
{
	void run();


template<typename T>
class Singleton
{
public:
	template<typename ...Args>
	static T* Instance(Args ...args) {
		if (_obj == nullptr) {
			_obj = new T(std::forward<Args>(args)...);
		}
		return _obj;
	}

	static T* GetInStance() {
		if (_obj == nullptr) {
			throw std::logic_error("instance is nullptr");
		}
		return _obj;
	}

	static void DestoryInstance() {
		delete _obj;
		_obj = nullptr;
	}
private:
	Singleton(void);
	virtual ~Singleton();
	Singleton(const Singleton&);
	Singleton& operator = (const Singleton&);
private:
	static T *_obj;
};

template<typename T>
T* Singleton<T>::_obj = nullptr;

}