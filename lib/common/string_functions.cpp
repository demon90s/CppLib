#include "string_functions.h"
#include <cstring>
#include <cstdarg>

std::vector<std::string> SplitString(const std::string &str, const char *delim)
{
	std::vector<std::string> result;
	size_t delim_len = strlen(delim);
	std::string::size_type pos1 = 0;
	std::string::size_type pos2 = str.find(delim);

	while (pos2 != std::string::npos)
	{
		result.push_back(std::string(str, pos1, pos2 - pos1));

		pos1 = pos2 + delim_len;
		pos2 = str.find(delim, pos1);
	}

	if (pos1 != str.length())
	{
		result.push_back(std::string(str, pos1));
	}

	return result;
}

std::string StringFormat(const char* fmt, ...)
{
	int buffer_len = 1024;
	char *buffer = new char[buffer_len]{};
	int str_len = -1;

	do
	{
		va_list ap;
		va_start(ap, fmt);
		str_len = vsnprintf(buffer, buffer_len, fmt, ap);

		if (str_len >= buffer_len) {
			str_len = -1;

			char *tmp_buffer = new char[buffer_len * 2]{};
			delete[] buffer;
			buffer_len *= 2;
			buffer = tmp_buffer;
		}
		va_end(ap);
	} while (str_len == -1);

	std::string str = buffer;
	delete[] buffer;

	return str;
}
