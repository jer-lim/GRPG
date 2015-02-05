#ifndef _STRING_FX_H                 // Prevent multiple definitions if this 
#define _STRING_FX_H                 // file is included in more than one place

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>

//using namespace std;

class String_Functions
{
public:
	static std::string replaceAll(std::string s,const char a,const char b) {
		std::replace(s.begin(), s.end(), a, b); // replace all 'x' to 'y'
		return s;
	}
	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	static std::string translateNewLines(string txt)
	{
		stringstream ss;
		char tempChar;
		queue<char> last2Characters;
		string realText = "";
		ss << txt;
		tempChar = ss.get();
		while (tempChar != EOF){
			last2Characters.push(tempChar);
			if (last2Characters.size() > 2){
				realText += last2Characters.front();
				last2Characters.pop();
			}
			if (last2Characters.front() == '\\' && tempChar == 'n'){
				realText = realText + '\n';
				last2Characters.pop();
				last2Characters.pop();
			}
			tempChar = ss.get();
		}

		while (last2Characters.size() > 0){
			realText += last2Characters.front();
			last2Characters.pop();
		}
		return realText;
	}


	static std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}
};

#endif