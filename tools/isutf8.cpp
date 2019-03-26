// 该程序输出参数表示的文件, 是否是 utf8 无 BOM 格式
// 在我的工作中, 有一些文件必须是 utf8 无 BOM 的, 此工具可以做一个检查

// 如果判断成功, 返回0

#include <cstdio>
#include <fstream>
#include "common/IsUTF8.h"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		printf("Usage: %s filename\n", argv[0]);
		return 1;
	}

	std::ifstream ifs(argv[1]);

	if (!ifs) {
		printf("open file failed\n");
		return 1;
	}

	std::string file;
	std::string line;
	while (getline(ifs, line)) {
		file += line;
		file += '\n';
	}

	bool is_utf8_no_bom = IsUTF8(file.c_str(), file.length()) && !IsUTF8_Bom(file.c_str(), file.length());

	if (is_utf8_no_bom) {
		printf("%s is utf8 no bom\n", argv[1]);
		return 0;
	}

	printf("%s is NOT utf8 no bom\n", argv[1]);
	return 1;
}
