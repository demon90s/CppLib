// stacktrace.h (c) 2008, Timo Bingmann from http://idlebox.net/
// published under the WTFPL v2.0

#ifndef _STACKTRACE_H_
#define _STACKTRACE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <execinfo.h>
#include <cxxabi.h>

/** Print a demangled stack backtrace of the caller function to FILE* out. */
void stacktrace(std::string &out, unsigned int max_frames = 6);

#endif // _STACKTRACE_H_
