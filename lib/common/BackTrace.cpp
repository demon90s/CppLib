#include "BackTrace.h"
#include "common/string_functions.h"

void BackTrace(const char* some_content_prefix)
{
	std::string out;
	stacktrace(out);
	std::vector<std::string> frames = SplitString(out, "\n");
	
    printf("[BACKTRACE] %s\n", some_content_prefix);
	for (size_t i = 1; i < frames.size(); ++i)
	{
		printf("#%lu: %s\n", i, frames[i].c_str());
	}
    fflush(stdout);
}
