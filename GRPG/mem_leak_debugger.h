#ifndef MEM_LEAK_H
#define MEM_LEAK_H

#include <string>
#include <map>
#include "globals.h"
#include "debug_new.h"

using namespace std;

class mem_leak_debugger
{
private:
public:
	static map<void*,string> ptrLinks;
	static string logAllAllocatedPtrs()
	{
		std::map<void*, string>::iterator it;
		for (it = ptrLinks.begin(); it != ptrLinks.end(); ++it)
			runtimeLog << it->second << '\n';
	}
	static void deleteAllAllocatedPtrs()
	{//unlikely to work properly
		std::map<void*, string>::iterator it;
		for (it = ptrLinks.begin(); it != ptrLinks.end(); ++it)
		{
			delete it->first;
		}
	}
};

#endif