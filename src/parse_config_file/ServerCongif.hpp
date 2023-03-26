#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "everything.hpp"


class ServerCongif
{
private:
	_string         __port;
	_string			__body_size;
	_string         __host;
	_string     	__name;
	// _string         __index;
	_locations		__location;
public:
	ServerCongif(_string port,_string body_size, _string host, _string name, _locations const &loc):__location(loc)
	{
		__port = port;
		__body_size = body_size;
		__host = host;
		__name = name;
		// __index = index;
	}
	~ServerCongif(){};
};

// ServerCongif::ServerCongif()
// {
// }

// ServerCongif::~ServerCongif()
// {
// }



#endif