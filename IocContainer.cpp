#include "IocContainer.h"


namespace {
	struct ICar
	{
		virtual ~ICar(){
			std::cout << "~ icar" << std::endl;
		};
		virtual void test() = 0;
	};
	struct Bus final: ICar
	{
		Bus() {}
		void test() override {
			std::cout << "bus test" << std::endl;
		}
	};
	struct Car final : ICar
	{
		Car() {}
		void test() override {
			std::cout << "car test" << std::endl;
		}
	};

	struct My
	{
		My(ICar* c):_c(c) {}
		~My() {
			if (_c) {
				delete _c;
				_c = nullptr;
			}		
		}
		void Func() {
			_c->test();
		}
		ICar* _c;
	};
}
void ioc_container_run()
{
	IocContainer ioc;
	ioc.RegisterType<My, Bus>("bus");
	ioc.RegisterType<My, Car>("car");

	auto bus = ioc.Resolve<My>("bus");
	auto car = ioc.ResolveShared<My>("car");

	bus->Func();
	car->Func();
}








