#include "parsing.hpp"

bool isWhitespace(char c) 
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

size_t	escape_white_space(_string my_string)
{
	size_t k = 0;
	for(; k < my_string.size(); k++)
	{
		if(!isWhitespace(my_string[k]))
			break ;
	}
	return k;
}

_string		erase_some_charc(_string my_string)
{
	std::vector<char> sure;
	sure.push_back(' ');
	sure.push_back('\t');
	sure.push_back('\n');
	sure.push_back('\r');
	sure.push_back(';');
	for (size_t i = 0; i < sure.size(); i++)
	{
		my_string.erase(remove(my_string.begin(), my_string.end(), sure[i]), my_string.end());
	}
	return my_string;
}