#ifndef _STRING_FX_H                 // Prevent multiple definitions if this 
#define _STRING_FX_H                 // file is included in more than one place

#include <string>
#include <algorithm>

//using namespace std;

class String_Functions
{
public:
	static std::string replaceAll(std::string s,const char a,const char b) {
		std::replace(s.begin(), s.end(), a, b); // replace all 'x' to 'y'
		return s;
	}
};

#endif