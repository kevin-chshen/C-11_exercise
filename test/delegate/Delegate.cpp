#include "Delegate.h"
#include <unordered_map>

void NormalFunc(int a)
{
    printf("这里是普通函数 ：%d\n", a);
}

class A
{
public:
    static void StaticFunc(int a)
    {
        printf("这里是成员静态函数 ： %d\n", a);
    }
    void MemberFunc(int a)
    {
        printf("这里是成员非静态函数 ： %d\n", a);
    }
};

template<typename ...ParamTypes>
class EventTest
{
public:    

    using CallBackHandler = CMultiDelegate<void, ParamTypes...>;

    typename 
    void Register(int et, CallBackHandler cb)
    {
        _events.emplace(et, cb);
    }

    void UnRegister(int et)
    {
        _events.erase(et);
    }

    //typename <typename ...Args>
    void OnEvent(int et, ParamTypes&& ...args)
    {
        if (_events[et] != nullptr)
            _events[et](args);
    }

private:
    std::unordered_map<int, CallBackHandler> _events;
};


void Delete_foo()
{
    //首先创建了一个返回值为 void ,参数为int 的一个委托。  
    CMultiDelegate<void, int> e;

    //将三个函数注册到该委托中      
    e += newDelegate(NormalFunc);
    e += newDelegate(A::StaticFunc);
    A* a = new A();
    //e += newDelegate(a, &A::MemberFunc);

    //调用  
    e(1);
}
