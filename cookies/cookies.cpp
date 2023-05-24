#include "../hd.h"

std::string set_cookies(std::string name, std::string value)
{
	std::string ret = "Set-Cookie: " + name + "=" + value + "\r\n";
	return (ret);
}
