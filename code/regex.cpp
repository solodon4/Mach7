#include <stdlib.h>
#include <regex>
#include <string>
#include <iostream>

using namespace std;

regex expression ("([0-9]+)(\\-|_|$)(.*)");

// processftp : on success returns the ftp response code, and fills
// msg with the ftp response message.
int process_ftp(const char* response, std::string* msg)
{
	cmatch what ;
	if (regex_match(response, what, expression))
	{
		// what[0] contains the whole string
		// what[1] contains the response code
		// what[2] contains the separator character
		// what[3] contains the text message
		if (msg)
			msg->assign(what[3].first, what[3].second);
		return std::atoi(what[1].first);
	}
	// failure did not match
	if (msg)
		msg->erase() ;
	return -1;
}

int main()
{
	regex  r1("([0-9]+)-([0-9]+)-([0-9]+)");
	cmatch m1;
	regex_match("979-739-3587",m1,r1);
	std::cout << m1[0] << std::endl;
	std::cout << m1[1] << std::endl;
	std::cout << m1[2] << std::endl;
	std::cout << m1[3] << std::endl;
}
