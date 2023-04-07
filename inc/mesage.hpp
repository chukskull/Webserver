#ifndef MESSAGE_HPP
# define  MESSAGE_HPP

#include "headers.hpp"

struct Mesage
{
	_string						message;
	//lwl client tani is the server
	std::pair<size_t, size_t>	_connections;
};


#endif