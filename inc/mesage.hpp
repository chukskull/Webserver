#ifndef MESSAGE_HPP
# define  MESSAGE_HPP

#include "headers.hpp"


struct Mesage
{
	_string									message;
	_string									response;
	//lwl client tani is the server
	//int lwla index server;
	//int tanya index port;
	std::pair<int, std::pair<int, int> >	_connections;
};


#endif