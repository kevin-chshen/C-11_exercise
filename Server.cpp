// Server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "cpp11.h"
#include "Lamba.h"
#include "Allocator.h"

#include "TemplateOp.h"
#include "Common.h"
#include "TupleOp.h"
#include "ThreadDemo.h"
#include "AlignofCpp.h"
#include "AlgorithmCpp.h"
#include "Singleton.h"
#include "Objector.h"
#include "Event.h"
#include "Visitor.h"
#include "Command.h"
#include "IocContainer.h"
#include "Any.h"

int main()
{
	//TestAny();
	ioc_container_run();
	//command_run();
	//visitor_run();
	//objector_run();
	//event_foo();
	//app::run();
	//AlgorithmCpp al;
	//al.run();
	//AlignofCpp a;
	//a.run();
	//Common c;
	//c.run();
	//cpp11 c11;
	//c11.run();
	/*TupleOp op;
	op.run();*/

	//Msl::Allocator a;
	//a.Run();

	/*Lamba la;
	la.run();*/
	//TemplateOp op;
	//op.run();

	/*ThreadDemo t;
	t.run();*/
	//ThreadDemo t;
	//t.run();

	//std::cout << "main thread id:= " << std::this_thread::get_id() << std::endl;
	//std::this_thread::sleep_for(std::chrono::seconds(5));
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
