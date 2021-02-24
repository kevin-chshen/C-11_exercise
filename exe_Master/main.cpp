
#include <iostream>
#include <cassert>
#include <string>
#include <map>

namespace
{
	bool ParseKeyValue(const std::string &str, std::string &key, std::string &val)
	{
		std::string::size_type pos = str.find_first_of('=');
		if (pos == std::string::npos) {
			return false;
		}
		key = str.substr(0, pos);
		val = str.substr(pos + 1);
		return true;
	}

	bool ParseCommandLine(int argc, char* argv[],
		std::map<std::string, std::string>& res)
	{
		assert(argc >= 1);

		std::string key, val;
		for (int n = 1; n < argc; n++) 
		{
			if (!ParseKeyValue(argv[n], key, val)){
				return false;
			}
			res[key] = val;
		}
		return true;
	}
}

int main(int argc, char* argv[])
{
	std::map<std::string, std::string> cmds;
	if (!ParseCommandLine(argc, argv, cmds))
	{
		std::cerr << "命令行解析错误！" << std::endl;
		return 0;
	}
	std::string sg_id_s = cmds["sg"];
	if (sg_id_s.empty())
	{
		std::cerr << "未指定服务器组号！" << std::endl;
		return 0;
	}
	int sg_id;
	try
	{
		sg_id = std::stoi(sg_id_s);
	}
	catch (...)
	{

	}

	return 0;
}