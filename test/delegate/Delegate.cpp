#include "Delegate.h"
#include <unordered_map>

void NormalFunc(int a)
{
    printf("��������ͨ���� ��%d\n", a);
}

class A
{
public:
    static void StaticFunc(int a)
    {
        printf("�����ǳ�Ա��̬���� �� %d\n", a);
    }
    void MemberFunc(int a)
    {
        printf("�����ǳ�Ա�Ǿ�̬���� �� %d\n", a);
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
    //���ȴ�����һ������ֵΪ void ,����Ϊint ��һ��ί�С�  
    CMultiDelegate<void, int> e;

    //����������ע�ᵽ��ί����      
    e += newDelegate(NormalFunc);
    e += newDelegate(A::StaticFunc);
    A* a = new A();
    //e += newDelegate(a, &A::MemberFunc);

    //����  
    e(1);
}
